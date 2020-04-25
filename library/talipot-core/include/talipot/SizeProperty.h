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

#ifndef TALIPOT_SIZE_PROPERTY_H
#define TALIPOT_SIZE_PROPERTY_H

#include <unordered_map>

#include <talipot/PropertyTypes.h>
#include <talipot/AbstractProperty.h>

namespace tlp {

class PropertyContext;

typedef AbstractProperty<tlp::SizeType, tlp::SizeType> AbstractSizeProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps a tlp::Size value to graph elements.
 */
class TLP_SCOPE SizeProperty : public AbstractSizeProperty {

public:
  SizeProperty(Graph *, const std::string &n = "");

  Size getMax(const Graph *sg = nullptr);
  Size getMin(const Graph *sg = nullptr);
  void scale(const tlp::Vec3f &, const Graph *sg = nullptr);
  void scale(const tlp::Vec3f &, Iterator<node> *, Iterator<edge> *);

  // redefinition of some PropertyInterface methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }

  // redefinition of some AbstractProperty methods
  void setNodeValue(const node n, tlp::StoredType<Size>::ReturnedConstValue v) override;
  void setAllNodeValue(tlp::StoredType<Size>::ReturnedConstValue v,
                       const Graph *graph = nullptr) override;

  int compare(const node n1, const node n2) const override;

protected:
  void resetMinMax();

private:
  std::unordered_map<unsigned int, Size> max, min;
  std::unordered_map<unsigned int, bool> minMaxOk;
  void computeMinMax(const Graph *sg = nullptr);
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<tlp::Size> value to graph elements.
 */
class TLP_SCOPE SizeVectorProperty
    : public AbstractVectorProperty<tlp::SizeVectorType, tlp::SizeType> {
public:
  SizeVectorProperty(Graph *g, const std::string &n = "")
      : AbstractVectorProperty<SizeVectorType, SizeType>(g, n) {}

  // redefinition of some PropertyInterface methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }
};
#ifdef _MSC_VER
template class AbstractVectorProperty<SizeVectorType, SizeType>;
#endif
}
#endif // TALIPOT_SIZE_PROPERTY_H
