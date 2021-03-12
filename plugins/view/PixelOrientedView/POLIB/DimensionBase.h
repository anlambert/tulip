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

#ifndef DIMENSION_BASE_H
#define DIMENSION_BASE_H

#include <vector>
#include <string>

struct DimensionBase {
  virtual ~DimensionBase() = default;
  virtual unsigned int numberOfItems() const = 0;
  virtual unsigned int numberOfValues() const = 0;
  virtual std::string getItemLabelAtRank(const unsigned int rank) const = 0;
  virtual std::string getItemLabel(const unsigned int itemId) const = 0;
  virtual double getItemValue(const unsigned int itemId) const = 0;
  virtual double getItemValueAtRank(const unsigned int rank) const = 0;
  virtual unsigned int getItemIdAtRank(const unsigned int rank) = 0;
  virtual unsigned int getRankForItem(const unsigned int itemId) = 0;
  virtual double minValue() const = 0;
  virtual double maxValue() const = 0;
  virtual std::vector<unsigned int> links(const unsigned int itemId) const = 0;
  virtual std::string getDimensionName() const = 0;
};

#endif // DIMENSION_BASE_H
