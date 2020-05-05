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

#ifndef TALIPOT_BASIC_ITERATORS_H
#define TALIPOT_BASIC_ITERATORS_H

#include <talipot/Iterator.h>
#include <talipot/MemoryPool.h>
#include <talipot/MutableContainer.h>

namespace tlp {

class Graph;
class GraphImpl;
class GraphView;

struct node;
struct edge;
class TLP_SCOPE NodeIterator : public Iterator<node> {};

class TLP_SCOPE EdgeIterator : public Iterator<edge> {};

//===========================================================
/// Factorization of code for iterators
class TLP_SCOPE FactorNodeIterator : public NodeIterator {
protected:
  Graph *_parentGraph;
  void *ito;
  void enableListening(const Graph *sg);
  void disableListening(const Graph *sg);

public:
  FactorNodeIterator(const Graph *sg) : _parentGraph(sg->getSuperGraph()), ito(nullptr) {}
};

class TLP_SCOPE FactorEdgeIterator : public EdgeIterator {
protected:
  const Graph *_parentGraph;
  void *ito;
  void enableListening(const Graph *sg);
  void disableListening(const Graph *sg);

public:
  FactorEdgeIterator(const Graph *sg) : _parentGraph(sg->getSuperGraph()), ito(nullptr) {}
};
//============================================================
/// Subgraph node iterator
//============================================================
/// Node iterator for GraphView
template <typename VALUE_TYPE>
class SGraphNodeIterator : public FactorNodeIterator,
                           public MemoryPool<SGraphNodeIterator<VALUE_TYPE>> {
private:
  const Graph *sg;
  Iterator<node> *it;
  node curNode;
  VALUE_TYPE value;
  const MutableContainer<VALUE_TYPE> &_filter;

protected:
  void prepareNext() {
    while (it->hasNext()) {
      curNode = it->next();

      if (_filter.get(curNode) == value) {
        return;
      }
    }

    // set curNode as invalid
    curNode = node();
  }

public:
  SGraphNodeIterator(const Graph *sG, const MutableContainer<VALUE_TYPE> &filter,
                     typename tlp::StoredType<VALUE_TYPE>::ReturnedConstValue val)
      : FactorNodeIterator(sG), sg(sG), value(val), _filter(filter) {
    enableListening(sg);
    it = sg->getNodes();
    // anticipate first iteration
    prepareNext();
  }

  ~SGraphNodeIterator() override {
    disableListening(sg);
    delete it;
  }

  node next() override {
    assert(curNode.isValid());
    // we are already pointing to the next
    node tmp = curNode;
    // anticipate next iteration
    prepareNext();
    return tmp;
  }

  bool hasNext() override {
    return (curNode.isValid());
  }
};

//=============================================================
/// subgraph edges iterator
template <typename VALUE_TYPE>
class SGraphEdgeIterator : public FactorEdgeIterator,
                           public MemoryPool<SGraphEdgeIterator<VALUE_TYPE>> {
private:
  const Graph *sg;
  Iterator<edge> *it;
  edge curEdge;
  VALUE_TYPE value;
  const MutableContainer<VALUE_TYPE> &_filter;

protected:
  void prepareNext() {
    while (it->hasNext()) {
      curEdge = it->next();

      if (_filter.get(curEdge.id) == value) {
        return;
      }
    }

    // set curEdge as invalid
    curEdge = edge();
  }

public:
  SGraphEdgeIterator(const Graph *sG, const MutableContainer<VALUE_TYPE> &filter,
                     typename tlp::StoredType<VALUE_TYPE>::ReturnedConstValue val)
      : FactorEdgeIterator(sG), sg(sG), value(val), _filter(filter) {
    it = sg->getEdges();
    // anticipate first iteration
    prepareNext();
  }

  ~SGraphEdgeIterator() override {
    delete it;
  }

  edge next() override {
    assert(curEdge.isValid());
    // we are already pointing to the next
    edge tmp = curEdge;
    // anticipating the next iteration
    prepareNext();
    return tmp;
  }

  bool hasNext() override {
    return (curEdge.isValid());
  }
};
}
#endif // TALIPOT_BASIC_ITERATORS_H
