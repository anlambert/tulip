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
template <typename INDEX_TYPE>
class IteratorValue : public Iterator<INDEX_TYPE> {
public:
  virtual INDEX_TYPE nextValue(DataMem &) = 0;
};
//===================================================================
template <typename TYPE, typename INDEX_TYPE = uint>
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
  void set(const INDEX_TYPE i, typename StoredType<TYPE>::ConstReference value,
           bool forceDefaultValueRemoval = false);
  /**
   * add val to the value associated to i
   */
  void add(const INDEX_TYPE i, TYPE val);
  /**
   * get the value associated to i
   */
  typename StoredType<TYPE>::ConstReference get(const INDEX_TYPE i) const;
  /**
   * get the value associated to i and indicates if it is not the default one
   */
  typename StoredType<TYPE>::Reference get(const INDEX_TYPE i, bool &isNotDefault) const;
  /**
   * get the default value
   */
  typename StoredType<TYPE>::Reference getDefault() const;
  /**
   * return if the value associated to i is not the default one
   */
  bool hasNonDefaultValue(const INDEX_TYPE i) const;
  /**
   * return a pointer on an iterator for all the elements whose associated value
   * is equal to a given value or different from the default value.
   * A null pointer is returned in case of an iteration on all the elements
   * whose value is equal to the default value.
   */
  Iterator<INDEX_TYPE> *findAll(typename StoredType<TYPE>::ConstReference value,
                                bool equal = true) const;
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
  void invertBooleanValue(const INDEX_TYPE i);

private:
  MutableContainer(const MutableContainer<TYPE> &) {}
  void operator=(const MutableContainer<TYPE> &) {}
  typename StoredType<TYPE>::ConstReference operator[](const INDEX_TYPE i) const;
  void vecttohash();
  void hashtovect();
  void compress(INDEX_TYPE min, INDEX_TYPE max, uint nbElements);
  void vectset(const INDEX_TYPE i, typename StoredType<TYPE>::Value value);
  IteratorValue<INDEX_TYPE> *findAllValues(typename StoredType<TYPE>::ConstReference value,
                                           bool equal = true) const;

private:
  std::deque<typename StoredType<TYPE>::Value> *vData;
  std::unordered_map<INDEX_TYPE, typename StoredType<TYPE>::Value> *hData;
  INDEX_TYPE minIndex, maxIndex;
  typename StoredType<TYPE>::Value defaultValue;
  enum State { VECT = 0, HASH = 1 };
  State state;
  uint elementInserted;
  double ratio;
  bool compressing;
};

}

#include "cxx/MutableContainer.cxx"

#endif // TALIPOT_MUTABLE_CONTAINER_H
