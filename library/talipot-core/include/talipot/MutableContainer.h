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
class IteratorValue : public Iterator<unsigned int> {
public:
  virtual unsigned int nextValue(DataMem &) = 0;
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
  void set(const unsigned int i, typename StoredType<TYPE>::ConstReference value,
           bool forceDefaultValueRemoval = false);
  /**
   * add val to the value associated to i
   */
  void add(const unsigned int i, TYPE val);
  /**
   * get the value associated to i
   */
  typename StoredType<TYPE>::ConstReference get(const unsigned int i) const;
  /**
   * get the value associated to i and indicates if it is not the default one
   */
  typename StoredType<TYPE>::Reference get(const unsigned int i, bool &isNotDefault) const;
  /**
   * get the default value
   */
  typename StoredType<TYPE>::Reference getDefault() const;
  /**
   * return if the value associated to i is not the default one
   */
  bool hasNonDefaultValue(const unsigned int i) const;
  /**
   * return a pointer on an iterator for all the elements whose associated value
   * is equal to a given value or different from the default value.
   * A null pointer is returned in case of an iteration on all the elements
   * whose value is equal to the default value.
   */
  Iterator<unsigned int> *findAll(typename StoredType<TYPE>::ConstReference value,
                                  bool equal = true) const;
  /**
   * return the number of non default values
   */
  unsigned int numberOfNonDefaultValues() const;

  /**
   * return whether a non default value exists
   */
  bool hasNonDefaultValues() const {
    return numberOfNonDefaultValues() != 0;
  }

  /**
   * invert the boolean value set to i (do nothing for non boolean value)
   */
  void invertBooleanValue(const unsigned int i);

private:
  MutableContainer(const MutableContainer<TYPE> &) {}
  void operator=(const MutableContainer<TYPE> &) {}
  typename StoredType<TYPE>::ConstReference operator[](const unsigned int i) const;
  void vecttohash();
  void hashtovect();
  void compress(unsigned int min, unsigned int max, unsigned int nbElements);
  void vectset(const unsigned int i, typename StoredType<TYPE>::Value value);
  IteratorValue *findAllValues(typename StoredType<TYPE>::ConstReference value,
                               bool equal = true) const;

private:
  std::deque<typename StoredType<TYPE>::Value> *vData;
  std::unordered_map<unsigned int, typename StoredType<TYPE>::Value> *hData;
  unsigned int minIndex, maxIndex;
  typename StoredType<TYPE>::Value defaultValue;
  enum State { VECT = 0, HASH = 1 };
  State state;
  unsigned int elementInserted;
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
               unsigned int minIndex)
      : _value(value), _equal(equal), _pos(minIndex), vData(vData), it(vData->begin()) {
    while (it != (*vData).end() && StoredType<TYPE>::equal((*it), _value) != _equal) {
      ++it;
      ++_pos;
    }
  }
  bool hasNext() override {
    return (_pos < UINT_MAX && it != (*vData).end());
  }
  unsigned int next() override {
    unsigned int tmp = _pos;

    do {
      ++it;
      ++_pos;
    } while (it != (*vData).end() && StoredType<TYPE>::equal((*it), _value) != _equal);

    return tmp;
  }
  unsigned int nextValue(DataMem &val) override {
    static_cast<TypedValueContainer<TYPE> &>(val).value = StoredType<TYPE>::get(*it);
    unsigned int pos = _pos;

    do {
      ++it;
      ++_pos;
    } while (it != (*vData).end() && StoredType<TYPE>::equal((*it), _value) != _equal);

    return pos;
  }

private:
  const TYPE _value;
  bool _equal;
  unsigned int _pos;
  std::deque<typename StoredType<TYPE>::Value> *vData;
  typename std::deque<typename StoredType<TYPE>::Value>::const_iterator it;
};

// one for hash storage
template <typename TYPE>
class IteratorHash : public IteratorValue {
public:
  IteratorHash(const TYPE &value, bool equal,
               std::unordered_map<unsigned int, typename StoredType<TYPE>::Value> *hData)
      : _value(value), _equal(equal), hData(hData) {
    it = (*hData).begin();

    while (it != (*hData).end() && StoredType<TYPE>::equal((*it).second, _value) != _equal) {
      ++it;
    }
  }
  bool hasNext() override {
    return (it != (*hData).end());
  }
  unsigned int next() override {
    unsigned int tmp = (*it).first;

    do {
      ++it;
    } while (it != (*hData).end() && StoredType<TYPE>::equal((*it).second, _value) != _equal);

    return tmp;
  }
  unsigned int nextValue(DataMem &val) override {
    static_cast<TypedValueContainer<TYPE> &>(val).value = StoredType<TYPE>::get((*it).second);
    unsigned int pos = (*it).first;

    do {
      ++it;
    } while (it != (*hData).end() && StoredType<TYPE>::equal((*it).second, _value) != _equal);

    return pos;
  }

private:
  const TYPE _value;
  bool _equal;
  std::unordered_map<unsigned int, typename StoredType<TYPE>::Value> *hData;
  typename std::unordered_map<unsigned int, typename StoredType<TYPE>::Value>::const_iterator it;
};
}

#include "cxx/MutableContainer.cxx"

#endif // TALIPOT_MUTABLE_CONTAINER_H
