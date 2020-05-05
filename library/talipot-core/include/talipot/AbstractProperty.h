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

#ifndef TALIPOT_ABSTRACT_PROPERTY_H
#define TALIPOT_ABSTRACT_PROPERTY_H

#include <typeinfo>
#include <string>
#include <cstdlib>
#include <talipot/config.h>
#include <talipot/StoredType.h>
#include <talipot/MutableContainer.h>
#include <talipot/PropertyInterface.h>
#include <talipot/Iterator.h>
#include <talipot/DataSet.h>
#include <talipot/Graph.h>

namespace tlp {

class GraphView;

//==============================================================

/**
 * @ingroup Graph
 * @brief This class extends upon PropertyInterface, and adds type-safe methods to
 * get and set the node and edge values, through the magic of template programming.
 *
 * Nodes and Edges can have different types (e.g. tlp::LayoutProperty has tlp::PointType as node
 * type and tlp::LineType as edge type),
 * but most of the time they have the same type (e.g. tlp::DoubleProperty, tlp::IntegerProperty).
 *
 * Some of the pure virtual functions of PropertyInterface are implemented in this class (e.g.
 * erase()).
 *
 * The actual data is stored in this class, and it manages the default values.
 */
template <class Tnode, class Tedge, class Tprop = PropertyInterface>
class TLP_SCOPE AbstractProperty : public Tprop {
  friend class Graph;
  friend class GraphView;

public:
  AbstractProperty(Graph *, const std::string &n = "");

  /**
   * @brief Gets the default node value of the property.
   * @return The default value of nodes.
   */
  typename Tnode::RealType getNodeDefaultValue() const;

  /**
   * @brief Gets the default edge value of the property.
   * @return The default value of edges.
   **/
  typename Tedge::RealType getEdgeDefaultValue() const;

  /**
   * @brief Returns the value associated with the node n in this property.
   * If there is no value, it returns the default node value.
   *
   * @param n The node for which we want to get the value of the property.
   * @return :StoredType< Tnode::RealType >::ReturnedConstValue The value of the property for this
   *node.
   **/
  typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue
  getNodeValue(const node n) const;

  /**
   * @brief Returns the value associated to the edge e in this property.
   * If there is no value, it returns the default edge value.
   *
   * @param e The edge for which we want to get the value of the property.
   * @return :StoredType< Tedge::RealType >::ReturnedConstValue The value of the property for this
   *edge.
   **/
  typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue
  getEdgeValue(const edge e) const;

  /**
   * Returns an iterator through all nodes belonging to g
   * whose associated value is equal to val.
   * If g is nullptr, the graph given when creating the property is considered.
   */
  virtual tlp::Iterator<node> *
  getNodesEqualTo(typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue v,
                  const Graph *g = nullptr) const;

  /**
   * Returns an iterator through all edges belonging to g
   * whose associated value is equal to val.
   * If g is nullptr, the graph given when creating the property is considered.
   */
  virtual tlp::Iterator<edge> *
  getEdgesEqualTo(typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue v,
                  const Graph *g = nullptr) const;

  /**
   * @brief Sets the value of a node and notify the observers of a modification.
   *
   * @param n The node to set the value of.
   * @param v The value to affect for this node.
   **/
  virtual void
  setNodeValue(const node n,
               typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue v);

  /**
   * @brief Set the value of an edge and notify the observers of a modification.
   *
   * @param e The edge to set the value of.
   * @param v The value to affect for this edge.
   **/
  virtual void
  setEdgeValue(const edge e,
               typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue v);

  /**
   * @brief Sets the value of all nodes and notify the observers.
   *
   * All previous values are lost and the given value is assigned
   * as the default one to the future added nodes.
   *
   * @param v The value to set to all nodes.
   * @param graph An optional descendant graph from the one associated
   * to that property (itself included). If provided, only the nodes from
   * that graph will have their value modified and the default node value
   * will not be modified.
   *
   * @warning If the provided graph is not a descendant of the one associated
   * to that property, no node value will be modified in it.
   *
   */
  virtual void
  setAllNodeValue(typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue v,
                  const Graph *graph = nullptr);

  /**
   * @brief Sets the value assigned as the default one to the future added nodes.
   *
   * @param v the new value to set on future added nodes.
   *
   * @return Whether the given string was a correct representation for this property's type. If not,
   * the default value is not set.
   */
  virtual void
  setNodeDefaultValue(typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue v);

