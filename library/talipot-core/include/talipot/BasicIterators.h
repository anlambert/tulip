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

//============================================================
/// Subgraph node iterator
//============================================================
/// Node iterator for GraphView
template <typename VALUE_TYPE>
class SGraphNodeIterator : public NodeIterator, public MemoryPool<SGraphNodeIterator<VALUE_TYPE>> {
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
  SGraphNodeIterator(const Graph *sg, const MutableContainer<VALUE_TYPE> &filter,
                     typename tlp::StoredType<VALUE_TYPE>::ReturnedConstValue val)
      : sg(sg), value(val), _filter(filter) {
    it = sg->getNodes();
    // prepare first iteration
    prepareNext();
  }

  ~SGraphNodeIterator() override {
    delete it;
  }

  node next() override {
    assert(curNode.isValid());
    // we are already pointing to the next node
    node tmp = curNode;
    // prepare next iteration
    prepareNext();
    return tmp;
  }

  bool hasNext() override {
    return curNode.isValid();
  }
};

//=============================================================
/// subgraph edges iterator
template <typename VALUE_TYPE>
class SGraphEdgeIterator : public EdgeIterator, public MemoryPool<SGraphEdgeIterator<VALUE_TYPE>> {
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
  SGraphEdgeIterator(const Graph *sg, const MutableContainer<VALUE_TYPE> &filter,
                     typename tlp::StoredType<VALUE_TYPE>::ReturnedConstValue val)
      : sg(sg), value(val), _filter(filter) {
    it = sg->getEdges();
    // prepare first iteration
    prepareNext();
  }

  ~SGraphEdgeIterator() override {
    delete it;
  }

  edge next() override {
    assert(curEdge.isValid());
    // we are already pointing to the next edge
    edge tmp = curEdge;
    // prepare next iteration
    prepareNext();
    return tmp;
  }

  bool hasNext() override {
    return curEdge.isValid();
  }
};
}
#endif // TALIPOT_BASIC_ITERATORS_H
