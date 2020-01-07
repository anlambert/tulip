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

#ifndef TALIPOT_DOUBLE_PROPERTY_H
#define TALIPOT_DOUBLE_PROPERTY_H

#include <talipot/PropertyTypes.h>
#include <talipot/AbstractProperty.h>
#include <talipot/PropertyAlgorithm.h>
#include <talipot/MinMaxProperty.h>
#include <talipot/NumericProperty.h>

namespace tlp {

class PropertyContext;

typedef MinMaxProperty<tlp::DoubleType, tlp::DoubleType, tlp::NumericProperty> DoubleMinMaxProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps a double value to graph elements.
 */
class TLP_SCOPE DoubleProperty : public DoubleMinMaxProperty {
public:
  DoubleProperty(Graph *, const std::string &n = "");

  void clone_handler(
      AbstractProperty<tlp::DoubleType, tlp::DoubleType, tlp::NumericProperty> &) override;

  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }

  void setNodeValue(const node n, tlp::StoredType<double>::ReturnedConstValue v) override;
  void setEdgeValue(const edge e, tlp::StoredType<double>::ReturnedConstValue v) override;
  void setAllNodeValue(tlp::StoredType<double>::ReturnedConstValue v,
                       const Graph *graph = nullptr) override;
  void setAllEdgeValue(tlp::StoredType<double>::ReturnedConstValue v,
                       const Graph *graph = nullptr) override;

  enum PredefinedMetaValueCalculator {
    NO_CALC = 0,
    AVG_CALC = 1,
    SUM_CALC = 2,
    MAX_CALC = 3,
    MIN_CALC = 4
  };

  // setMetaValueCalculator overloading
  void setMetaValueCalculator(PropertyInterface::MetaValueCalculator *calc) override;
  void setMetaValueCalculator(PredefinedMetaValueCalculator nodeCalc = AVG_CALC,
                              PredefinedMetaValueCalculator edgeCalc = AVG_CALC);

  // NumericProperty interface
  double getNodeDoubleValue(const node n) const override {
    return getNodeValue(n);
  }
  double getNodeDoubleDefaultValue() const override {
    return getNodeDefaultValue();
  }
  double getNodeDoubleMin(const Graph *g = nullptr) override {
    return getNodeMin(g);
  }
  double getNodeDoubleMax(const Graph *g = nullptr) override {
    return getNodeMax(g);
  }
  double getEdgeDoubleValue(const edge e) const override {
    return getEdgeValue(e);
  }
  double getEdgeDoubleDefaultValue() const override {
    return getEdgeDefaultValue();
  }
  double getEdgeDoubleMin(const Graph *g = nullptr) override {
    return getEdgeMin(g);
  }
  double getEdgeDoubleMax(const Graph *g = nullptr) override {
    return getEdgeMax(g);
  }

  void nodesUniformQuantification(unsigned int) override;

  void edgesUniformQuantification(unsigned int) override;

  NumericProperty *copyProperty(Graph *g) override {
    DoubleProperty *newProp = new DoubleProperty(g);
    newProp->copy(this);

    return newProp;
  }

private:
  // override Observable::treatEvent
  void treatEvent(const Event &) override;
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<double> value to graph elements.
 */

class TLP_SCOPE DoubleVectorProperty
    : public AbstractVectorProperty<tlp::DoubleVectorType, tlp::DoubleType> {
public:
  DoubleVectorProperty(Graph *g, const std::string &n = "")
      : AbstractVectorProperty<DoubleVectorType, tlp::DoubleType>(g, n) {}
  // redefinition of some PropertyInterface methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }
};
#ifdef _MSC_VER
template class AbstractVectorProperty<DoubleVectorType, DoubleType>;
#endif
}
#endif // TALIPOT_DOUBLE_PROPERTY_H