  /**
   * @brief Sets the value assigned as the default one to the future added edges.
   *
   * @param value the new value to set on future added edges.
   *
   * @return Whether the given string was a correct representation for this property's type. If not,
   * the default value is not set.
   */
  virtual void
  setEdgeDefaultValue(typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue v);

  /**
   * @brief Sets the value of all edges and notify the observers.
   *
   * All previous values are lost and the given value is assigned
   * as the default one to the future added edges.
   *
   * @param v The value to set to all edges.
   * @param graph An optional descendant graph from the one associated
   * to that property (itself included). If provided, only the edges
   * from that graph will have their value modified and the default edge
   * value will not be modified.
   *
   * @warning If the provided graph is not a descendant of the one associated
   * to that property, no edge value will be modified in it.
   *
   */
  virtual void
  setAllEdgeValue(typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue v,
                  const Graph *graph = nullptr);

  //=================================================================================

  /**
   * @brief Resets the value of a node to the default value.
   *
   * @param n The node to reset the value of.
   *
   **/
  void erase(const node n) override {
    setNodeValue(n, nodeDefaultValue);
  }
  //=================================================================================

  /**
   * @brief Resets the value of an edge to the default value.
   *
   * @param e The edge to reset the value of.
   *
   **/
  void erase(const edge e) override {
    setEdgeValue(e, edgeDefaultValue);
  }
  //=================================================================================
  /**
   * @brief This operator overload allows to copy a property using the following syntax :
   *
   * @code
   * IntegerProperty* shape = graph->getIntegerProperty("viewShape");
   * IntegerProperty* backup = graph->getIntegerProperty("shapeBackup");
   * *backup = *shape; // all the values from 'shape' will be copied into 'backup'.
   * @endcode
   * @param prop The property to copy the values from.
   * @return This property with the values copied.
   */
  virtual AbstractProperty<Tnode, Tedge, Tprop> &
  operator=(AbstractProperty<Tnode, Tedge, Tprop> &prop) {
    if (this != &prop) {
      if (Tprop::graph == nullptr) {
        Tprop::graph = prop.Tprop::graph;
      }

      if (Tprop::graph == prop.Tprop::graph) {
        setAllNodeValue(prop.getNodeDefaultValue());
        setAllEdgeValue(prop.getEdgeDefaultValue());

        for (auto itn : prop.getNonDefaultValuatedNodes()) {
          setNodeValue(itn, prop.getNodeValue(itn));
        }

        for (auto ite : prop.getNonDefaultValuatedEdges()) {
          setEdgeValue(ite, prop.getEdgeValue(ite));
        }

      } else {
        //==============================================================*
        for (auto n : Tprop::graph->nodes()) {
          if (prop.Tprop::graph->isElement(n)) {
            setNodeValue(n, prop.getNodeValue(n));
          }
        }

        for (auto e : Tprop::graph->edges()) {
          if (prop.Tprop::graph->isElement(e)) {
            setEdgeValue(e, prop.getEdgeValue(e));
          }
        }
      }

      clone_handler(prop);
    }

    return *this;
  }
  //=================================================================================
  // Untyped accessors inherited from PropertyInterface, documentation is inherited
  std::string getNodeDefaultStringValue() const override {
    typename Tnode::RealType v = getNodeDefaultValue();
    return Tnode::toString(v);
  }
  std::string getEdgeDefaultStringValue() const override {
    typename Tedge::RealType v = getEdgeDefaultValue();
    return Tedge::toString(v);
  }
  std::string getNodeStringValue(const node n) const override {
    typename Tnode::RealType v = getNodeValue(n);
    return Tnode::toString(v);
  }
  std::string getEdgeStringValue(const edge e) const override {
    typename Tedge::RealType v = getEdgeValue(e);
    return Tedge::toString(v);
  }
  bool setNodeStringValue(const node inN, const std::string &inV) override {
    typename Tnode::RealType v;

    if (!Tnode::fromString(v, inV)) {
      return false;
    }

    setNodeValue(inN, v);
    return true;
  }
  bool setEdgeStringValue(const edge inE, const std::string &inV) override {
    typename Tedge::RealType v;

    if (!Tedge::fromString(v, inV)) {
      return false;
    }

    setEdgeValue(inE, v);
    return true;
  }
  bool setNodeDefaultStringValue(const std::string &inV) override {
    typename Tnode::RealType v;

    if (!Tnode::fromString(v, inV)) {
      return false;
    }

    setNodeDefaultValue(v);
    return true;
  }
  bool setAllNodeStringValue(const std::string &inV, const Graph *graph = nullptr) override {
    typename Tnode::RealType v;

    if (!Tnode::fromString(v, inV)) {
      return false;
    }

    setAllNodeValue(v, graph);
    return true;
  }
  bool setEdgeDefaultStringValue(const std::string &inV) override {
    typename Tedge::RealType v;

    if (!Tedge::fromString(v, inV)) {
      return false;
    }

    setEdgeDefaultValue(v);
    return true;
  }
  bool setAllEdgeStringValue(const std::string &inV, const Graph *graph = nullptr) override {
    typename Tedge::RealType v;

    if (!Tedge::fromString(v, inV)) {
      return false;
    }

    setAllEdgeValue(v, graph);
    return true;
  }

