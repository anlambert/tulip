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

#include <talipot/VectorGraph.h>
#include <talipot/TlpTools.h>

using namespace std;
using namespace tlp;

namespace tlp {
template struct TLP_SCOPE VectorGraphProperty<double>::ValuesImpl;
template struct TLP_SCOPE VectorGraphProperty<node>::ValuesImpl;
template class TLP_SCOPE NodeProperty<node>;
template class TLP_SCOPE NodeProperty<double>;
template class TLP_SCOPE EdgeProperty<node>;
template class TLP_SCOPE EdgeProperty<double>;
}

namespace {
//==================================================================================
/**
 * \class bInOutNodeIterator
 * \brief internal class to iterate on adj+/adj- of element of a BAsicGraph
 * \see VectorGraph
 */
template <bool SENS>
class bInOutNodeIterator : public Iterator<node>, public MemoryPool<bInOutNodeIterator<SENS>> {
public:
  bInOutNodeIterator(const std::vector<node> &nodes, const std::vector<bool> &sens,
                     unsigned int elem)
      : itn(nodes.begin()), itn_end(nodes.end()), its(sens.begin()), its_end(sens.end()),
        _elem(elem) {
    update();
  }
  bool hasNext() override {
    return its != its_end;
  }
  node next() override {
    node n = *itn;
    ++its;
    ++itn;
    --_elem;
    update();
    return n;
  }

private:
  void update() {
    if (_elem == 0) {
      its = its_end;
      return;
    }

    while ((its != its_end) && ((*its) != SENS)) {
      ++its;
      ++itn;
    }
  }

  std::vector<node>::const_iterator itn, itn_end;
  std::vector<bool>::const_iterator its, its_end;
  unsigned int _elem;
};

//==================================================================================
/**
 * \class bInOutEdgeIterator
 * \brief internal class to iterate on star+/star- of element of a BAsicGraph
 * \see VectorGraph
 */
template <bool SENS>
class bInOutEdgeIterator : public Iterator<edge>, public MemoryPool<bInOutEdgeIterator<SENS>> {
public:
  bInOutEdgeIterator(const std::vector<edge> &edges, const std::vector<bool> &sens,
                     unsigned int elem)
      : ite(edges.begin()), ite_end(edges.end()), its(sens.begin()), its_end(sens.end()),
        _elem(elem) {
    update();
  }
  bool hasNext() override {
    return its != its_end;
  }
  edge next() override {
    edge e = *ite;
    ++its;
    ++ite;
    --_elem;
    update();
    return e;
  }

private:
  void update() {
    if (_elem == 0) {
      its = its_end;
      return;
    }

    while ((its != its_end) && ((*its) != SENS)) {
      ++its;
      ++ite;
    }
  }

  std::vector<edge>::const_iterator ite, ite_end;
  std::vector<bool>::const_iterator its, its_end;
  unsigned int _elem;
};
} // namespace

