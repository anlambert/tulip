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

#include <cassert>
#include <iostream>

#include <talipot/Graph.h>
#include <talipot/GraphImpl.h>
#include <talipot/GraphView.h>
#include <talipot/GraphIterators.h>
#include <talipot/Observable.h>
#include <talipot/StoredType.h>

namespace tlp {

//===================================================================
OutNodesIterator::OutNodesIterator(const GraphView *sg, node n)
    : sg(sg), it(new OutEdgesIterator(sg, n)) {}

OutNodesIterator::~OutNodesIterator() {
  delete it;
}
node OutNodesIterator::next() {
#ifndef NDEBUG
  assert(it->hasNext());
  node tmp = sg->target(it->next());
  assert(sg->isElement(tmp));
  return tmp;
#endif
  return sg->target(it->next());
}
bool OutNodesIterator::hasNext() {
  return it->hasNext();
}
//===================================================================
InNodesIterator::InNodesIterator(const GraphView *sg, node n)
    : sg(sg), it(new InEdgesIterator(sg, n)) {}

InNodesIterator::~InNodesIterator() {
  delete it;
}
node InNodesIterator::next() {
#ifndef NDEBUG
  assert(it->hasNext());
  node tmp = sg->source(it->next());
  assert(sg->isElement(tmp));
  return tmp;
#endif
  return sg->source(it->next());
}
bool InNodesIterator::hasNext() {
  return it->hasNext();
}
//===================================================================
InOutNodesIterator::InOutNodesIterator(const GraphView *sg, node n)
    : sg(sg), it(new InOutEdgesIterator(sg, n)), n(n) {}

InOutNodesIterator::~InOutNodesIterator() {
  delete it;
}
node InOutNodesIterator::next() {
#ifndef NDEBUG
  assert(it->hasNext());
  node tmp = sg->opposite(it->next(), n);
  assert(sg->isElement(tmp));
  return tmp;
#else
  return sg->opposite(it->next(), n);
#endif
}
bool InOutNodesIterator::hasNext() {
  return (it->hasNext());
}
//===================================================================
OutEdgesIterator::OutEdgesIterator(const GraphView *sg, node n) : sg(sg) {
  assert(sg->isElement(n));
  it = sg->getRoot()->getOutEdges(n);
  // prepare first iteration
  prepareNext();
}
OutEdgesIterator::~OutEdgesIterator() {
  delete it;
}
void OutEdgesIterator::prepareNext() {
  while (it->hasNext()) {
    curEdge = it->next();

    if (sg->isElement(curEdge)) {
      return;
    }
  }
  // set curEdge as invalid
  curEdge = edge();
}
edge OutEdgesIterator::next() {
  assert(curEdge.isValid());
  // we are already pointing to the next edge
  edge tmp = curEdge;
  // prepare next iteration
  prepareNext();
  return tmp;
}
bool OutEdgesIterator::hasNext() {
  return curEdge.isValid();
}
//===================================================================
InEdgesIterator::InEdgesIterator(const GraphView *sg, node n) : sg(sg) {
  assert(sg->isElement(n));
  it = sg->getRoot()->getInEdges(n);
  // prepare first iteration
  prepareNext();
}
InEdgesIterator::~InEdgesIterator() {
  delete it;
}
void InEdgesIterator::prepareNext() {
  while (it->hasNext()) {
    curEdge = it->next();

    if (sg->isElement(curEdge)) {
      return;
    }
  }

  // set curEdge as invalid
  curEdge = edge();
}
edge InEdgesIterator::next() {
  assert(curEdge.isValid());
  // we are already pointing to the next edge
  edge tmp = curEdge;
  // prepare next iteration
  prepareNext();
  return tmp;
}
bool InEdgesIterator::hasNext() {
  return curEdge.isValid();
}
//===================================================================
InOutEdgesIterator::InOutEdgesIterator(const GraphView *sg, node n) : sg(sg) {
  assert(sg->isElement(n));
  it = sg->getRoot()->getInOutEdges(n);
  // prepare first iteration
  prepareNext();
}
InOutEdgesIterator::~InOutEdgesIterator() {
  delete it;
}
void InOutEdgesIterator::prepareNext() {
  while (it->hasNext()) {
    curEdge = it->next();

    if (sg->isElement(curEdge)) {
      return;
    }
  }

  // set curEdge as invalid
  curEdge = edge();
}
edge InOutEdgesIterator::next() {
  assert(curEdge.isValid());
  // we are already pointing to the next edge
  edge tmp = curEdge;
  // prepare next iteration
  prepareNext();
  return tmp;
}
bool InOutEdgesIterator::hasNext() {
  return curEdge.isValid();
}
//===================================================================
}
