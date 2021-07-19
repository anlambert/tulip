/**
 *
 * Copyright (C) 2021  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <memory>

#include <talipot/BooleanProperty.h>
#include <talipot/Color.h>
#include <talipot/ColorProperty.h>
#include <talipot/DoubleProperty.h>
#include <talipot/IntegerProperty.h>
#include <talipot/LayoutProperty.h>
#include <talipot/SizeProperty.h>
#include <talipot/StringProperty.h>

#include <talipot/PropertyProxy.h>

using namespace std;
using namespace tlp;

#define DEFINE_PP_METHODS(NODE_TYPE, EDGE_TYPE, PROP_TYPE)                                       \
  Iterator<node> *PropertyProxy::getNodesEqualTo(TYPE_CONST_REFERENCE(NODE_TYPE##Type) v,        \
                                                 const Graph *g) const {                         \
    return _graph->get##PROP_TYPE##Property(_propertyName)->getNodesEqualTo(v, g);               \
  }                                                                                              \
                                                                                                 \
  Iterator<edge> *PropertyProxy::getEdgesEqualTo(TYPE_CONST_REFERENCE(EDGE_TYPE##Type) v,        \
                                                 const Graph *g) const {                         \
    return _graph->get##PROP_TYPE##Property(_propertyName)->getEdgesEqualTo(v, g);               \
  }                                                                                              \
                                                                                                 \
  void PropertyProxy::setAllNodeValue(TYPE_CONST_REFERENCE(NODE_TYPE##Type) v, const Graph *g) { \
    _graph->get##PROP_TYPE##Property(_propertyName)->setAllNodeValue(v, g);                      \
  }                                                                                              \
                                                                                                 \
  void PropertyProxy::setAllEdgeValue(TYPE_CONST_REFERENCE(EDGE_TYPE##Type) v, const Graph *g) { \
    _graph->get##PROP_TYPE##Property(_propertyName)->setAllEdgeValue(v, g);                      \
  }                                                                                              \
                                                                                                 \
  void PropertyProxy::setNodeDefaultValue(TYPE_CONST_REFERENCE(NODE_TYPE##Type) v) {             \
    _graph->get##PROP_TYPE##Property(_propertyName)->setNodeDefaultValue(v);                     \
  }                                                                                              \
                                                                                                 \
  void PropertyProxy::setEdgeDefaultValue(TYPE_CONST_REFERENCE(EDGE_TYPE##Type) v) {             \
    _graph->get##PROP_TYPE##Property(_propertyName)->setEdgeDefaultValue(v);                     \
  }

DEFINE_PP_METHODS(Boolean, Boolean, Boolean)
DEFINE_PP_METHODS(BooleanVector, BooleanVector, BooleanVector)
DEFINE_PP_METHODS(Color, Color, Color)
DEFINE_PP_METHODS(ColorVector, ColorVector, ColorVector)
DEFINE_PP_METHODS(Double, Double, Double)
DEFINE_PP_METHODS(DoubleVector, DoubleVector, DoubleVector)
DEFINE_PP_METHODS(Integer, Integer, Integer)
DEFINE_PP_METHODS(IntegerVector, IntegerVector, IntegerVector)
DEFINE_PP_METHODS(Point, Line, Layout)
DEFINE_PP_METHODS(Size, Size, Size)
DEFINE_PP_METHODS(SizeVector, SizeVector, SizeVector)
DEFINE_PP_METHODS(String, String, String)
DEFINE_PP_METHODS(StringVector, StringVector, StringVector)

Iterator<node> *PropertyProxy::getNodesEqualTo(const char *v, const Graph *g) const {
  return _graph->getStringProperty(_propertyName)->getNodesEqualTo(v, g);
}

Iterator<edge> *PropertyProxy::getEdgesEqualTo(const char *v, const Graph *g) const {
  return _graph->getStringProperty(_propertyName)->getEdgesEqualTo(v, g);
}

void PropertyProxy::setAllNodeValue(const char *v, const Graph *g) {
  _graph->getStringProperty(_propertyName)->setAllNodeValue(v, g);
}

void PropertyProxy::setAllEdgeValue(const char *v, const Graph *g) {
  _graph->getStringProperty(_propertyName)->setAllEdgeValue(v, g);
}

void PropertyProxy::setNodeDefaultValue(const char *v) {
  _graph->getStringProperty(_propertyName)->setNodeDefaultValue(v);
}

void PropertyProxy::setEdgeDefaultValue(const char *v) {
  _graph->getStringProperty(_propertyName)->setEdgeDefaultValue(v);
}

Iterator<node> *PropertyProxy::getNonDefaultValuatedNodes(const Graph *g) const {
  auto *prop = _graph->getProperty(_propertyName);
  assert(prop);
  return prop->getNonDefaultValuatedNodes(g);
}

bool PropertyProxy::hasNonDefaultValuatedNodes(const Graph *g) const {
  auto *prop = _graph->getProperty(_propertyName);
  assert(prop);
  return prop->hasNonDefaultValuatedNodes(g);
}

unsigned int PropertyProxy::numberOfNonDefaultValuatedNodes(const Graph *g) const {
  auto *prop = _graph->getProperty(_propertyName);
  assert(prop);
  return prop->numberOfNonDefaultValuatedNodes(g);
}

Iterator<edge> *PropertyProxy::getNonDefaultValuatedEdges(const Graph *g) const {
  auto *prop = _graph->getProperty(_propertyName);
  assert(prop);
  return prop->getNonDefaultValuatedEdges(g);
}

bool PropertyProxy::hasNonDefaultValuatedEdges(const Graph *g) const {
  auto *prop = _graph->getProperty(_propertyName);
  assert(prop);
  return prop->hasNonDefaultValuatedEdges(g);
}

unsigned int PropertyProxy::numberOfNonDefaultValuatedEdges(const Graph *g) const {
  auto *prop = _graph->getProperty(_propertyName);
  assert(prop);
  return prop->numberOfNonDefaultValuatedEdges(g);
}

#define DEFINE_PVW_SET_METHOD(PROP_TYPE)                                                      \
  PropertyValueWrapper &PropertyValueWrapper::operator=(TYPE_CONST_REFERENCE(PROP_TYPE##Type) \
                                                            val) {                            \
    auto *prop = _graph->get##PROP_TYPE##Property(_propertyName);                             \
    if (_n.isValid()) {                                                                       \
      prop->setNodeValue(_n, val);                                                            \
    } else if (_e.isValid()) {                                                                \
      prop->setEdgeValue(_e, val);                                                            \
    }                                                                                         \
    return *this;                                                                             \
  }

#define DEFINE_PVW_GET_METHOD(PROP_TYPE)                                         \
  PropertyValueWrapper::operator TYPE_CONST_REFERENCE(PROP_TYPE##Type)() const { \
    auto *prop = _graph->get##PROP_TYPE##Property(_propertyName);                \
    if (_n.isValid()) {                                                          \
      return prop->getNodeValue(_n);                                             \
    } else {                                                                     \
      return prop->getEdgeValue(_e);                                             \
    }                                                                            \
  }

#define DEFINE_PVW_METHODS(PROP_TYPE) \
  DEFINE_PVW_SET_METHOD(PROP_TYPE)    \
  DEFINE_PVW_GET_METHOD(PROP_TYPE)

DEFINE_PVW_METHODS(Boolean)
DEFINE_PVW_METHODS(BooleanVector)
DEFINE_PVW_METHODS(Color)
DEFINE_PVW_METHODS(ColorVector)
DEFINE_PVW_METHODS(Double)
DEFINE_PVW_METHODS(DoubleVector)
DEFINE_PVW_METHODS(IntegerVector)
DEFINE_PVW_METHODS(Size)
DEFINE_PVW_METHODS(SizeVector)
DEFINE_PVW_METHODS(String)
DEFINE_PVW_METHODS(StringVector)

DEFINE_PVW_GET_METHOD(Integer)

PropertyValueWrapper &PropertyValueWrapper::operator=(TYPE_CONST_REFERENCE(IntegerType) val) {
  if (_n.isValid()) {
    if (_graph->existProperty(_propertyName) &&
        _graph->getProperty(_propertyName)->getTypename() == DoubleProperty::propertyTypename) {
      _graph->getDoubleProperty(_propertyName)->setNodeValue(_n, val);
    } else {
      _graph->getIntegerProperty(_propertyName)->setNodeValue(_n, val);
    }
  } else if (_e.isValid()) {
    if (_graph->existProperty(_propertyName) &&
        _graph->getProperty(_propertyName)->getTypename() == DoubleProperty::propertyTypename) {
      _graph->getDoubleProperty(_propertyName)->setEdgeValue(_e, val);
    } else {
      _graph->getIntegerProperty(_propertyName)->setEdgeValue(_e, val);
    }
  }
  return *this;
}

PropertyValueWrapper &PropertyValueWrapper::operator=(TYPE_CONST_REFERENCE(LineType) val) {
  if (_n.isValid()) {
    _graph->getCoordVectorProperty(_propertyName)->setNodeValue(_n, val);
  } else if (_e.isValid()) {
    if (_graph->existProperty(_propertyName) &&
        _graph->getProperty(_propertyName)->getTypename() == LayoutProperty::propertyTypename) {
      _graph->getLayoutProperty(_propertyName)->setEdgeValue(_e, val);
    } else {
      _graph->getCoordVectorProperty(_propertyName)->setEdgeValue(_e, val);
    }
  }
  return *this;
}

PropertyValueWrapper::operator TYPE_CONST_REFERENCE(LineType)() const {
  if (_n.isValid()) {
    return _graph->getCoordVectorProperty(_propertyName)->getNodeValue(_n);
  } else {
    if (_graph->existProperty(_propertyName) &&
        _graph->getProperty(_propertyName)->getTypename() == LayoutProperty::propertyTypename) {
      return _graph->getLayoutProperty(_propertyName)->getEdgeValue(_e);
    } else {
      return _graph->getCoordVectorProperty(_propertyName)->getEdgeValue(_e);
    }
  }
}

PropertyValueWrapper &PropertyValueWrapper::operator=(TYPE_CONST_REFERENCE(PointType) val) {
  auto *prop = _graph->getLayoutProperty(_propertyName);
  if (_n.isValid()) {
    prop->setNodeValue(_n, val);
  }
  return *this;
}

PropertyValueWrapper::operator TYPE_CONST_REFERENCE(PointType)() const {
  auto *prop = _graph->getLayoutProperty(_propertyName);
  if (_n.isValid()) {
    return prop->getNodeValue(_n);
  } else {
    static REAL_TYPE(PointType) p;
    return p;
  }
}

PropertyValueWrapper &PropertyValueWrapper::operator=(const char *val) {
  return *this = string(val);
}

static bool checkValidProperties(PropertyInterface *propTgt, PropertyInterface *propSrc,
                                 const string &errMsgPrefix) {
  if (!propTgt) {
    tlp::error() << errMsgPrefix << ": target property does not exit." << endl;
    return false;
  }
  if (!propSrc) {
    tlp::error() << errMsgPrefix << ": source property does not exit." << endl;
    return false;
  }
  if (propTgt->getTypename() != propSrc->getTypename()) {
    tlp::error() << errMsgPrefix << ": source and target properties are of different types."
                 << endl;
    return false;
  }
  return true;
}

static inline auto getNodeDataMemValue(PropertyInterface *prop, node n) {
  return unique_ptr<DataMem>(prop->getNodeDataMemValue(n));
}

static inline auto getEdgeDataMemValue(PropertyInterface *prop, edge e) {
  return unique_ptr<DataMem>(prop->getEdgeDataMemValue(e));
}

PropertyValueWrapper &PropertyValueWrapper::operator=(const PropertyValueWrapper &pvw) {
  auto *propTgt = _graph->getProperty(_propertyName);
  auto *propSrc = pvw._graph->getProperty(pvw._propertyName);

  if (checkValidProperties(propTgt, propSrc, __PRETTY_FUNCTION__)) {
    if (_n.isValid() && pvw._n.isValid()) {
      propTgt->setNodeDataMemValue(_n, getNodeDataMemValue(propSrc, pvw._n).get());
    } else if (_e.isValid() && pvw._e.isValid()) {
      propTgt->setEdgeDataMemValue(_e, getEdgeDataMemValue(propSrc, pvw._e).get());
    } else if (_n.isValid() && pvw._e.isValid()) {
      propTgt->setNodeDataMemValue(_n, getEdgeDataMemValue(propSrc, pvw._e).get());
    } else {
      propTgt->setEdgeDataMemValue(_e, getNodeDataMemValue(propSrc, pvw._n).get());
    }
  }

  return *this;
}

#define CHECK_VALID_PROPERTIES()                                                                \
  if (!checkValidProperties(_graph->getProperty(_propertyName),                                 \
                            pvw._graph->getProperty(pvw._propertyName), __PRETTY_FUNCTION__)) { \
    return false;                                                                               \
  }

bool PropertyValueWrapper::operator==(const PropertyValueWrapper &pvw) const {
  CHECK_VALID_PROPERTIES()
  return getValue() == pvw.getValue();
}

bool PropertyValueWrapper::operator!=(const PropertyValueWrapper &pvw) const {
  CHECK_VALID_PROPERTIES()
  return getValue() != pvw.getValue();
}

bool PropertyValueWrapper::operator<(const PropertyValueWrapper &pvw) const {
  CHECK_VALID_PROPERTIES()
  return getValue() < pvw.getValue();
}

bool PropertyValueWrapper::operator<=(const PropertyValueWrapper &pvw) const {
  CHECK_VALID_PROPERTIES()
  return getValue() <= pvw.getValue();
}

bool PropertyValueWrapper::operator>(const PropertyValueWrapper &pvw) const {
  CHECK_VALID_PROPERTIES()
  return getValue() > pvw.getValue();
}

bool PropertyValueWrapper::operator>=(const PropertyValueWrapper &pvw) const {
  CHECK_VALID_PROPERTIES()
  return getValue() >= pvw.getValue();
}

template <class TypedProperty>
void setValue(TypedProperty *prop, WrappedValue &val, node n, edge e) {
  if (n.isValid()) {
    val = prop->getNodeValue(n);
  } else {
    val = prop->getEdgeValue(e);
  }
}

WrappedValue PropertyValueWrapper::getValue() const {
  WrappedValue ret;
  string propertyType = _graph->getProperty(_propertyName)->getTypename();

  if (propertyType == BooleanProperty::propertyTypename) {
    setValue(_graph->getBooleanProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == ColorProperty::propertyTypename) {
    setValue(_graph->getColorProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == DoubleProperty::propertyTypename) {
    setValue(_graph->getDoubleProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == IntegerProperty::propertyTypename) {
    setValue(_graph->getIntegerProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == LayoutProperty::propertyTypename) {
    setValue(_graph->getLayoutProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == SizeProperty::propertyTypename) {
    setValue(_graph->getSizeProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == StringProperty::propertyTypename) {
    setValue(_graph->getStringProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == BooleanVectorProperty::propertyTypename) {
    setValue(_graph->getBooleanVectorProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == ColorVectorProperty::propertyTypename) {
    setValue(_graph->getColorVectorProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == DoubleVectorProperty::propertyTypename) {
    setValue(_graph->getDoubleVectorProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == IntegerVectorProperty::propertyTypename) {
    setValue(_graph->getIntegerVectorProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == SizeVectorProperty::propertyTypename) {
    setValue(_graph->getSizeVectorProperty(_propertyName), ret, _n, _e);
  } else if (propertyType == StringVectorProperty::propertyTypename) {
    setValue(_graph->getStringVectorProperty(_propertyName), ret, _n, _e);
  }
  return ret;
}