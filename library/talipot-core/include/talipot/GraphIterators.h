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

#ifndef TALIPOT_GRAPH_ITERATORS_H
#define TALIPOT_GRAPH_ITERATORS_H

#include <talipot/BasicIterators.h>
#include <talipot/config.h>

namespace tlp {

class Graph;
class GraphImpl;
class GraphView;

//============================================================
/// Out node iterator for GraphView
class TLP_SCOPE OutNodesIterator : public NodeIterator, public MemoryPool<OutNodesIterator> {
private:
  const GraphView *sg;
  Iterator<edge> *it;

public:
  OutNodesIterator(const GraphView *sg, node n);
  ~OutNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//============================================================
/// In node iterator for GraphView
class InNodesIterator : public NodeIterator, public MemoryPool<InNodesIterator> {
private:
  const GraphView *sg;
  Iterator<edge> *it;

public:
  InNodesIterator(const GraphView *sg, node n);
  ~InNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//============================================================
/// In Out node iterator for GraphView
class TLP_SCOPE InOutNodesIterator : public NodeIterator, public MemoryPool<InOutNodesIterator> {
private:
  const GraphView *sg;
  Iterator<edge> *it;
  node n;

public:
  InOutNodesIterator(const GraphView *sg, node n);
  ~InOutNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//============================================================
/// Out edge iterator for GraphView
class TLP_SCOPE OutEdgesIterator : public EdgeIterator, public MemoryPool<OutEdgesIterator> {
private:
  const GraphView *sg;
  Iterator<edge> *it;
  edge curEdge;

public:
  OutEdgesIterator(const GraphView *sg, node n);
  ~OutEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};
//============================================================
/// In edge iterator for GraphView
class TLP_SCOPE InEdgesIterator : public EdgeIterator, public MemoryPool<InEdgesIterator> {
private:
  const GraphView *sg;
  Iterator<edge> *it;
  edge curEdge;

public:
  InEdgesIterator(const GraphView *sg, node n);
  ~InEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};
//============================================================
/// In Out edge iterator for GraphView
class TLP_SCOPE InOutEdgesIterator : public EdgeIterator, public MemoryPool<InOutEdgesIterator> {
private:
  const GraphView *sg;
  Iterator<edge> *it;
  edge curEdge;

public:
  InOutEdgesIterator(const GraphView *sg, node n);
  ~InOutEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};
}
#endif // TALIPOT_GRAPH_ITERATORS_H