  tlp::Iterator<node> *getNonDefaultValuatedNodes(const Graph *g = nullptr) const override;
  bool hasNonDefaultValuatedNodes(const Graph *g = nullptr) const override;
  unsigned int numberOfNonDefaultValuatedNodes(const Graph *g = nullptr) const override;
  unsigned int nodeValueSize() const override;
  void writeNodeDefaultValue(std::ostream &) const override;
  void writeNodeValue(std::ostream &, node) const override;
  bool readNodeDefaultValue(std::istream &) override;
  bool readNodeValue(std::istream &, node) override;
  tlp::Iterator<edge> *getNonDefaultValuatedEdges(const Graph *g = nullptr) const override;
  bool hasNonDefaultValuatedEdges(const Graph *g = nullptr) const override;
  unsigned int numberOfNonDefaultValuatedEdges(const Graph * = nullptr) const override;
  unsigned int edgeValueSize() const override;
  void writeEdgeDefaultValue(std::ostream &) const override;
  void writeEdgeValue(std::ostream &, edge) const override;
  bool readEdgeDefaultValue(std::istream &) override;
  bool readEdgeValue(std::istream &, edge) override;
  bool copy(const node destination, const node source, PropertyInterface *property,
            bool ifNotDefault = false) override {
    if (property == nullptr) {
      return false;
    }

    tlp::AbstractProperty<Tnode, Tedge, Tprop> *tp =
        dynamic_cast<tlp::AbstractProperty<Tnode, Tedge, Tprop> *>(property);
    assert(tp);
    bool notDefault;
    typename StoredType<typename Tnode::RealType>::ReturnedValue value =
        tp->nodeProperties.get(source.id, notDefault);

    if (ifNotDefault && !notDefault) {
      return false;
    }

    setNodeValue(destination, value);
    return true;
  }
  bool copy(const edge destination, const edge source, PropertyInterface *property,
            bool ifNotDefault = false) override {
    if (property == nullptr) {
      return false;
    }

    tlp::AbstractProperty<Tnode, Tedge, Tprop> *tp =
        dynamic_cast<tlp::AbstractProperty<Tnode, Tedge, Tprop> *>(property);
    assert(tp);
    bool notDefault;
    typename StoredType<typename Tedge::RealType>::ReturnedValue value =
        tp->edgeProperties.get(source.id, notDefault);

    if (ifNotDefault && !notDefault) {
      return false;
    }

    setEdgeValue(destination, value);
    return true;
  }
  void copy(PropertyInterface *property) override {
    tlp::AbstractProperty<Tnode, Tedge, Tprop> *prop =
        dynamic_cast<typename tlp::AbstractProperty<Tnode, Tedge, Tprop> *>(property);
    assert(prop != nullptr);
    *this = *prop;
  }
  // for performance reason and use in GraphUpdatesRecorder
  DataMem *getNodeDefaultDataMemValue() const override {
    return new TypedValueContainer<typename Tnode::RealType>(getNodeDefaultValue());
  }
  DataMem *getEdgeDefaultDataMemValue() const override {
    return new TypedValueContainer<typename Tedge::RealType>(getEdgeDefaultValue());
  }
  DataMem *getNodeDataMemValue(const node n) const override {
    return new TypedValueContainer<typename Tnode::RealType>(getNodeValue(n));
  }
  DataMem *getEdgeDataMemValue(const edge e) const override {
    return new TypedValueContainer<typename Tedge::RealType>(getEdgeValue(e));
  }
  DataMem *getNonDefaultDataMemValue(const node n) const override {
    bool notDefault;
    typename StoredType<typename Tnode::RealType>::ReturnedValue value =
        nodeProperties.get(n.id, notDefault);

    if (notDefault) {
      return new TypedValueContainer<typename Tnode::RealType>(value);
    }

    return nullptr;
  }
  DataMem *getNonDefaultDataMemValue(const edge e) const override {
    bool notDefault;
    typename StoredType<typename Tedge::RealType>::ReturnedValue value =
        edgeProperties.get(e.id, notDefault);

    if (notDefault) {
      return new TypedValueContainer<typename Tedge::RealType>(value);
    }

    return nullptr;
  }
  void setNodeDataMemValue(const node n, const DataMem *v) override {
    setNodeValue(n, static_cast<const TypedValueContainer<typename Tnode::RealType> *>(v)->value);
  }
  void setEdgeDataMemValue(const edge e, const DataMem *v) override {
    setEdgeValue(e, static_cast<const TypedValueContainer<typename Tedge::RealType> *>(v)->value);
  }
  void setAllNodeDataMemValue(const DataMem *v) override {
    setAllNodeValue(static_cast<const TypedValueContainer<typename Tnode::RealType> *>(v)->value);
  }
  void setAllEdgeDataMemValue(const DataMem *v) override {
    setAllEdgeValue(static_cast<const TypedValueContainer<typename Tedge::RealType> *>(v)->value);
  }

