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

#ifndef TALIPOT_MUTABLE_CONTAINER_H
#define TALIPOT_MUTABLE_CONTAINER_H

#include <deque>
#include <iostream>
#include <string>
#include <cassert>
#include <climits>
#include <cstring>
#include <unordered_map>
#include <talipot/config.h>
#include <talipot/StoredType.h>
#include <talipot/DataSet.h>
#include <talipot/Iterator.h>

namespace tlp {

//===================================================================
// we first define an interface
// to make easier the iteration on values
// stored in a MutableContainer for the GraphUpdatesRecorder
class IteratorValue : public Iterator<uint> {
public:
  virtual uint nextValue(DataMem &) = 0;
};
//===================================================================
template <typename TYPE>
class MutableContainer {
  friend class MutableContainerTest;
  friend class GraphUpdatesRecorder;

public:
  MutableContainer();
  ~MutableContainer();

  /**
   * Set the default value
   */
  void setDefault(typename StoredType<TYPE>::ConstReference value);
  /**
   * set the same value to all elements and modify the default value
   */
  void setAll(typename StoredType<TYPE>::ConstReference value);
  /**
   * set the value associated to i
   */
  void set(const uint i, typename StoredType<TYPE>::ConstReference value,
           bool forceDefaultValueRemoval = false);
  /**
   * add val to the value associated to i
   */
  void add(const uint i, TYPE val);
  /**
   * get the value associated to i
   */
  typename StoredType<TYPE>::ConstReference get(const uint i) const;
  /**
   * get the value associated to i and indicates if it is not the default one
   */
  typename StoredType<TYPE>::Reference get(const uint i, bool &isNotDefault) const;
  /**
   * get the default value
   */
  typename StoredType<TYPE>::Reference getDefault() const;
  /**
   * return if the value associated to i is not the default one
   */
  bool hasNonDefaultValue(const uint i) const;
  /**
   * return a pointer on an iterator for all the elements whose associated value
   * is equal to a given value or different from the default value.
   * A null pointer is returned in case of an iteration on all the elements
   * whose value is equal to the default value.
   */
  Iterator<uint> *findAll(typename StoredType<TYPE>::ConstReference value, bool equal = true) const;
  /**
   * return the number of non default values
   */
  uint numberOfNonDefaultValues() const;

  /**
   * return whether a non default value exists
   */
  bool hasNonDefaultValues() const {
    return numberOfNonDefaultValues() != 0;
  }

  /**
   * invert the boolean value set to i (do nothing for non boolean value)
   */
  void invertBooleanValue(const uint i);

private:
  MutableContainer(const MutableContainer<TYPE> &) {}
  void operator=(const MutableContainer<TYPE> &) {}
  typename StoredType<TYPE>::ConstReference operator[](const uint i) const;
  void vecttohash();
  void hashtovect();
  void compress(uint min, uint max, uint nbElements);
  void vectset(const uint i, typename StoredType<TYPE>::Value value);
  IteratorValue *findAllValues(typename StoredType<TYPE>::ConstReference value,
                               bool equal = true) const;

private:
  std::deque<typename StoredType<TYPE>::Value> *vData;
  std::unordered_map<uint, typename StoredType<TYPE>::Value> *hData;
  uint minIndex, maxIndex;
  typename StoredType<TYPE>::Value defaultValue;
  enum State { VECT = 0, HASH = 1 };
  State state;
  uint elementInserted;
  double ratio;
  bool compressing;
};

//===================================================================
// we implement 2 templates with IteratorValue as parent class
// for the two kinds of storage used in a MutableContainer
// one for vector storage
template <typename TYPE>
class IteratorVect : public tlp::IteratorValue {
public:
  IteratorVect(const TYPE &value, bool equal, std::deque<typename StoredType<TYPE>::Value> *vData,
               uint minIndex)
      : _value(value), _equal(equal), _pos(minIndex), vData(vData), it(vData->begin()) {
    while (it != (*vData).end() && StoredType<TYPE>::equal((*it), _value) != _equal) {
      ++it;
      ++_pos;
    }
  }
  bool hasNext() override {
    return (_pos < UINT_MAX && it != (*vData).end());
  }
  uint next() override {
    uint tmp = _pos;

    do {
      ++it;
      ++_pos;
    } while (it != (*vData).end() && StoredType<TYPE>::equal((*it), _value) != _equal);

    return tmp;
  }
  uint nextValue(DataMem &val) override {
    static_cast<TypedValueContainer<TYPE> &>(val).value = StoredType<TYPE>::get(*it);
    uint pos = _pos;

    do {
      ++it;
      ++_pos;
    } while (it != (*vData).end() && StoredType<TYPE>::equal((*it), _value) != _equal);

    return pos;
  }

private:
  const TYPE _value;
  bool _equal;
  uint _pos;
  std::deque<typename StoredType<TYPE>::Value> *vData;
  typename std::deque<typename StoredType<TYPE>::Value>::const_iterator it;
};

// one for hash storage
template <typename TYPE>
class IteratorHash : public IteratorValue {
public:
  IteratorHash(const TYPE &value, bool equal,
               std::unordered_map<uint, typename StoredType<TYPE>::Value> *hData)
      : _value(value), _equal(equal), hData(hData) {
    it = (*hData).begin();

    while (it != (*hData).end() && StoredType<TYPE>::equal((*it).second, _value) != _equal) {
      ++it;
    }
  }
  bool hasNext() override {
    return (it != (*hData).end());
  }
  uint next() override {
    uint tmp = (*it).first;

    do {
      ++it;
    } while (it != (*hData).end() && StoredType<TYPE>::equal((*it).second, _value) != _equal);

    return tmp;
  }
  uint nextValue(DataMem &val) override {
    static_cast<TypedValueContainer<TYPE> &>(val).value = StoredType<TYPE>::get((*it).second);
    uint pos = (*it).first;

    do {
      ++it;
    } while (it != (*hData).end() && StoredType<TYPE>::equal((*it).second, _value) != _equal);

    return pos;
  }

private:
  const TYPE _value;
  bool _equal;
  std::unordered_map<uint, typename StoredType<TYPE>::Value> *hData;
  typename std::unordered_map<uint, typename StoredType<TYPE>::Value>::const_iterator it;
};
}

#include "cxx/MutableContainer.cxx"

#endif // TALIPOT_MUTABLE_CONTAINER_H
