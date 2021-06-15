/**
 *
 * Copyright (C) 2021  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <talipot/MutableContainer.h>

INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<tlp::edge>), TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<tlp::node>), TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<uint>), TLP_TEMPLATE_DEFINE_SCOPE)

INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::BooleanType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::BooleanVectorType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::ColorType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::ColorVectorType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::DoubleType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::DoubleVectorType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::IntegerType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::IntegerVectorType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::PointType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::LineType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::SizeType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::SizeVectorType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::StringType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::StringVectorType)>),
                         TLP_TEMPLATE_DEFINE_SCOPE)

INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::BooleanType), tlp::node>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::BooleanVectorType), tlp::node>),
    TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::ColorType), tlp::node>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::ColorVectorType), tlp::node>),
    TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::DoubleType), tlp::node>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::DoubleVectorType), tlp::node>),
    TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::IntegerType), tlp::node>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::IntegerVectorType), tlp::node>),
    TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::PointType), tlp::node>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::LineType), tlp::node>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::SizeType), tlp::node>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::SizeVectorType), tlp::node>),
    TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::StringType), tlp::node>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::StringVectorType), tlp::node>),
    TLP_TEMPLATE_DEFINE_SCOPE)

INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::BooleanType), tlp::edge>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::BooleanVectorType), tlp::edge>),
    TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::ColorType), tlp::edge>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::ColorVectorType), tlp::edge>),
    TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::DoubleType), tlp::edge>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::DoubleVectorType), tlp::edge>),
    TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::IntegerType), tlp::edge>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::IntegerVectorType), tlp::edge>),
    TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::PointType), tlp::edge>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::LineType), tlp::edge>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::SizeType), tlp::edge>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::SizeVectorType), tlp::edge>),
    TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::StringType), tlp::edge>),
                         TLP_TEMPLATE_DEFINE_SCOPE)
INSTANTIATE_DLL_TEMPLATE(
    SINGLE_ARG(tlp::MutableContainer<REAL_TYPE(tlp::StringVectorType), tlp::edge>),
    TLP_TEMPLATE_DEFINE_SCOPE)