  // PropertyInterface methods
  // mN is the meta node, sg is the corresponding subgraph
  // and mg is the graph owning mN
  void computeMetaValue(node n, Graph *sg, Graph *mg) override {
    if (Tprop::metaValueCalculator) {
      static_cast<typename tlp::AbstractProperty<Tnode, Tedge, Tprop>::MetaValueCalculator *>(
          Tprop::metaValueCalculator)
          ->computeMetaValue(this, n, sg, mg);
    }
  }
  // mE is the meta edge, itE is an iterator on the underlying edges
  // mg is the graph owning mE
  void computeMetaValue(edge e, tlp::Iterator<edge> *itE, Graph *mg) override {
    if (Tprop::metaValueCalculator) {
      static_cast<typename tlp::AbstractProperty<Tnode, Tedge, Tprop>::MetaValueCalculator *>(
          Tprop::metaValueCalculator)
          ->computeMetaValue(this, e, itE, mg);
    } else {
      delete itE;
    }
  }
  void setMetaValueCalculator(PropertyInterface::MetaValueCalculator *mvCalc) override {
    if (mvCalc &&
        !dynamic_cast<typename tlp::AbstractProperty<Tnode, Tedge, Tprop>::MetaValueCalculator *>(
            mvCalc)) {
      tlp::warning()
          << "Warning : " << __PRETTY_FUNCTION__ << " ... invalid conversion of "
          << typeid(mvCalc).name() << "into "
          << typeid(typename tlp::AbstractProperty<Tnode, Tedge, Tprop>::MetaValueCalculator *)
                 .name()
          << std::endl;
      abort();
    }

    Tprop::metaValueCalculator = mvCalc;
  }

  int compare(const node n1, const node n2) const override;
  int compare(const edge e1, const edge e2) const override;

  /**
   * @brief This class is used to delegate the computation of the values associated to meta nodes or
   *edges.
   **/
  class MetaValueCalculator : public PropertyInterface::MetaValueCalculator {
  public:
    // computes the value of the meta node mN of the graph mg
    // for the property prop, according to the values associated
    // to the underlying nodes i.e the nodes of the subgraph sg.
    virtual void computeMetaValue(AbstractProperty<Tnode, Tedge, Tprop> *, node, Graph *, Graph *) {
    }
    // computes the value of the meta node mE of the graph mg
    // for the property prop, according to the values associated
    // to the underlying edges given by the iterator itE.
    // The method do not have to delete the iterator
    virtual void computeMetaValue(AbstractProperty<Tnode, Tedge, Tprop> *, edge,
                                  tlp::Iterator<edge> *itE, Graph *) {
      delete itE;
    }
  };

protected:
  //=================================================================================
  /// Enable to clone part of sub_class
  virtual void clone_handler(AbstractProperty<Tnode, Tedge, Tprop> &) {}

