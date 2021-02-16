/**
 *
 * Copyright (C) 2019-2021  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <GL/glew.h>

#include <talipot/GlMainWidget.h>

// remove warnings about qt5/glew incompatibility
// as we do not rely on QOpenGLFunctions for rendering
#undef __GLEW_H__
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#define __GLEW_H__

#include <QSurfaceFormat>
#include <QOffscreenSurface>

#include <talipot/TlpQtTools.h>
#include <talipot/Settings.h>
#include <talipot/Graph.h>
#include <talipot/GlTools.h>
#include <talipot/GlTextureManager.h>
#include <talipot/Gl2DRect.h>
#include <talipot/GlQuadTreeLODCalculator.h>
#include <talipot/GLInteractor.h>
#include <talipot/GlGraph.h>
#include <talipot/Interactor.h>
#include <talipot/GlCompositeHierarchyManager.h>
#include <talipot/GlVertexArrayManager.h>
#include <talipot/View.h>
#include <talipot/Camera.h>
#include <talipot/OpenGlConfigManager.h>
#include <talipot/GlOffscreenRenderer.h>
#include <talipot/GlTextureManager.h>

#include <cstdint>

using namespace std;

namespace tlp {

bool GlMainWidget::inRendering = false;

//==================================================
GlMainWidget::GlMainWidget(QWidget *parent, View *view)
    : QOpenGLWidget(parent), scene(new GlQuadTreeLODCalculator), view(view), widthStored(0),
      heightStored(0), glFrameBuf(nullptr), glFrameBuf2(nullptr),
      keepPointOfViewOnSubgraphChanging(false),
      sceneTextureId("scene" + to_string(reinterpret_cast<uintptr_t>(this))) {
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  grabGesture(Qt::PinchGesture);
  grabGesture(Qt::PanGesture);
  grabGesture(Qt::SwipeGesture);
  makeCurrent();
  QSurfaceFormat format;
  format.setSamples(OpenGlConfigManager::maxNumberOfSamples());
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
#ifndef NDEBUG
  format.setOption(QSurfaceFormat::DebugContext);
#endif
  setFormat(format);
  getScene()->setViewOrtho(Settings::isViewOrtho());
  OpenGlConfigManager::initExtensions();
  doneCurrent();
}
//==================================================
GlMainWidget::~GlMainWidget() {
  delete glFrameBuf;
  delete glFrameBuf2;
}
//==================================================
void GlMainWidget::paintEvent(QPaintEvent *) {
  QRegion rect = this->visibleRegion();

  // If the visible are changed we need to draw the entire scene
  // Because the saved snapshot only backup the visible part of the
  // Graph
  if (rect.boundingRect() != _visibleArea.boundingRect()) {
    _visibleArea = rect;
    draw();
  } else {
    redraw();
  }

  _visibleArea = rect; // Save the new visible area.
}
//==================================================
void GlMainWidget::closeEvent(QCloseEvent *e) {
  emit closing(this, e);
}
//==================================================
void GlMainWidget::createFramebuffers(int width, int height) {

  if (!glFrameBuf || glFrameBuf->size().width() != width || glFrameBuf->size().height() != height) {
    makeCurrent();
    deleteFramebuffers();
    QOpenGLFramebufferObjectFormat fboFormat;
    fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    fboFormat.setSamples(OpenGlConfigManager::maxNumberOfSamples());
    glFrameBuf = new QOpenGLFramebufferObject(width, height, fboFormat);
    glFrameBuf2 = new QOpenGLFramebufferObject(width, height);
    GlTextureManager::registerExternalTexture(sceneTextureId, glFrameBuf2->texture());
    widthStored = width;
    heightStored = height;
  }
}
//==================================================
void GlMainWidget::deleteFramebuffers() {
  delete glFrameBuf;
  glFrameBuf = nullptr;
  delete glFrameBuf2;
  glFrameBuf2 = nullptr;
}

//==================================================
void GlMainWidget::render(RenderingOptions options, bool checkVisibility) {

  if ((isVisible() || !checkVisibility) && !inRendering) {

    // begin rendering process
    inRendering = true;
    makeCurrent();

    // backup internal QOpenGLWidget bound framebuffer id
    int drawFboId = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &drawFboId);

    // get the content width and height
    int width = screenToViewport(contentsRect().width());
    int height = screenToViewport(contentsRect().height());

    // if the framebuffers have invalid size, new ones need to be created
    // so force the RenderScene flag.
    if (widthStored != width || heightStored != height) {
      options |= RenderScene;
    }

    computeInteractors();

    if (options.testFlag(RenderScene)) {
      createFramebuffers(width, height);

      // render the graph in the antialiased framebuffer.
      glFrameBuf->bind();
      scene.draw();
      glFrameBuf->release();

      // copy rendered scene in a texture/QImage compatible framebuffer
      QRect fbRect(0, 0, width, height);
      QOpenGLFramebufferObject::blitFramebuffer(glFrameBuf2, fbRect, glFrameBuf, fbRect);

      // restore internal QOpenGLWidget framebuffer binding
      makeCurrent();
      glBindFramebuffer(GL_FRAMEBUFFER, drawFboId);
    } else {
      scene.initGlParameters();
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);

    // draw rendered scene from texture
    Camera camera2D(scene.getGraphCamera().getScene(), false);
    camera2D.setScene(scene.getGraphCamera().getScene());
    camera2D.initGl();
    Gl2DRect rect(height, 0, 0, width, sceneTextureId);
    rect.draw(0, &camera2D);
    scene.getGraphCamera().initGl();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);

    // draw interactors and foreground entities.
    drawInteractors();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_STENCIL_TEST);

    if (options.testFlag(SwapBuffers)) {
      update();
    }

    inRendering = false;
  }
}
//==================================================
void GlMainWidget::redraw() {
  render(SwapBuffers);
  emit viewRedrawn(this);
}
//==================================================
void GlMainWidget::draw(bool graphChanged) {
  render(RenderingOptions(RenderScene | SwapBuffers));
  emit viewDrawn(this, graphChanged);
}
//==================================================
void GlMainWidget::computeInteractors() {
  if (!view) {
    return;
  }

  GLInteractorComposite *interactor =
      dynamic_cast<GLInteractorComposite *>(view->currentInteractor());

  if (interactor == nullptr) {
    return;
  }

  interactor->compute(this);
}
//==================================================
void GlMainWidget::drawInteractors() {
  if (!view) {
    return;
  }

  GLInteractorComposite *interactor =
      dynamic_cast<GLInteractorComposite *>(view->currentInteractor());

  if (!interactor) {
    return;
  }

  interactor->draw(this);
}
//==================================================
// QGLWidget slots
//==================================================
void GlMainWidget::resizeGL(int w, int h) {

  if (w == 0 || h == 0) {
    return;
  }

  int width = contentsRect().width();
  int height = contentsRect().height();

  deleteFramebuffers();

  scene.setViewport(0, 0, screenToViewport(width), screenToViewport(height));

  emit glResized(w, h);
}
//==================================================
void GlMainWidget::makeCurrent() {
  if (isVisible()) {
    QOpenGLWidget::makeCurrent();
    int width = contentsRect().width();
    int height = contentsRect().height();
    scene.setViewport(0, 0, screenToViewport(width), screenToViewport(height));
  } else {
    GlOffscreenRenderer::instance().makeOpenGLContextCurrent();
  }
}
//==================================================
void GlMainWidget::doneCurrent() {
  if (isVisible()) {
    QOpenGLWidget::doneCurrent();
  } else {
    GlOffscreenRenderer::instance().doneOpenGLContextCurrent();
  }
}
//==================================================
bool GlMainWidget::pickGlEntities(const int x, const int y, const int width, const int height,
                                  std::vector<SelectedEntity> &pickedEntities, GlLayer *layer) {
  makeCurrent();
  return scene.selectEntities(
      static_cast<RenderingEntitiesFlag>(RenderingEntities | RenderingWithoutRemove),
      screenToViewport(x), screenToViewport(y), screenToViewport(width), screenToViewport(height),
      layer, pickedEntities);
}
//==================================================
bool GlMainWidget::pickGlEntities(const int x, const int y,
                                  std::vector<SelectedEntity> &pickedEntities, GlLayer *layer) {
  return pickGlEntities(x, y, 2, 2, pickedEntities, layer);
}
//==================================================
void GlMainWidget::pickNodesEdges(const int x, const int y, const int width, const int height,
                                  std::vector<SelectedEntity> &selectedNodes,
                                  std::vector<SelectedEntity> &selectedEdges, GlLayer *layer,
                                  bool pickNodes, bool pickEdges) {
  makeCurrent();

  if (pickNodes) {
    scene.selectEntities(
        static_cast<RenderingEntitiesFlag>(RenderingNodes | RenderingWithoutRemove),
        screenToViewport(x), screenToViewport(y), screenToViewport(width), screenToViewport(height),
        layer, selectedNodes);
  }

  if (pickEdges) {
    scene.selectEntities(
        static_cast<RenderingEntitiesFlag>(RenderingEdges | RenderingWithoutRemove),
        screenToViewport(x), screenToViewport(y), screenToViewport(width), screenToViewport(height),
        layer, selectedEdges);
  }
}
//=====================================================
bool GlMainWidget::pickNodesEdges(const int x, const int y, SelectedEntity &selectedEntity,
                                  GlLayer *layer, bool pickNodes, bool pickEdges) {
  makeCurrent();
  vector<SelectedEntity> selectedEntities;

  if (pickNodes && scene.selectEntities(
                       static_cast<RenderingEntitiesFlag>(RenderingNodes | RenderingWithoutRemove),
                       screenToViewport(x - 1), screenToViewport(y - 1), screenToViewport(3),
                       screenToViewport(3), layer, selectedEntities)) {
    selectedEntity = selectedEntities[0];
    return true;
  }

  if (pickEdges && scene.selectEntities(
                       static_cast<RenderingEntitiesFlag>(RenderingEdges | RenderingWithoutRemove),
                       screenToViewport(x - 1), screenToViewport(y - 1), screenToViewport(3),
                       screenToViewport(3), layer, selectedEntities)) {
    selectedEntity = selectedEntities[0];
    return true;
  }

  return false;
}
//=====================================================
void GlMainWidget::getTextureRealSize(int width, int height, int &textureRealWidth,
                                      int &textureRealHeight) {
  textureRealWidth = 1;
  textureRealHeight = 1;

  while (textureRealWidth <= width) {
    textureRealWidth *= 2;
  }

  while (textureRealHeight <= height) {
    textureRealHeight *= 2;
  }

  if (textureRealWidth > 4096) {
    textureRealHeight = textureRealHeight / (textureRealWidth / 8192);
    textureRealWidth = 4096;
  }

  if (textureRealHeight > 4096) {
    textureRealWidth = textureRealWidth / (textureRealHeight / 8192);
    textureRealHeight = 4096;
  }
}
//=====================================================
void GlMainWidget::createPicture(const std::string &pictureName, int width, int height,
                                 bool center) {
  createPicture(width, height, center).save(tlp::tlpStringToQString(pictureName));
}
//=====================================================
QImage GlMainWidget::createPicture(int width, int height, bool center, QImage::Format format) {

  QImage resultImage;

  makeCurrent();

  QOpenGLFramebufferObjectFormat fboFormat;
  fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
  fboFormat.setSamples(OpenGlConfigManager::maxNumberOfSamples());
  QOpenGLFramebufferObject *frameBuf = new QOpenGLFramebufferObject(width, height, fboFormat);
  QOpenGLFramebufferObject *frameBuf2 = new QOpenGLFramebufferObject(width, height);

  if (frameBuf->isValid() && frameBuf2->isValid()) {
    frameBuf->bind();

    int oldWidth = scene.getViewport()[2];
    int oldHeight = scene.getViewport()[3];
    vector<Camera> oldCameras;
    const vector<pair<string, GlLayer *>> &layersList = scene.getLayersList();

    if (center) {
      for (const auto &it : layersList) {
        if (!it.second->useSharedCamera()) {
          oldCameras.push_back(it.second->getCamera());
        }
      }
    }

    scene.setViewport(0, 0, width, height);

    if (center) {
      scene.adjustSceneToSize(width, height);
    }

    computeInteractors();
    scene.draw();
    drawInteractors();
    frameBuf->release();

    QOpenGLFramebufferObject::blitFramebuffer(frameBuf2, QRect(0, 0, width, height), frameBuf,
                                              QRect(0, 0, width, height));

    resultImage = frameBuf2->toImage();

    scene.setViewport(0, 0, oldWidth, oldHeight);

    if (center) {
      int i = 0;

      for (const auto &it : layersList) {
        if (!it.second->useSharedCamera()) {
          Camera &camera = it.second->getCamera();
          camera.setCenter(oldCameras[i].getCenter());
          camera.setEyes(oldCameras[i].getEyes());
          camera.setSceneRadius(oldCameras[i].getSceneRadius());
          camera.setUp(oldCameras[i].getUp());
          camera.setZoomFactor(oldCameras[i].getZoomFactor());
        }

        i++;
      }
    }
  }

  delete frameBuf;
  delete frameBuf2;

  // The QOpenGLFramebufferObject returns the wrong image format
  // QImage::Format_ARGB32_Premultiplied. We need to create an image from original data with the
  // right format QImage::Format_ARGB32. We need to clone the data as when the image var will be
  // destroy at the end of the function it's data will be destroyed too and the newly created image
  // object will have invalid data pointer.
  return QImage(resultImage.bits(), resultImage.width(), resultImage.height(),
                QImage::Format_ARGB32)
      .convertToFormat(format);
}

void GlMainWidget::centerScene(bool graphChanged, float zf) {
  makeCurrent();
  scene.centerScene();

  if (zf != 1) {
    scene.zoomFactor(zf);
  }

  draw(graphChanged);
}

void GlMainWidget::emitGraphChanged() {
  emit graphChanged();
}

void GlMainWidget::setKeepScenePointOfViewOnSubgraphChanging(bool k) {
  keepPointOfViewOnSubgraphChanging = k;
}

bool GlMainWidget::keepScenePointOfViewOnSubgraphChanging() const {
  return keepPointOfViewOnSubgraphChanging;
}

GlGraphRenderingParameters &GlMainWidget::getGlGraphRenderingParameters() {
  return scene.getGlGraph()->getRenderingParameters();
}

GlGraphInputData *GlMainWidget::getGlGraphInputData() const {
  return scene.getGlGraph()->getInputData();
}

}
