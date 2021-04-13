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

#ifndef TALIPOT_ID_MANAGER_H
#define TALIPOT_ID_MANAGER_H

#include <algorithm>
#include <climits>
#include <set>
#include <vector>
#include <deque>
#include <iostream>
#include <talipot/MutableContainer.h>
#include <talipot/StlIterator.h>
#include <talipot/ParallelTools.h>

namespace tlp {

// define a minimal structure to hold the state of an id manager
struct IdManagerState {
  // the first id in use
  unsigned int firstId;
  // the next id to use
  unsigned int nextId;
  // the unused ids between the first and the next
  std::set<unsigned int> freeIds;

  IdManagerState() : firstId(0), nextId(0) {}
};

// define a class to iterate through the non free ids
// of an IdManagerState
template <typename TYPE>
class IdManagerIterator : public Iterator<TYPE> {

private:
  unsigned int current;
  unsigned int last;
  const std::set<unsigned int> &freeIds;
  std::set<unsigned int>::const_iterator it;

public:
  IdManagerIterator(const IdManagerState &info)
      : current(info.firstId), last(info.nextId), freeIds(info.freeIds), it(freeIds.begin()) {
#ifdef TLP_NO_IDS_REUSE
    std::set<unsigned int>::const_reverse_iterator itr;
    itr = freeIds.rbegin();

    while (itr != freeIds.rend() && (*itr) == last - 1) {
      --last;
      ++itr;
    }

#endif
  }
  ~IdManagerIterator() override = default;

  bool hasNext() override {
    return (current < last);
  }

  TYPE next() override {
    unsigned int tmp = current;
    ++current;

    while (it != freeIds.end()) {
      if (current < *it) {
        return static_cast<TYPE>(tmp);
      }

      ++current;
      ++it;
    }

    return static_cast<TYPE>(tmp);
  }
};

/// class for the management of the identifiers
class TLP_SCOPE IdManager {

  // the current state
  IdManagerState state;

public:
  IdManager() = default;
  /**
   * Returns true if the id is not used else false.
   */
  bool is_free(unsigned int) const;
  /**
   * Free the id given in parameter.
   */
  void free(const unsigned int);
  /**
   * Returns a new id.
   */
  unsigned int get() {
#ifdef TLP_NO_IDS_REUSE
    return state.nextId++;
#else
    return state.firstId ? --state.firstId : (state.freeIds.empty() ? state.nextId++ : getFreeId());
#endif
  }

#ifndef TLP_NO_IDS_REUSE
  /**
   * remove and return the first available id from the free ids
   */
  unsigned int getFreeId();
#endif
  /**
   * assuming the given id is free.
   * remove it from free ids
   * (used to ensure the same id when loading a graph with subgraphs)
   */
  void getFreeId(unsigned int id);
  /**
   * return the current state of the Id manager
   */
  const IdManagerState &getState() {
    return state;
  }
  /**
   * restore a saved state, used by push/pop
   */
  void restoreState(const IdManagerState &info) {
    state = info;
  }
  /**
   * Returns an iterator on all the used ids. Warning, if
   * the idManager is modified (free, get) this iterator
   * will be invalid.
   */
  template <typename TYPE>
  Iterator<TYPE> *getIds() const {
    return new IdManagerIterator<TYPE>(state);
  }

  friend std::ostream &operator<<(std::ostream &, const IdManager &);
  friend class IdManagerTest;
};

std::ostream &operator<<(std::ostream &, const IdManager &);

// class for the management of the identifiers: node, edge
// or any type which can be easily cast in an unsigned int
template <typename ID_TYPE>
class TLP_SCOPE IdContainer : public std::vector<ID_TYPE> {
  // queue of free ids
  std::deque<ID_TYPE> freeIds;
  // the position of the ids
  std::vector<unsigned int> pos;

public:
  IdContainer() : std::vector<ID_TYPE>() {}

  // reset
  void clear() {
    std::vector<ID_TYPE>::clear();
    pos.clear();
    freeIds.clear();
  }

  // reserve enough room to store nb elts
  void reserve(size_t nb) {
    std::vector<ID_TYPE>::reserve(nb);
    pos.reserve(nb);
  }

  // return whether there is free ids or not
  bool hasFree() const {
    return !freeIds.empty();
  }

  // return the number of elts in the free storage
  unsigned int numberOfFree() const {
    return freeIds.size();
  }

  // return whether the id exist or not
  bool isElement(ID_TYPE elt) const {
    unsigned int id = elt;
    return id < pos.size() && pos[id] != UINT_MAX;
  }

