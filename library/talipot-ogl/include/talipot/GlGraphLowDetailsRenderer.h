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

#ifndef TALIPOT_GL_GRAPH_LOW_DETAILS_RENDERER_H
#define TALIPOT_GL_GRAPH_LOW_DETAILS_RENDERER_H

#include <talipot/OpenGlIncludes.h>

#include <vector>

#include <talipot/Observable.h>
#include <talipot/GlGraphRenderer.h>

namespace tlp {

class Graph;
class GlScene;
class LayoutProperty;
class ColorProperty;
class SizeProperty;
class BooleanProperty;

/** \brief Class to display graph with very simple and very fast renderer
 *
 * Very high performance renderer
 * This class display graph with :
 *  - Nodes : quads
 *  - Edges : lines
 * Warning : this renderer doesn't work for selection
 *
 * See GlGraphRenderer documentation for functions documentations
 */
class TLP_GL_SCOPE GlGraphLowDetailsRenderer : public GlGraphRenderer, public Observable {

public:
  GlGraphLowDetailsRenderer(const GlGraphInputData *inputData);

  ~GlGraphLowDetailsRenderer() override;

  void draw(float lod, Camera *camera) override;

protected:
  void initEdgesArray();
  void initTexArray(uint glyph, Vec2f tex[4]);
  void initNodesArray();

  void addObservers();
  void removeObservers();
  void updateObservers();
  void treatEvent(const Event &ev) override;

  GlScene *fakeScene;

  bool buildVBO;

  std::vector<Vec2f> points;
  std::vector<Color> colors;
  std::vector<GLuint> indices;

  std::vector<Vec2f> quad_points;
  std::vector<Color> quad_colors;
  std::vector<GLuint> quad_indices;

  Graph *observedGraph;
  LayoutProperty *observedLayoutProperty;
  ColorProperty *observedColorProperty;
  SizeProperty *observedSizeProperty;
  BooleanProperty *observedSelectionProperty;
};
}

#endif // TALIPOT_GL_GRAPH_LOW_DETAILS_RENDERER_H
