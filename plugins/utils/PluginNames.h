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

#ifndef PLUGIN_NAMES_H
#define PLUGIN_NAMES_H

namespace tlp {
namespace ViewName {

const static char GeographicViewName[] = "Geographic";
const static char HistogramViewName[] = "Histogram";
const static char MatrixViewName[] = "Matrix";
const static char ParallelCoordinatesViewName[] = "Parallel Coordinates";
const static char PixelOrientedViewName[] = "Pixel Oriented";
const static char ScatterPlot2DViewName[] = "Scatter Plot 2D";
const static char SOMViewName[] = "Self Organizing Map";
} // namespace ViewName

namespace SelectionAlgorithm {

const static char LoopSelection[] = "Loop Selection";
const static char ParallelEdgesSelection[] = "Parallel Edges Selection";
const static char InducedSubGraphSelection[] = "Induced SubGraph";
const static char ReachableSubGraphSelection[] = "Reachable SubGraph";
const static char SpanningDagSelection[] = "Spanning Dag";
const static char MakeSelectionGraph[] = "Make Selection a Graph";
} // namespace SelectionAlgorithm

namespace InteractorName {
const static char InteractorNavigation[] = "InteractorNavigation";
const static char HistogramInteractorNavigation[] = "Histogram Navigation Interactor";
const static char PixelOrientedInteractorNavigation[] = "PixelOriented Navigation Interactor";
const static char SOMViewNavigation[] = "SOM Navigation Interactor";
const static char ScatterPlot2DInteractorNavigation[] = "ScatterPlot2D Navigation Interactor";
} // namespace InteractorName

}

#endif // PLUGIN_NAMES_H
