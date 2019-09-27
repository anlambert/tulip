/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#include <GL/glew.h>

#include <QOpenGLFramebufferObject>

#include <talipot/Camera.h>
#include <talipot/GlOffscreenRenderer.h>
#include <talipot/GlMainWidget.h>
#include <talipot/GlVertexArrayManager.h>
#include <talipot/GlGraphComposite.h>
#include <talipot/OpenGlConfigManager.h>
#include <talipot/GlTools.h>

#include <sstream>

using namespace std;

namespace tlp {

GlOffscreenRenderer *GlOffscreenRenderer::instance = new GlOffscreenRenderer();

GlOffscreenRenderer::GlOffscreenRenderer()
    : vPWidth(512), vPHeight(512), glFrameBuf(nullptr), glFrameBuf2(nullptr),
      mainLayer(new GlLayer("Main")), entitiesCpt(0), zoomFactor(DBL_MAX),
      cameraCenter(FLT_MAX, FLT_MAX, FLT_MAX) {
  GlLayer *backgroundLayer = new GlLayer("Background");
  backgroundLayer->setVisible(true);
  GlLayer *foregroundLayer = new GlLayer("Foreground");
  foregroundLayer->setVisible(true);
  backgroundLayer->set2DMode();
  foregroundLayer->set2DMode();
  scene.addExistingLayer(backgroundLayer);
  scene.addExistingLayer(mainLayer);
  scene.addExistingLayer(foregroundLayer);
  antialiasedFbo = false;
}

GlOffscreenRenderer::~GlOffscreenRenderer() {
  delete glFrameBuf;
  delete glFrameBuf2;
}

void GlOffscreenRenderer::setViewPortSize(const unsigned int viewPortWidth,
                                          const unsigned int viewPortHeight) {
  vPWidth = viewPortWidth;
  vPHeight = viewPortHeight;
}

void GlOffscreenRenderer::setSceneBackgroundColor(const Color &color) {
  scene.setBackgroundColor(color);
}

unsigned int GlOffscreenRenderer::getViewportWidth() {
  return glFrameBuf->width();
}

unsigned int GlOffscreenRenderer::getViewportHeight() {
  return glFrameBuf->height();
}

void GlOffscreenRenderer::addGlEntityToScene(GlSimpleEntity *entity) {
  ostringstream oss;
  oss << "entity " << ++entitiesCpt;
  mainLayer->addGlEntity(entity, oss.str());
}

void GlOffscreenRenderer::addGraphToScene(Graph *graph) {
  addGraphCompositeToScene(new GlGraphComposite(graph));
}

void GlOffscreenRenderer::addGraphCompositeToScene(GlGraphComposite *graphComposite) {
  // Delete old composite if it exist
  GlSimpleEntity *oldComposite = mainLayer->findGlEntity("graph");

  if (oldComposite != nullptr) {
    mainLayer->deleteGlEntity(oldComposite);
  }

  GlVertexArrayManager *vertexArrayManager =
      graphComposite->getInputData()->getGlVertexArrayManager();
  vertexArrayManager->setHaveToComputeAll(true);
  mainLayer->addGlEntity(graphComposite, "graph");
}

void GlOffscreenRenderer::clearScene(bool deleteGlEntities) {
  mainLayer->getComposite()->reset(deleteGlEntities);
  const vector<pair<string, GlLayer *>> &layersList = scene.getLayersList();

  for (unsigned int i = 0; i < layersList.size(); ++i) {
    if (layersList[i].second != mainLayer) {
      layersList[i].second->getComposite()->reset(true);
    }
  }

  entitiesCpt = 0;
  zoomFactor = DBL_MAX;
}

void GlOffscreenRenderer::initFrameBuffers(const bool antialiased) {

  antialiasedFbo = antialiased && QOpenGLFramebufferObject::hasOpenGLFramebufferBlit();

  if (glFrameBuf != nullptr &&
      (vPWidth != uint(glFrameBuf->width()) || vPHeight != uint(glFrameBuf->height()))) {
    delete glFrameBuf;
    glFrameBuf = nullptr;
    delete glFrameBuf2;
    glFrameBuf2 = nullptr;
  }

  if (glFrameBuf == nullptr) {
    QOpenGLFramebufferObjectFormat fboFmt;
    fboFmt.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    if (antialiasedFbo)
      fboFmt.setSamples(OpenGlConfigManager::maxNumberOfSamples());

    glFrameBuf = new QOpenGLFramebufferObject(vPWidth, vPHeight, fboFmt);
  }

  if (antialiasedFbo && glFrameBuf2 == nullptr) {
    glFrameBuf2 = new QOpenGLFramebufferObject(vPWidth, vPHeight);
  }
}

void GlOffscreenRenderer::renderScene(const bool centerScene, const bool antialiased) {

  // If no OpenGL context, activate the default one in order to avoid segfault when trying to render
  // an OpenGL scene
  if (!QGLContext::currentContext()) {
    QGLWidget *firstWidget = GlMainWidget::getFirstQGLWidget();
    firstWidget->makeCurrent();
  }

  initFrameBuffers(antialiased);

  scene.setViewport(0, 0, vPWidth, vPHeight);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  GL_TEST_ERROR();
  glFrameBuf->bind();
  // the line above can produce an invalid op error, forget it
  glGetError();
  GL_TEST_ERROR();

  if (centerScene) {
    scene.centerScene();
  }

  Camera &camera = mainLayer->getCamera();

  if (cameraCenter != Coord(FLT_MAX, FLT_MAX, FLT_MAX)) {
    camera.setCenter(cameraCenter);
    camera.setEyes(Coord(0, 0, camera.getSceneRadius()));
    camera.setEyes(camera.getEyes() + camera.getCenter());
    camera.setUp(Coord(0, 1., 0));
  }

  if (zoomFactor != DBL_MAX) {
    camera.setZoomFactor(zoomFactor);
  }

  scene.draw();
  glFrameBuf->release();

  if (antialiasedFbo)
    QOpenGLFramebufferObject::blitFramebuffer(
        glFrameBuf2, QRect(0, 0, glFrameBuf2->width(), glFrameBuf2->height()), glFrameBuf,
        QRect(0, 0, glFrameBuf->width(), glFrameBuf->height()));

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  GL_TEST_ERROR();
  glPopAttrib();
  // the line above can produce an invalid op error, forget it
  glGetError();
  GL_TEST_ERROR();
}

void GlOffscreenRenderer::renderExternalScene(GlScene *scene, const bool antialiased) {
  // If no OpenGL context, activate the default one in order to avoid segfault when trying to render
  // an OpenGL scene
  if (!QGLContext::currentContext()) {
    QGLWidget *firstWidget = GlMainWidget::getFirstQGLWidget();
    firstWidget->makeCurrent();
  }

  initFrameBuffers(antialiased);

  Vector<int, 4> backupViewport = scene->getViewport();

  scene->setViewport(0, 0, vPWidth, vPHeight);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glFrameBuf->bind();
  scene->draw();
  glFrameBuf->release();

  if (antialiasedFbo)
    QOpenGLFramebufferObject::blitFramebuffer(
        glFrameBuf2, QRect(0, 0, glFrameBuf2->width(), glFrameBuf2->height()), glFrameBuf,
        QRect(0, 0, glFrameBuf->width(), glFrameBuf->height()));

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  GL_TEST_ERROR();
  glPopAttrib();
  // the line above can produce an invalid op error, forget it
  glGetError();
  GL_TEST_ERROR();

  scene->setViewport(backupViewport);
}

bool GlOffscreenRenderer::frameBufferOk() const {
  return glFrameBuf->isValid();
}

static QImage convertImage(const QImage &image) {
  return QImage(image.bits(), image.width(), image.height(), QImage::Format_ARGB32)
      .convertToFormat(QImage::Format_RGB32);
}

QImage GlOffscreenRenderer::getImage() {
  if (!antialiasedFbo)
    return convertImage(glFrameBuf->toImage());
  else
    return convertImage(glFrameBuf2->toImage());
}

GLuint GlOffscreenRenderer::getGLTexture(const bool generateMipMaps) {

  bool canUseMipmaps = OpenGlConfigManager::isExtensionSupported("GL_ARB_framebuffer_object") ||
                       OpenGlConfigManager::isExtensionSupported("GL_EXT_framebuffer_object");

  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureId);

  if (generateMipMaps && canUseMipmaps) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  GL_TEST_ERROR();
  QImage image = getImage().mirrored();
  // the line above can produce an invalid op error, forget it
  glGetError();
  GL_TEST_ERROR();
  unsigned char *buff = image.bits();

  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getViewportWidth(), getViewportHeight(), 0, GL_BGRA,
               GL_UNSIGNED_BYTE, buff);

  if (generateMipMaps && canUseMipmaps) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  GL_TEST_ERROR();
  glDisable(GL_TEXTURE_2D);
  GL_TEST_ERROR();

  return textureId;
}
} // namespace tlp