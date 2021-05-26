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

#ifndef PARALLEL_COORDINATES_GRAPH_PROXY_H
#define PARALLEL_COORDINATES_GRAPH_PROXY_H

#include <talipot/GraphDecorator.h>
#include <talipot/Size.h>
#include <talipot/StableIterator.h>
#include <talipot/StoredType.h>

namespace tlp {

class ColorProperty;

class ParallelCoordinatesGraphProxy : public GraphDecorator {

public:
  ParallelCoordinatesGraphProxy(Graph *graph, const ElementType location = NODE);
  ~ParallelCoordinatesGraphProxy() override;

  unsigned int getNumberOfSelectedProperties() const;
  bool selectedPropertiesisEmpty() const;
  std::vector<std::string> getAllProperties();
  std::vector<std::string> getSelectedProperties();
  void setSelectedProperties(const std::vector<std::string> &properties);
  void removePropertyFromSelection(const std::string &propertyName);

  ElementType getDataLocation() const;
  void setDataLocation(const ElementType location) {
    dataLocation = location;
  }

  unsigned int getDataCount() const;
  Iterator<unsigned int> *getDataIterator();
  Iterator<unsigned int> *getSelectedDataIterator();
  Iterator<unsigned int> *getUnselectedDataIterator();
  Color getDataColor(const unsigned int dataId);
  std::string getDataTexture(const unsigned int dataId);
  Size getDataViewSize(const unsigned int dataId);
  std::string getDataLabel(const unsigned int dataId);

  bool isDataSelected(const unsigned int dataId);
  void setDataSelected(const unsigned int dataId, const bool dataSelected);
  void resetSelection();

  void deleteData(const unsigned int dataId);

  Graph *getGraph() const {
    return graph_component;
  }

  void setUnhighlightedEltsColorAlphaValue(const unsigned int alpha) {
    unhighlightedEltsColorAlphaValue = alpha;
  }
  unsigned int getUnhighlightedEltsColorAlphaValue() const {
    return unhighlightedEltsColorAlphaValue;
  }
  void addOrRemoveEltToHighlight(const unsigned int eltId);
  void unsetHighlightedElts();
  bool highlightedEltsSet() const;
  bool isDataHighlighted(const unsigned int dataId);
  void selectHighlightedElements();
  void setSelectHighlightedElements(bool val);
  const std::set<unsigned int> &getHighlightedElts() const {
    return highlightedElts;
  }
  void resetHighlightedElts(const std::set<unsigned int> &highlightedData);
  void removeHighlightedElement(const unsigned int dataId);
  bool graphColorsModified() const {
    return graphColorsChanged;
  }
  void colorDataAccordingToHighlightedElts();

  void treatEvents(const std::vector<Event> &) override;

  template <typename Proxytype>
  Proxytype *getLocalProperty(const std::string &name) {
    return graph_component->getLocalProperty<Proxytype>(name);
  }

  template <typename Proxytype>
  Proxytype *getProperty(const std::string &name) {
    return graph_component->getProperty<Proxytype>(name);
  }

  PropertyInterface *getProperty(const std::string &name) const override {
    return graph_component->getProperty(name);
  }

  template <typename PROPERTY, typename PROPERTYTYPE>
  TYPE_CONST_REFERENCE(PROPERTYTYPE)
  getPropertyValueForData(const std::string &propertyName, const unsigned int dataId) {
    if (getDataLocation() == NODE) {
      return getProperty<PROPERTY>(propertyName)->getNodeValue(node(dataId));
    } else {
      return getProperty<PROPERTY>(propertyName)->getEdgeValue(edge(dataId));
    }
  }

  template <typename PROPERTY, typename PROPERTYTYPE>
  void setPropertyValueForData(const std::string &propertyName, const unsigned int dataId,
                               const typename PROPERTYTYPE::RealType &propertyValue) {
    if (getDataLocation() == NODE) {
      getProperty<PROPERTY>(propertyName)->setNodeValue(node(dataId), propertyValue);
    } else {
      getProperty<PROPERTY>(propertyName)->setEdgeValue(edge(dataId), propertyValue);
    }
  }

  template <typename PROPERTY, typename PROPERTYTYPE>
  void setPropertyValueForAllData(const std::string &propertyName,
                                  const typename PROPERTYTYPE::RealType &propertyValue) {
    if (getDataLocation() == NODE) {
      getProperty<PROPERTY>(propertyName)->setAllNodeValue(propertyValue);
    } else {
      getProperty<PROPERTY>(propertyName)->setAllEdgeValue(propertyValue);
    }
  }

  template <typename PROPERTY, typename PROPERTYTYPE>
  typename PROPERTYTYPE::RealType getPropertyMinValue(const std::string &propertyName) {
    if (getDataLocation() == NODE) {
      return getProperty<PROPERTY>(propertyName)->getNodeMin(graph_component);
    } else {
      return getProperty<PROPERTY>(propertyName)->getEdgeMin(graph_component);
    }
  }

  template <typename PROPERTY, typename PROPERTYTYPE>
  typename PROPERTYTYPE::RealType getPropertyMaxValue(const std::string &propertyName) {
    if (getDataLocation() == NODE) {
      return getProperty<PROPERTY>(propertyName)->getNodeMax(graph_component);
    } else {
      return getProperty<PROPERTY>(propertyName)->getEdgeMax(graph_component);
    }
  }

private:
  Color getOriginalDataColor(const unsigned int dataId);

  bool graphColorsChanged;
  ColorProperty *dataColors;
  ColorProperty *originalDataColors;
  std::set<unsigned int> highlightedElts;
  std::vector<std::string> selectedProperties;
  ElementType dataLocation;
  unsigned int unhighlightedEltsColorAlphaValue;
};

}

#endif // PARALLEL_COORDINATES_GRAPH_PROXY_H