  // return an iterator on the existing elts
  Iterator<ID_TYPE> *getElts() const {
    return stlIterator(*this);
  }

  // return the position of an existing elt
  unsigned int getPos(ID_TYPE elt) const {
    assert(isElement(elt));
    return pos[elt];
  }

  // add a new elt
  ID_TYPE add() {
    unsigned int curSize = this->size();
    // use a previously freed id if available,
    // create a new one otherwise
    ID_TYPE elt = hasFree() ? freeIds.front() : ID_TYPE(curSize);
    // pop reused freed id if any
    if (hasFree()) {
      freeIds.pop_front();
    } else {
      pos.resize(curSize + 1);
    }
    this->push_back(elt);
    pos[elt] = curSize;
    return elt;
  }

  // add nb new elts
  std::vector<ID_TYPE> addNb(unsigned int nb) {
    unsigned int lastPos = this->size();
    unsigned int i = 0;
    while (i++ < nb) {
      // use a previously freed id if available
      if (hasFree()) {
        this->push_back(freeIds.front());
        freeIds.pop_front();
        // create a new one otherwise
      } else {
        this->push_back(ID_TYPE(this->size()));
      }
    }

    pos.resize(lastPos + nb);

    for (i = 0; i < nb; ++i) {
      ID_TYPE elt = (*this)[lastPos + i];
      pos[elt] = lastPos + i;
    }

    return {this->begin() + lastPos, this->end()};
  }

  // push the elt in the free storage
  void free(ID_TYPE elt) {
    unsigned int curPos = pos[elt];
    unsigned int lastPos = this->size() - 1;

    if (curPos != lastPos) {
      // swap the elt with the last one
      assert((*this)[curPos] == elt);
      std::swap((*this)[curPos], (*this)[lastPos]);
      pos[(*this)[curPos]] = curPos;
    }

    // push free id at the end of the freeIds queue
    freeIds.push_back(elt);

    // decrement ids vector size
    this->resize(lastPos);

    pos[elt] = UINT_MAX;

    if (this->empty()) {
      // all elts are freed so forget them
      freeIds.clear();
      pos.resize(0);
    }
  }

  // swap two elts
  void swap(ID_TYPE a, ID_TYPE b) {
    assert(isElement(a));
    assert(isElement(b));
    std::swap((*this)[pos[a]], (*this)[pos[b]]);
    std::swap(pos[a], pos[b]);
  }

  // recompute elts positions
  void reIndex() {
    unsigned int nbElts = this->size();
    TLP_PARALLEL_MAP_INDICES(nbElts, [&](unsigned int i) { pos[(*this)[i]] = i; });
  }

  // ascending sort
  void sort() {
    std::sort(this->begin(), this->end());
    reIndex();
  }
};

// used as nodes/edges container in GraphView
template <typename ID_TYPE>
class SGraphIdContainer : public std::vector<ID_TYPE> {
  // used to store the elts positions in the vector
  MutableContainer<unsigned int> pos;

public:
  SGraphIdContainer() {
    pos.setAll(UINT_MAX);
  }
  bool isElement(ID_TYPE elt) const {
    return (getPos(elt) != UINT_MAX);
  }

  unsigned int getPos(ID_TYPE elt) const {
    return pos.get(elt);
  }

  void add(ID_TYPE elt) {
    assert(!isElement(elt));
    // put the elt at the end
    pos.set(elt, this->size());
    this->push_back(elt);
  }

  void clone(const std::vector<ID_TYPE> &elts) {
    static_cast<std::vector<ID_TYPE> &>(*this) = elts;
    unsigned int nb = elts.size();

    for (unsigned int i = 0; i < nb; ++i) {
      pos.set(elts[i], i);
    }
  }

  void remove(ID_TYPE elt) {
    assert(isElement(elt));
    // get the position of the elt to remove
    unsigned int i = pos.get(elt);
    assert(i < this->size());
    // put the last elt at the freed position
    unsigned int last = this->size() - 1;

    if (i < last) {
      pos.set(((*this)[i] = (*this)[last]), i);
    }

    // resize the container
    this->resize(last);
    // the elt no longer exist in the container
    pos.set(elt, UINT_MAX);
  }

  // ascending sort
  void sort() {
    std::sort(this->begin(), this->end());
    unsigned int nbElts = this->size();

    for (unsigned int i = 0; i < nbElts; ++i) {
      pos.set((*this)[i], i);
    }
  }
};
}

#endif // TALIPOT_ID_MANAGER_H
