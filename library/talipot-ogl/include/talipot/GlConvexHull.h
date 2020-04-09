/**
 *
 * Copyright (C) 2019-2020  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#ifndef TALIPOT_GL_CONVEX_HULL_H
#define TALIPOT_GL_CONVEX_HULL_H

#include <vector>

#include <talipot/Color.h>
#include <talipot/GlComposite.h>

namespace tlp {

struct ConvexHullItem;
class Graph;

/** \brief Class used to represent ConvexHull
 *
 * Class used to represent ConvexHull. GlHierarchyConvexHull class use this to create all convexHull
 * of the graph
 */
class TLP_GL_SCOPE GlConvexHull : public GlComposite {
public:
  /**
   * Default constructor
   */
  GlConvexHull() {}

  /**
   * Build a Polygon with the convex hull of points
   */
  GlConvexHull(const std::vector<Coord> &points, const std::vector<Color> &fillColors,
               const std::vector<Color> &outlineColors, const bool filled, const bool outlined,
               const std::string &name, bool computeHull = true);

  ~GlConvexHull() override {}

  /**
   * Function used to visit composite's children
   */
  void acceptVisitor(GlSceneVisitor *visitor) override;

  /**
   * Return the name of this convex hull
   */
  std::string getName() {
    return _name;
  }

  /**
   * Draw the convexHull
   */
  void draw(float lod, Camera *camera) override;

  /**
   * Static function who build a hierarchy of convexHull with the given graph
   */
  static ConvexHullItem *buildConvexHullsFromHierarchy(Graph *graph, std::vector<Color> fColors,
                                                       std::vector<Color> oColors,
                                                       bool deduceFromChilds = true,
                                                       Graph *root = nullptr,
                                                       unsigned int depth = 0);

  /**
   * Translate entity
   */
  void translate(const Coord &move) override;

  /**
   * Function to export data in outString (in XML format)
   */
  void getXML(std::string &outString) override;

  /**
   * Function to set data with inString (in XML format)
   */
  void setWithXML(const std::string &inString, unsigned int &currentPosition) override;

  tlp::Graph *_graph;

protected:
  std::vector<Coord> _points;
  std::vector<Color> _fillColors;
  std::vector<Color> _outlineColors;
  bool _filled;
  bool _outlined;
  std::string _name;
};

struct ConvexHullItem {
  GlConvexHull *hull;
  Graph *_graph;
  std::string name;
  std::vector<ConvexHullItem *> children;
};
}
#endif // TALIPOT_GL_CONVEX_HULL_H