namespace tlp {
//===========================================
VectorGraph::VectorGraph() = default;
//=======================================================
VectorGraph::~VectorGraph() {
  for (auto *values : _nodeValues) {
    delete values;
  }

  for (auto *values : _edgeValues) {
    delete values;
  }
}
//=======================================================
void VectorGraph::clear() {
  delAllNodes();
  for (auto *values : _nodeValues) {
    delete values;
  }

  for (auto *values : _edgeValues) {
    delete values;
  }

  _nodeValues.clear();
  _edgeValues.clear();
}
//=======================================================
edge VectorGraph::existEdge(const node src, const node tgt, bool directed) const {
  const _iNodes &srcData = _nData[src];
  const _iNodes &tgtData = _nData[tgt];
  unsigned int srcDeg = srcData._adje.size();
  unsigned int tgtDeg = tgtData._adje.size();

  if (tgtDeg < srcDeg) {
    const std::vector<bool> &adjt = tgtData._adjt;
    const std::vector<node> &adjn = tgtData._adjn;

    for (size_t i = 0; i < tgtDeg; ++i) {
      if (directed && !adjt[i] && adjn[i] == src) {
        return tgtData._adje[i];
      }

      if (!directed && adjn[i] == src) {
        return tgtData._adje[i];
      }
    }
  } else {
    const std::vector<bool> &adjt = srcData._adjt;
    const std::vector<node> &adjn = srcData._adjn;

    for (size_t i = 0; i < srcDeg; ++i) {
      if (directed && adjt[i] && adjn[i] == tgt) {
        return srcData._adje[i];
      }

      if (!directed && adjn[i] == tgt) {
        return srcData._adje[i];
      }
    }
  }

  return edge();
}
//=======================================================
void VectorGraph::setEdgeOrder(const node n, const std::vector<edge> &v) {
  //@TODO add assert that test edge element of v are correct
  for (size_t i = 0; i < v.size() - 1; ++i) {
    swapEdgeOrder(n, _nData[n]._adje[i], v[i]);
  }

  //        integrityTest();
}
//=======================================================
void VectorGraph::swapEdgeOrder(const node n, const edge e1, const edge e2) {
  if (e1 == e2) {
    return; // we can't swap loops in our model
  }

  unsigned int e1i, e2i;

  auto &[src1, tgt1] = _eData[e1]._endsPos;
  auto &[src2, tgt2] = _eData[e2]._endsPos;

  if (source(e1) == n) {
    e1i = src1;
  } else {
    e1i = tgt1;
  }

  if (source(e2) == n) {
    e2i = src2;
  } else {
    e2i = tgt2;
  }

  edge tmpe = _nData[n]._adje[e1i];
  _nData[n]._adje[e1i] = _nData[n]._adje[e2i];
  _nData[n]._adje[e2i] = tmpe;
  node tmpn = _nData[n]._adjn[e1i];
  _nData[n]._adjn[e1i] = _nData[n]._adjn[e2i];
  _nData[n]._adjn[e2i] = tmpn;
  bool tmpt = _nData[n]._adjt[e1i];
  _nData[n]._adjt[e1i] = _nData[n]._adjt[e2i];
  _nData[n]._adjt[e2i] = tmpt;

  if (source(e1) == n) {
    src1 = e2i;
  } else {
    tgt1 = e2i;
  }

  if (source(e2) == n) {
    src2 = e1i;
  } else {
    tgt2 = e1i;
  }
}
//=======================================================
void VectorGraph::reserveNodes(const size_t nbNodes) {
  _nodes.reserve(nbNodes);
  _nData.reserve(nbNodes);
  for (auto *values : _nodeValues) {
    values->reserve(nbNodes);
  }
}
//=======================================================
void VectorGraph::reserveEdges(const size_t nbEdges) {
  _edges.reserve(nbEdges);
  _eData.reserve(nbEdges);
  for (auto *values : _edgeValues) {
    values->reserve(nbEdges);
  }
}
//=======================================================
void VectorGraph::reserveAdj(const size_t nbEdges) {
  for (unsigned int i = 0; i < _nodes.size(); ++i) {
    reserveAdj(_nodes[i], nbEdges);
  }
}
//=======================================================
void VectorGraph::reserveAdj(const node n, const size_t nbEdges) {
  _nData[n]._adjt.reserve(nbEdges);
  _nData[n]._adje.reserve(nbEdges);
  _nData[n]._adjn.reserve(nbEdges);
}
//=======================================================
Iterator<node> *VectorGraph::getNodes() const {
  return stlIterator(_nodes);
}
//=======================================================
Iterator<edge> *VectorGraph::getEdges() const {
  return stlIterator(_edges);
}
//=======================================================
Iterator<edge> *VectorGraph::getInOutEdges(const node n) const {
  return stlIterator(_nData[n]._adje);
}
//=======================================================
Iterator<edge> *VectorGraph::getOutEdges(const node n) const {
  return new bInOutEdgeIterator<true>(_nData[n]._adje, _nData[n]._adjt, outdeg(n));
}
//=======================================================
Iterator<edge> *VectorGraph::getInEdges(const node n) const {
  return new bInOutEdgeIterator<false>(_nData[n]._adje, _nData[n]._adjt, indeg(n));
}
//=======================================================
Iterator<node> *VectorGraph::getInOutNodes(const node n) const {
  return stlIterator(_nData[n]._adjn);
}
//=======================================================
Iterator<node> *VectorGraph::getInNodes(const node n) const {
  return new bInOutNodeIterator<false>(_nData[n]._adjn, _nData[n]._adjt, indeg(n));
}
//=======================================================
Iterator<node> *VectorGraph::getOutNodes(const node n) const {
  return new bInOutNodeIterator<true>(_nData[n]._adjn, _nData[n]._adjt, outdeg(n));
}
//=======================================================
node VectorGraph::addNode() {
  node newNode(_nodes.add());

  if (newNode.id == _nData.size()) {
    _nData.push_back(_iNodes());
    addNodeToValues(newNode);
  } else {
    _nData[newNode].clear();
  }

  return newNode;
}
//=======================================================
std::vector<node> VectorGraph::addNodes(unsigned int nb) {
  std::vector<node> addedNodes = _nodes.addNb(nb);

  unsigned int sz = _nData.size();

  if (sz < _nodes.size()) {
    _nData.resize(_nodes.size());
    addNodeToValues(node(_nodes.size() - 1));
    // get the number of recycled nodes
    // that need to be cleared
    nb -= _nodes.size() - sz;
  }

  for (auto n : addedNodes) {
    _nData[n].clear();
  }
  return addedNodes;
}
//=======================================================
void VectorGraph::delNode(const node n) {
  assert(isElement(n));
  delEdges(n);
  _nData[n].clear();
  _nodes.free(n);

  if (_nodes.empty()) {
    _nData.resize(0);
  }
}
//=======================================================
void VectorGraph::addEdgeInternal(const edge newEdge, const node src, const node tgt) {
  _iEdges &eData = _eData[newEdge];
  eData._ends = {src, tgt};
  _iNodes &srcData = _nData[src];
  auto &[eSrc, eTgt] = eData._endsPos;
  eSrc = srcData._adje.size();

  _iNodes &tgtData = _nData[tgt];

  if (src != tgt) {
    eTgt = tgtData._adje.size();
  } else {
    eTgt = srcData._adje.size() + 1;
  }

  srcData.addEdge(true, tgt, newEdge);
  tgtData.addEdge(false, src, newEdge);

  srcData._outdeg += 1;
}
//=======================================================
edge VectorGraph::addEdge(const node src, const node tgt) {
  assert(isElement(src));
  assert(isElement(tgt));
  edge newEdge(_edges.add());

  if (newEdge.id == _eData.size()) {
    _eData.resize(newEdge.id + 1);
    addEdgeToValues(newEdge);
  }

  addEdgeInternal(newEdge, src, tgt);

  // integrityTest();
  return newEdge;
}
//=======================================================
std::vector<edge> VectorGraph::addEdges(const std::vector<std::pair<node, node>> &ends) {
  unsigned int nb = ends.size();

  std::vector<edge> addedEdges = _edges.addNb(nb);

  unsigned int sz = _eData.size();

  if (sz < _edges.size()) {
    _eData.resize(_edges.size());
    addEdgeToValues(edge(_edges.size() - 1));
  }

  for (unsigned int i = 0; i < nb; ++i) {
    const auto &[src, tgt] = ends[i];
    addEdgeInternal(addedEdges[i], src, tgt);
  }
  return addedEdges;
}

//=======================================================
void VectorGraph::delEdge(const edge e) {
  assert(isElement(e));
  const auto &[psrc, ptgt] = _eData[e]._ends;
  _nData[psrc]._outdeg -= 1;
  partialDelEdge(psrc, e);

  if (psrc != ptgt) {
    partialDelEdge(ptgt, e);
  }

  // remove the edge in the edge vector
  removeEdge(e);
  // integrityTest();
}
//=======================================================
void VectorGraph::delEdges(const node n) {
  assert(isElement(n));
  _iNodes &ndata = _nData[n];
  const vector<edge> &adje = ndata._adje;
  const vector<node> &adjn = ndata._adjn;
  const vector<bool> &adjt = ndata._adjt;

  for (unsigned int i = 0; i < adje.size(); ++i) {
    edge e = adje[i];

    if (isElement(e)) {

      node opp = adjn[i];

      if (opp != n) { // not a loop
        partialDelEdge(opp, e);

        if (!adjt[i]) {
          _nData[opp]._outdeg -= 1;
        }
      }

      removeEdge(e);
    }
  }

  ndata.clear();
}
//=======================================================
void VectorGraph::delAllEdges() {
  _edges.clear();
  _eData.clear();

  for (size_t i = 0; i < _nodes.size(); ++i) {
    _nData[_nodes[i]].clear();
  }
}
//=======================================================
void VectorGraph::delAllNodes() {
  _edges.clear();
  _eData.clear();
  _nodes.clear();
  _nData.clear();
}
//=======================================================
node VectorGraph::opposite(const edge e, const node n) const {
  assert(isElement(e));
  assert(isElement(n));

  const auto &[src, tgt] = _eData[e]._ends;

  if (src == n) {
    return tgt;
  } else {
    assert(tgt == n);
    return src;
  }
}
//=======================================================
void VectorGraph::reverse(const edge e) {
  assert(isElement(e));
  auto &[src, tgt] = _eData[e]._ends;
  _nData[src]._outdeg -= 1;
  _nData[tgt]._outdeg += 1;
  auto &[srcPos, tgtPos] = _eData[e]._endsPos;
  _nData[src]._adjt[srcPos] = false;
  _nData[tgt]._adjt[tgtPos] = true;
  std::swap(src, tgt);
  std::swap(srcPos, tgtPos);
}
//=======================================================
void VectorGraph::setEnds(const edge e, const node src, const node tgt) {
  assert(isElement(e));
  assert(isElement(src));
  assert(isElement(tgt));

  const auto &[psrc, ptgt] = _eData[e]._ends;
  auto &[srcPos, tgtPos] = _eData[e]._endsPos;

  _nData[psrc]._outdeg -= 1;
  _nData[src]._outdeg += 1;

  partialDelEdge(psrc, e);

  if (psrc != ptgt) {
    partialDelEdge(ptgt, e);
  }

  _eData[e]._ends = {src, tgt};
  srcPos = _nData[src]._adje.size();

  if (src != tgt) {
    tgtPos = _nData[tgt]._adje.size();
  } else { // loop
    tgtPos = _nData[src]._adje.size() + 1;
  }

  _nData[src].addEdge(true, tgt, e);
  _nData[tgt].addEdge(false, src, e);
}
//=======================================================
void VectorGraph::shuffleNodes() {
  shuffle(_nodes.begin(), _nodes.end(), getRandomNumberGenerator());

  // recompute indices of nodes
  _nodes.reIndex();
}
//=======================================================
void VectorGraph::shuffleEdges() {
  shuffle(_edges.begin(), _edges.end(), getRandomNumberGenerator());

  // recompute indices of edges
  _edges.reIndex();
}
//=======================================================
void VectorGraph::swap(const node a, const node b) {
  _nodes.swap(a, b);
}
//=======================================================
void VectorGraph::swap(const edge a, const edge b) {
  _edges.swap(a, b);
}
//=======================================================
void VectorGraph::dump() const {
  tlp::debug() << "nodes : ";
  for (auto n : nodes()) {
    tlp::debug() << n.id << " ";
  }
  tlp::debug() << endl;
  tlp::debug() << "edges: ";
  for (auto e : edges()) {
    tlp::debug() << "e_" << e.id << "(" << source(e).id << "," << target(e).id << ") ";
  }
  tlp::debug() << endl;

  for (auto n : nodes()) {
    tlp::debug() << "n_" << n << "{";
    for (auto e : getInOutEdges(n)) {
      tlp::debug() << "e_" << e.id << " ";
    }
    tlp::debug() << "}";
    tlp::debug() << endl;
  }
}
//=============================================================
void VectorGraph::integrityTest() {
  double sumDeg = 0;

  for (unsigned int i = 0; i < numberOfNodes(); ++i) {
    testCond("nodesId in array :", _nodes.getPos(_nodes[i]) == i);
  }

  for (unsigned int i = 0; i < numberOfEdges(); ++i) {
    testCond("edgesId in array :", _edges.getPos(_edges[i]) == i);
  }

  set<edge> edgeFound;
  set<node> nodeFound;

  for (unsigned int i = 0; i < numberOfNodes(); ++i) {
    testCond("edge adjn == node adje",
             _nData[_nodes[i]]._adjn.size() == _nData[_nodes[i]]._adje.size());
    testCond("edge adjn == node adjt",
             _nData[_nodes[i]]._adjn.size() == _nData[_nodes[i]]._adjt.size());
    unsigned int _indeg = 0;
    unsigned int _outdeg = 0;
    node n = _nodes[i];
    nodeFound.insert(n);

    for (unsigned int j = 0; j < _nData[n]._adjn.size(); ++j) {
      testCond("opposite", opposite(_nData[n]._adje[j], _nodes[i]) == _nData[n]._adjn[j]);

      if (!_nData[n]._adjt[j]) {
        ++_indeg;
      } else {
        ++_outdeg;
      }

      edgeFound.insert(_nData[n]._adje[j]);
      nodeFound.insert(_nData[n]._adjn[j]);
    }

    testCond("_adjt in", _indeg == indeg(n));
    testCond("_adjt out", _outdeg == outdeg(n));

    sumDeg += _nData[_nodes[i]]._adjn.size();
    testCond("deg/in/out", _nData[n]._adjn.size() == outdeg(n) + indeg(n));
  }

  testCond("edges found", edgeFound.size() == _edges.size());
  testCond("nodes found", nodeFound.size() == _nodes.size());

  // edge extremities pos test
  for (unsigned int i = 0; i < numberOfEdges(); ++i) {
    edge e = _edges[i];
    node src = source(e);
    node tgt = target(e);
    const auto &[srcp, tgtp] = _eData[e]._endsPos;
    testCond("p1 :", _nData[src]._adje[srcp] == e);
    testCond("p2 :", _nData[tgt]._adje[tgtp] == e);
    testCond("p3 :", _nData[src]._adjn[srcp] == tgt);
    testCond("p4 :", _nData[tgt]._adjn[tgtp] == src);
    testCond("p5 :", _nData[src]._adjt[srcp]);
    testCond("p6 :", !_nData[tgt]._adjt[tgtp]);
  }

  testCond("Nb edges", sumDeg == (numberOfEdges() * 2));
}

//=======================================================
void VectorGraph::testCond(string str, bool b) const {
  if (!b) {
    tlp::debug() << str << flush << endl;
    dump();
    exit(1);
  }
}
//=======================================================
void VectorGraph::addNodeToValues(node n) {
  for (auto *values : _nodeValues) {
    values->addElement(n.id);
  }
}
//=======================================================
void VectorGraph::addEdgeToValues(edge e) {
  for (auto *values : _edgeValues) {
    values->addElement(e.id);
  }
}
//=======================================================
void VectorGraph::removeEdge(edge e) {
  if (_edges.isElement(e)) {
    _edges.free(e);

    if (_edges.empty()) {
      _eData.resize(0);
    }
  }
}
//=======================================================
void VectorGraph::moveEdge(node n, unsigned int a, unsigned int b) {
  if (a == b) {
    return;
  }

  edge moved = _nData[n]._adje[a];
  auto &[srcPos, tgtPos] = _eData[moved]._endsPos;

  if (_nData[n]._adjt[a]) { // if true in edges -> target
    srcPos = b;
  } else {
    tgtPos = b;
  }

  _nData[n]._adje[b] = _nData[n]._adje[a];
  _nData[n]._adjn[b] = _nData[n]._adjn[a];
  _nData[n]._adjt[b] = _nData[n]._adjt[a];
}
//=======================================================
void VectorGraph::partialDelEdge(node n, edge e) {
  // e1 e2 e3 e4 e1 e6 e7 e7
  unsigned int endP = _nData[n]._adje.size() - 1;
  const auto &[src, tgt] = _eData[e]._ends;
  const auto &[srcPos, tgtPos] = _eData[e]._endsPos;

  if (endP > 0) {
    bool loop = (src == tgt);

    if (loop) {
      unsigned int i1 = std::max(srcPos, tgtPos);
      unsigned int i2 = std::min(srcPos, tgtPos);
      moveEdge(n, endP, i1);
      --endP;
      moveEdge(n, endP, i2);
    } else {
      unsigned int i;

      if (src == n) {
        i = srcPos;
      } else {
        i = tgtPos;
      }

      moveEdge(n, endP, i);
    }
  }

  _nData[n]._adje.resize(endP);
  _nData[n]._adjn.resize(endP);
  _nData[n]._adjt.resize(endP);
}
//=======================================================
}
