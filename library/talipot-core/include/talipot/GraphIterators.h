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
class TLP_SCOPE OutNodesIterator : public FactorNodeIterator, public MemoryPool<OutNodesIterator> {
private:
  Iterator<edge> *it;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  OutNodesIterator(const GraphView *sG, node n);
  ~OutNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//============================================================
/// In node iterator for GraphView
class InNodesIterator : public FactorNodeIterator, public MemoryPool<InNodesIterator> {
private:
  Iterator<edge> *it;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  InNodesIterator(const GraphView *sG, node n);
  ~InNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//============================================================
/// In Out node iterator for GraphView
class TLP_SCOPE InOutNodesIterator : public FactorNodeIterator,
                                     public MemoryPool<InOutNodesIterator> {
private:
  Iterator<edge> *it;
  node n;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  InOutNodesIterator(const GraphView *sG, node n);
  ~InOutNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//============================================================
/// Out edge iterator for GraphView
class TLP_SCOPE OutEdgesIterator : public FactorEdgeIterator, public MemoryPool<OutEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView *sg;

public:
  OutEdgesIterator(const GraphView *sG, node n);
  ~OutEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};
//============================================================
/// In edge iterator for GraphView
class TLP_SCOPE InEdgesIterator : public FactorEdgeIterator, public MemoryPool<InEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView *sg;

public:
  InEdgesIterator(const GraphView *sG, node n);
  ~InEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};
//============================================================
/// In Out edge iterator for GraphView
class TLP_SCOPE InOutEdgesIterator : public FactorEdgeIterator,
                                     public MemoryPool<InOutEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView *sg;

public:
  InOutEdgesIterator(const GraphView *sG, node n);
  ~InOutEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};

//============================================================
// Iterator for the Graph
//============================================================
class TLP_SCOPE GraphNodeIterator : public NodeIterator
#if defined(NDEBUG) || defined(_OPENMP)
    ,
                                    public MemoryPool<GraphNodeIterator>
#endif
{
private:
#if !defined(NDEBUG) && !defined(_OPENMP)
  const Graph *graph;
  void *ito;
#endif
  Iterator<node> *itId;

public:
  GraphNodeIterator(const Graph *g, Iterator<node> *it);
  ~GraphNodeIterator() override;
  node next() override;
  bool hasNext() override;
};
//=============================================================
/// Edge iterator for data sg
class TLP_SCOPE GraphEdgeIterator : public EdgeIterator
#if defined(NDEBUG) || defined(_OPENMP)
    ,
                                    public MemoryPool<GraphEdgeIterator>
#endif
{
private:
#if !defined(NDEBUG) && !defined(_OPENMP)
  const Graph *graph;
  void *ito;
#endif
  Iterator<edge> *itId;

public:
  GraphEdgeIterator(const Graph *g, Iterator<edge> *it);
  ~GraphEdgeIterator() override;
  edge next() override;
  bool hasNext() override;
};
//============================================================
}
#endif // TALIPOT_GRAPH_ITERATORS_H
