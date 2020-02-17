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

///@cond DOXYGEN_HIDDEN

#ifndef TALIPOT_OGDF_LAYOUT_PLUGIN_BASE_H
#define TALIPOT_OGDF_LAYOUT_PLUGIN_BASE_H

#include <talipot/config.h>
#include <talipot/PluginHeaders.h>
#include <talipot/PropertyAlgorithm.h>

#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>

#include "TalipotToOGDF.h"

class TLP_OGDF_SCOPE OGDFLayoutPluginBase : public tlp::LayoutAlgorithm {
public:
  OGDFLayoutPluginBase(const tlp::PluginContext *context, LayoutModule *ogdfLayoutAlgo);
  ~OGDFLayoutPluginBase() override;

  bool run() override;

protected:
  virtual void beforeCall() {}
  virtual void callOGDFLayoutAlgorithm(ogdf::GraphAttributes &gAttributes);
  virtual void afterCall() {}

  void transposeLayoutVertically();

  TalipotToOGDF *tlpToOGDF;
  ogdf::LayoutModule *ogdfLayoutAlgo;
};

#endif // TALIPOT_OGDF_LAYOUT_PLUGIN_BASE_H
       ///@endcond