  MutableContainer<typename Tnode::RealType> nodeProperties;
  MutableContainer<typename Tedge::RealType> edgeProperties;
  typename Tnode::RealType nodeDefaultValue;
  typename Tedge::RealType edgeDefaultValue;
};

template <typename vectType, typename eltType, typename propType = VectorPropertyInterface>
class TLP_SCOPE AbstractVectorProperty : public AbstractProperty<vectType, vectType, propType> {
public:
  AbstractVectorProperty(Graph *, const std::string &name = "");

  // 5 methods inherited from VectorPropertyInterface
  bool tokenize(const std::string &str, std::vector<std::string> &vect, char openChar = '(',
                char sepChar = ',', char closeChar = ')') override;

  bool setNodeStringValueAsVector(const node n, const std::vector<std::string> &values) override;

  bool setNodeStringValueAsVector(const node, const std::string &, char, char, char) override;

  bool setEdgeStringValueAsVector(const edge e, const std::vector<std::string> &values) override;

  bool setEdgeStringValueAsVector(const edge, const std::string &, char, char, char) override;

  /**
   * @brief Sets the value for node n, at index i, to v, and notify the observers of a modification.
   *
   * @param n The node to set a value of.
   * @param i The index at which the value should be set.
   * @param v The value to set.
   *
   **/
  void setNodeEltValue(const node n, unsigned int i,
                       typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
  /**
   * @brief Gets the value associated to node n, at index i.
   *
   * @param n The node to set a value of.
   * @param i The index at which to set the value.
   * @return const eltType& The value at index i in the vector for node n.
   **/
  typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue
  getNodeEltValue(const node n, unsigned int i) const;
  /**
   * @brief Appends a new value at the end of the vector associated to node n, and notify the
   *observers of a modification.
   *
   * @param n The node to add a value to.
   * @param v The value to append at the end of the vector.
   *
   **/
  void
  pushBackNodeEltValue(const node n,
                       typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
  /**
   * @brief Removes the value at the end of the vector associated to node n, and notify the
   *observers of a modification.
   *
   * @param n The node to remove a value from.
   *
   **/
  void popBackNodeEltValue(const node n);
  /**
   * @brief Resizes the vector associated to node n, and notify the observers of a modification.
   *
   * @param n The node associated to the vector to resize.
   * @param size The new size of the vector.
   * @param elt The default value to set at indices where there was no value before. Defaults to
   *eltType().
   *
   **/
  void resizeNodeValue(const node n, size_t size,
                       typename eltType::RealType elt = eltType::defaultValue());
  /**
   * @brief Sets the value for edge e, at index i, to v, and notify the observers of a modification.
   *
   * @param e The edge to set the value of.
   * @param i The index at which the value should be set.
   * @param v The value to set.
   *
   **/
  void setEdgeEltValue(const edge e, unsigned int i,
                       typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
  /**
   * @brief Gets the value associated to edge e, at index i.
   *
   * @param e The edge to set a value of.
   * @param i The index at which to set the value.
   * @return const eltType& The value at index i in the vector for node n.
   **/
  typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue
  getEdgeEltValue(const edge n, unsigned int i) const;
  /**
   * @brief Appends a new value at the end of the vector associated to edge e, and notify the
   *observers of a modification.
   *
   * @param e The node to add a value to.
   * @param v The value to append at the end of the vector.
   *
   **/
  void
  pushBackEdgeEltValue(const edge e,
                       typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
  /**
   * @brief Removes the value at the end of the vector associated to edge e, and notify the
   *observers of a modification.
   *
   * @param e The edge to remove a value from.
   *
   **/
  void popBackEdgeEltValue(const edge e);
  /**
   * @brief Resizes the vector associated to edge e, and notify the observers of a modification.
   *
   * @param e The edge associated to the vector to resize.
   * @param size The new size of the vector.
   * @param elt The default value to set at indices where there was no value before. Defaults to
   *eltType().
   *
   **/
  void resizeEdgeValue(const edge e, size_t size,
                       typename eltType::RealType elt = eltType::defaultValue());
};
}
#if !defined(_MSC_VER) || defined(DLL_Talipot) // When using VC++, we only want to include this when
// we are in the Talipot dll. With any other compiler,
// include it all the time
#include "cxx/AbstractProperty.cxx"
#endif
#endif // TALIPOT_ABSTRACT_PROPERTY_H
