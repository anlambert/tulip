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

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include <unordered_map>
#include "talipot/PythonIncludes.h"
#include "talipot/PythonCppTypesConverter.h"

static std::unordered_map<std::string, std::string> &getTypenamesMap() {
  static std::unordered_map<std::string, std::string> ret;
  ret[tlp::demangleClassName<std::string>()] = "std::string";
  ret[tlp::demangleClassName<tlp::Vec3f>()] = "tlp::Vec3f";
  ret[tlp::demangleClassName<tlp::Coord>()] = "tlp::Coord";
  ret[tlp::demangleClassName<tlp::Size>()] = "tlp::Size";
  ret[tlp::demangleClassName<std::vector<int>>()] = "std::vector<int>";
  ret[tlp::demangleClassName<std::vector<unsigned int>>()] = "std::vector<uint>";
  ret[tlp::demangleClassName<std::vector<long>>()] = "std::vector<long>";
  ret[tlp::demangleClassName<std::vector<unsigned long>>()] = "std::vector<unsigned long>";
  ret[tlp::demangleClassName<std::vector<float>>()] = "std::vector<float>";
  ret[tlp::demangleClassName<std::vector<double>>()] = "std::vector<double>";
  ret[tlp::demangleClassName<std::vector<bool>>()] = "std::vector<bool>";
  ret[tlp::demangleClassName<std::vector<std::string>>()] = "std::vector<std::string>";
  ret[tlp::demangleClassName<std::vector<tlp::node>>()] = "std::vector<tlp::node>";
  ret[tlp::demangleClassName<std::vector<tlp::edge>>()] = "std::vector<tlp::edge>";
  ret[tlp::demangleClassName<std::vector<tlp::Coord>>()] = "std::vector<tlp::Coord>";
  ret[tlp::demangleClassName<std::vector<tlp::Color>>()] = "std::vector<tlp::Color>";
  ret[tlp::demangleClassName<std::vector<tlp::Size>>()] = "std::vector<tlp::Size>";
  ret[tlp::demangleClassName<std::vector<tlp::Graph *>>()] = "std::vector<tlp::Graph*>";
  ret[tlp::demangleClassName<std::vector<tlp::ColorScale>>()] = "std::vector<tlp::ColorScale>";
  ret[tlp::demangleClassName<std::vector<tlp::StringCollection>>()] =
      "std::vector<tlp::StringCollection>";
  ret[tlp::demangleClassName<std::vector<tlp::BooleanProperty *>>()] =
      "std::vector<tlp::BooleanProperty *>";
  ret[tlp::demangleClassName<std::vector<tlp::ColorProperty *>>()] =
      "std::vector<tlp::ColorProperty *>";
  ret[tlp::demangleClassName<std::vector<tlp::DoubleProperty *>>()] =
      "std::vector<tlp::DoubleProperty *>";
  ret[tlp::demangleClassName<std::vector<tlp::IntegerProperty *>>()] =
      "std::vector<tlp::IntegerProperty *>";
  ret[tlp::demangleClassName<std::vector<tlp::LayoutProperty *>>()] =
      "std::vector<tlp::LayoutProperty *>";
  ret[tlp::demangleClassName<std::vector<tlp::SizeProperty *>>()] =
      "std::vector<tlp::SizeProperty *>";
  ret[tlp::demangleClassName<std::vector<tlp::StringProperty *>>()] =
      "std::vector<tlp::StringProperty *>";
  ret[tlp::demangleClassName<std::vector<tlp::PropertyInterface *>>()] =
      "std::vector<tlp::PropertyInterface *>";
  ret[tlp::demangleClassName<std::list<int>>()] = "std::list<int>";
  ret[tlp::demangleClassName<std::list<unsigned int>>()] = "std::list<uint>";
  ret[tlp::demangleClassName<std::list<long>>()] = "std::list<long>";
  ret[tlp::demangleClassName<std::list<unsigned long>>()] = "std::list<unsigned long>";
  ret[tlp::demangleClassName<std::list<float>>()] = "std::list<float>";
  ret[tlp::demangleClassName<std::list<double>>()] = "std::list<double>";
  ret[tlp::demangleClassName<std::list<bool>>()] = "std::list<bool>";
  ret[tlp::demangleClassName<std::list<std::string>>()] = "std::list<std::string>";
  ret[tlp::demangleClassName<std::list<tlp::node>>()] = "std::list<tlp::node>";
  ret[tlp::demangleClassName<std::list<tlp::edge>>()] = "std::list<tlp::edge>";
  ret[tlp::demangleClassName<std::list<tlp::Coord>>()] = "std::list<tlp::Coord>";
  ret[tlp::demangleClassName<std::list<tlp::Color>>()] = "std::list<tlp::Color>";
  ret[tlp::demangleClassName<std::list<tlp::Size>>()] = "std::list<tlp::Size>";
  ret[tlp::demangleClassName<std::list<tlp::Graph *>>()] = "std::list<tlp::Graph*>";
  ret[tlp::demangleClassName<std::list<tlp::ColorScale>>()] = "std::list<tlp::ColorScale>";
  ret[tlp::demangleClassName<std::list<tlp::StringCollection>>()] =
      "std::list<tlp::StringCollection>";
  ret[tlp::demangleClassName<std::list<tlp::BooleanProperty *>>()] =
      "std::list<tlp::BooleanProperty *>";
  ret[tlp::demangleClassName<std::list<tlp::ColorProperty *>>()] =
      "std::list<tlp::ColorProperty *>";
  ret[tlp::demangleClassName<std::list<tlp::DoubleProperty *>>()] =
      "std::list<tlp::DoubleProperty *>";
  ret[tlp::demangleClassName<std::list<tlp::IntegerProperty *>>()] =
      "std::list<tlp::IntegerProperty *>";
  ret[tlp::demangleClassName<std::list<tlp::LayoutProperty *>>()] =
      "std::list<tlp::LayoutProperty *>";
  ret[tlp::demangleClassName<std::list<tlp::SizeProperty *>>()] = "std::list<tlp::SizeProperty *>";
  ret[tlp::demangleClassName<std::list<tlp::StringProperty *>>()] =
      "std::list<tlp::StringProperty *>";
  ret[tlp::demangleClassName<std::list<tlp::PropertyInterface *>>()] =
      "std::list<tlp::PropertyInterface *>";
  ret[tlp::demangleClassName<std::set<int>>()] = "std::set<int>";
  ret[tlp::demangleClassName<std::set<unsigned int>>()] = "std::set<uint>";
  ret[tlp::demangleClassName<std::set<long>>()] = "std::set<long>";
  ret[tlp::demangleClassName<std::set<unsigned long>>()] = "std::set<unsigned long>";
  ret[tlp::demangleClassName<std::set<float>>()] = "std::set<float>";
  ret[tlp::demangleClassName<std::set<double>>()] = "std::set<double>";
  ret[tlp::demangleClassName<std::set<std::string>>()] = "std::set<std::string>";
  ret[tlp::demangleClassName<std::set<tlp::node>>()] = "std::set<tlp::node>";
  ret[tlp::demangleClassName<std::set<tlp::edge>>()] = "std::set<tlp::edge>";
  ret[tlp::demangleClassName<std::set<tlp::Coord>>()] = "std::set<tlp::Coord>";
  ret[tlp::demangleClassName<std::set<tlp::Color>>()] = "std::set<tlp::Color>";
  ret[tlp::demangleClassName<std::set<tlp::Size>>()] = "std::set<tlp::Size>";
  ret[tlp::demangleClassName<std::set<tlp::Graph *>>()] = "std::set<tlp::Graph*>";
  return ret;
}

static std::unordered_map<std::string, std::string> &cppTypenameToSipTypename = getTypenamesMap();

void *convertSipWrapperToCppType(PyObject *sipWrapper, const std::string &cppTypename,
                                 const bool transferTo) {
  PyObject *pyObject = sipWrapper;
  const sipTypeDef *kTypeDef = sipFindType(cppTypename.c_str());

  if (kTypeDef && sipCanConvertToType(pyObject, kTypeDef, transferTo ? 0 : SIP_NOT_NONE)) {
    int state = 0, err = 0;

    if (!transferTo) {
      return sipConvertToType(pyObject, kTypeDef, Py_None, SIP_NOT_NONE, &state, &err);
    } else {
      void *p = sipConvertToType(pyObject, kTypeDef, nullptr, 0, &state, &err);
      sipTransferTo(pyObject, pyObject);
      return p;
    }
  } else if (cppTypenameToSipTypename.find(cppTypename) != cppTypenameToSipTypename.end()) {
    kTypeDef = sipFindType(cppTypenameToSipTypename[cppTypename].c_str());

    if (kTypeDef && sipCanConvertToType(pyObject, kTypeDef, transferTo ? 0 : SIP_NOT_NONE)) {
      int state = 0, err = 0;

      if (!transferTo) {
        return sipConvertToType(pyObject, kTypeDef, Py_None, SIP_NOT_NONE, &state, &err);
      } else {
        void *p = sipConvertToType(pyObject, kTypeDef, nullptr, 0, &state, &err);
        sipTransferTo(pyObject, pyObject);
        return p;
      }
    }
  }

  return nullptr;
}

PyObject *convertCppTypeToSipWrapper(void *cppObj, const std::string &cppTypename, bool fromNew) {
  const sipTypeDef *kTypeDef = sipFindType(cppTypename.c_str());

  if (kTypeDef) {
    if (fromNew) {
      return sipConvertFromNewType(cppObj, kTypeDef, nullptr);
    } else {
      return sipConvertFromType(cppObj, kTypeDef, nullptr);
    }
  } else if (cppTypenameToSipTypename.find(cppTypename) != cppTypenameToSipTypename.end()) {
    kTypeDef = sipFindType(cppTypenameToSipTypename[cppTypename].c_str());

    if (kTypeDef) {
      if (fromNew) {
        return sipConvertFromNewType(cppObj, kTypeDef, nullptr);
      } else {
        return sipConvertFromType(cppObj, kTypeDef, nullptr);
      }
    }
  }

  return nullptr;
}

bool convertPyObjectToBool(PyObject *pyObject, bool &cppObject) {
  if (PyBool_Check(pyObject)) {
    cppObject = (pyObject == Py_True);
    return true;
  }

  return false;
}

PyObject *convertBoolToPyObject(bool cppObject) {
  return PyBool_FromLong(cppObject);
}

bool convertPyObjectToLong(PyObject *pyObject, long &cppObject) {
  if (PyLong_Check(pyObject)) {
    cppObject = PyLong_AsLong(pyObject);
    return true;
  }

  return false;
}

bool convertPyObjectToUnsignedLong(PyObject *pyObject, unsigned long &cppObject) {
  if (PyLong_Check(pyObject)) {
    cppObject = PyLong_AsUnsignedLong(pyObject);
    return true;
  }

  return false;
}

PyObject *convertLongToPyObject(long cppObject) {
  return PyLong_FromLong(cppObject);
}

PyObject *convertUnsignedLongToPyObject(unsigned long cppObject) {
  return PyLong_FromUnsignedLong(cppObject);
}

bool convertPyObjectToDouble(PyObject *pyObject, double &cppObject) {
  if (PyFloat_Check(pyObject)) {
    cppObject = PyFloat_AsDouble(pyObject);
    return true;
  }

  return false;
}

PyObject *convertDoubleToPyObject(double cppObject) {
  return PyFloat_FromDouble(cppObject);
}

template <typename T>
PyObject *getPyObjectFromCppObject(const T &cppObject) {
  PyObject *pyObj = nullptr;
  CppObjectToPyObjectConverter<T> converter;
  converter.convert(cppObject, pyObj);
  return pyObj;
}

template <typename T>
T getCppObjectFromPyObject(PyObject *pyObj) {
  T v{};
  PyObjectToCppObjectConverter<T> converter;
  converter.convert(pyObj, v);
  return v;
}

template <typename T>
T *getCppPointerFromPyObject(PyObject *pyObj) {
  T *v = nullptr;
  PyObjectToCppObjectConverter<T *> converter;
  converter.convert(pyObj, v);
  return v;
}

#define CHECK_BASE_CPP_TYPE_CONVERSION(T)                                               \
  if (!pyObj && dataType && dataType->getTypeName() == std::string(typeid(T).name())) { \
    pyObj = getPyObjectFromCppObject(*(static_cast<T *>(dataType->value)));             \
  }

#define CHECK_SIP_CPP_TYPE_CONVERSION(T)                                                \
  if (!pyObj && dataType && dataType->getTypeName() == std::string(typeid(T).name())) { \
    if (noCopy) {                                                                       \
      pyObj = getPyObjectFromCppObject(static_cast<T *>(dataType->value));              \
    } else {                                                                            \
      pyObj = getPyObjectFromCppObject(*(static_cast<T *>(dataType->value)));           \
    }                                                                                   \
  }

PyObject *getPyObjectFromDataType(const tlp::DataType *dataType, bool noCopy) {
  PyObject *pyObj = nullptr;
  CHECK_BASE_CPP_TYPE_CONVERSION(bool)
  CHECK_BASE_CPP_TYPE_CONVERSION(int)
  CHECK_BASE_CPP_TYPE_CONVERSION(long)
  CHECK_BASE_CPP_TYPE_CONVERSION(unsigned int)
  CHECK_BASE_CPP_TYPE_CONVERSION(unsigned long)
  CHECK_BASE_CPP_TYPE_CONVERSION(double)
  CHECK_BASE_CPP_TYPE_CONVERSION(float)
  CHECK_BASE_CPP_TYPE_CONVERSION(std::string)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::node)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::edge)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::Graph *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::PluginProgress *)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::DataSet)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::Coord)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::Color)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::Size)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::ColorScale)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::StringCollection)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::BooleanProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::ColorProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::DoubleProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::IntegerProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::NumericProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::LayoutProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::SizeProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::StringProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::BooleanVectorProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::ColorVectorProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::DoubleVectorProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::IntegerVectorProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::CoordVectorProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::SizeVectorProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::StringVectorProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::PropertyInterface *)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::Graph *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::node>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::edge>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<std::string>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::Coord>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::Color>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::Size>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::ColorScale>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::StringCollection>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::BooleanProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::ColorProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::DoubleProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::IntegerProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::LayoutProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::SizeProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::StringProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::PropertyInterface *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<bool>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<unsigned int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<unsigned long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<double>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<float>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::Graph *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::node>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::edge>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<std::string>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::Coord>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::Color>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::Size>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<unsigned int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<unsigned long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<double>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<float>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::Graph *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::node>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::edge>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<std::string>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::Coord>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::Color>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::Size>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::ColorScale>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::StringCollection>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::BooleanProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::ColorProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::DoubleProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::IntegerProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::LayoutProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::SizeProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::StringProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::PropertyInterface *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<bool>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<unsigned int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<unsigned long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<double>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<float>)
  return pyObj;
}

#define CHECK_SIP_TYPE_CONVERSION(CPP_TYPE, SIP_TYPE_STR)                               \
  if (sipCanConvertToType(pyObj, sipFindType(SIP_TYPE_STR), SIP_NOT_NONE)) {            \
    if (!dataType || dataType->getTypeName() == std::string(typeid(CPP_TYPE).name())) { \
      valSetter.setValue(getCppObjectFromPyObject<CPP_TYPE>(pyObj));                    \
      return true;                                                                      \
    }                                                                                   \
  }

#define CHECK_SIP_ENUM_CONVERSION(SIP_TYPE_STR)                \
  if (sipCanConvertToEnum(pyObj, sipFindType(SIP_TYPE_STR))) { \
    valSetter.setValue(int(PyLong_AsLong(pyObj)));             \
    return true;                                               \
  }

#define CHECK_SIP_POINTER_TYPE_CONVERSION(CPP_TYPE, SIP_TYPE_STR)                         \
  if (sipCanConvertToType(pyObj, sipFindType(SIP_TYPE_STR), 0)) {                         \
    if (!dataType || dataType->getTypeName() == std::string(typeid(CPP_TYPE *).name())) { \
      valSetter.setValue(getCppPointerFromPyObject<CPP_TYPE>(pyObj));                     \
      return true;                                                                        \
    }                                                                                     \
  }

#define CHECK_SIP_VECTOR_LIST_CONVERSION(CPP_TYPE, SIP_TYPE_STR)                                   \
  if (sipCanConvertToType(pyObj,                                                                   \
                          sipFindType((std::string("std::vector<") + SIP_TYPE_STR + ">").c_str()), \
                          SIP_NOT_NONE)) {                                                         \
    if (dataType && dataType->getTypeName() == std::string(typeid(std::list<CPP_TYPE>).name())) {  \
      valSetter.setValue(getCppObjectFromPyObject<std::list<CPP_TYPE>>(pyObj));                    \
    } else {                                                                                       \
      valSetter.setValue(getCppObjectFromPyObject<std::vector<CPP_TYPE>>(pyObj));                  \
    }                                                                                              \
    return true;                                                                                   \
  }

bool setCppValueFromPyObject(PyObject *pyObj, ValueSetter &valSetter, tlp::DataType *dataType) {

  if (PyBool_Check(pyObj)) {
    valSetter.setValue(getCppObjectFromPyObject<bool>(pyObj));
    return true;
  }

  if (PyLong_Check(pyObj)) {
    long val = getCppObjectFromPyObject<long>(pyObj);

    if (dataType && dataType->getTypeName() == std::string(typeid(int).name())) {
      valSetter.setValue(int(val));
    } else if (dataType && dataType->getTypeName() == std::string(typeid(unsigned int).name())) {
      valSetter.setValue(getCppObjectFromPyObject<unsigned int>(pyObj));
    } else if (dataType && dataType->getTypeName() == std::string(typeid(long).name())) {
      valSetter.setValue(val);
    } else if (dataType && dataType->getTypeName() == std::string(typeid(unsigned long).name())) {
      valSetter.setValue(getCppObjectFromPyObject<unsigned long>(pyObj));
    } else if (dataType && dataType->getTypeName() == std::string(typeid(float).name())) {
      valSetter.setValue(float(val));
    } else if (dataType && dataType->getTypeName() == std::string(typeid(double).name())) {
      valSetter.setValue(double(val));
    } else {
      valSetter.setValue(int(val));
    }

    return true;
  }

  if (PyFloat_Check(pyObj)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(float).name())) {
      valSetter.setValue(getCppObjectFromPyObject<float>(pyObj));
    } else {
      valSetter.setValue(getCppObjectFromPyObject<double>(pyObj));
    }

    return true;
  }

  CHECK_SIP_VECTOR_LIST_CONVERSION(bool, "bool")

  if (sipCanConvertToType(pyObj, sipFindType("std::vector<long>"), SIP_NOT_NONE)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(std::vector<int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<int>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::vector<unsigned int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<unsigned int>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::vector<long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<long>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::vector<unsigned long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<unsigned long>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::vector<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<float>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::vector<double>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<double>>(pyObj));

    } else if (dataType && dataType->getTypeName() == std::string(typeid(std::list<int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<int>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::list<unsigned int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<unsigned int>>(pyObj));
    } else if (dataType && dataType->getTypeName() == std::string(typeid(std::list<long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<long>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::list<unsigned long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<unsigned long>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::list<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<float>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::list<double>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<double>>(pyObj));
    } else {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<int>>(pyObj));
    }

    return true;
  }

  if (sipCanConvertToType(pyObj, sipFindType("std::vector<double>"), SIP_NOT_NONE)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(std::vector<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<float>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::list<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<float>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::list<double>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<double>>(pyObj));
    } else {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<double>>(pyObj));
    }

    return true;
  }

  if (sipCanConvertToType(pyObj, sipFindType("std::set<long>"), SIP_NOT_NONE)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(std::set<int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<int>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::set<unsigned int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<unsigned int>>(pyObj));
    } else if (dataType && dataType->getTypeName() == std::string(typeid(std::set<long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<long>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::set<unsigned long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<unsigned long>>(pyObj));
    } else if (dataType && dataType->getTypeName() == std::string(typeid(std::set<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<float>>(pyObj));
    } else if (dataType &&
               dataType->getTypeName() == std::string(typeid(std::set<double>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<double>>(pyObj));
    } else {
      valSetter.setValue(getCppObjectFromPyObject<std::set<int>>(pyObj));
    }

    return true;
  }

  if (sipCanConvertToType(pyObj, sipFindType("std::set<double>"), SIP_NOT_NONE)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(std::set<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<float>>(pyObj));
    } else {
      valSetter.setValue(getCppObjectFromPyObject<std::set<double>>(pyObj));
    }

    return true;
  }

  CHECK_SIP_ENUM_CONVERSION("tlp::NodeShape::NodeShapes")
  CHECK_SIP_ENUM_CONVERSION("tlp::EdgeShape::EdgeShapes")
  CHECK_SIP_ENUM_CONVERSION("tlp::EdgeExtremityShape::EdgeExtremityShapes")
  CHECK_SIP_ENUM_CONVERSION("tlp::LabelPosition::LabelPositions")

  CHECK_SIP_TYPE_CONVERSION(std::string, "std::string")
  CHECK_SIP_TYPE_CONVERSION(tlp::node, "tlp::node")
  CHECK_SIP_TYPE_CONVERSION(tlp::edge, "tlp::edge")
  CHECK_SIP_TYPE_CONVERSION(tlp::Coord, "tlp::Coord")
  CHECK_SIP_TYPE_CONVERSION(tlp::Size, "tlp::Size")
  CHECK_SIP_TYPE_CONVERSION(tlp::Vec3f, "tlp::Vec3f")
  CHECK_SIP_TYPE_CONVERSION(tlp::Color, "tlp::Color")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::Graph, "tlp::Graph")
  CHECK_SIP_TYPE_CONVERSION(tlp::DataSet, "tlp::DataSet")
  CHECK_SIP_TYPE_CONVERSION(tlp::ColorScale, "tlp::ColorScale")
  CHECK_SIP_TYPE_CONVERSION(tlp::StringCollection, "tlp::StringCollection")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::BooleanProperty, "tlp::BooleanProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::ColorProperty, "tlp::ColorProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::DoubleProperty, "tlp::DoubleProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::IntegerProperty, "tlp::IntegerProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::NumericProperty, "tlp::NumericProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::LayoutProperty, "tlp::LayoutProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::SizeProperty, "tlp::SizeProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::StringProperty, "tlp::StringProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::BooleanVectorProperty, "tlp::BooleanVectorProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::ColorVectorProperty, "tlp::ColorVectorProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::DoubleVectorProperty, "tlp::DoubleVectorProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::IntegerVectorProperty, "tlp::IntegerVectorProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::CoordVectorProperty, "tlp::CoordVectorProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::SizeVectorProperty, "tlp::SizeVectorProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::StringVectorProperty, "tlp::StringVectorProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::PropertyInterface, "tlp::PropertyInterface")

  CHECK_SIP_VECTOR_LIST_CONVERSION(std::string, "std::string")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::node, "tlp::node")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::edge, "tlp::edge")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Coord, "tlp::Coord")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Color, "tlp::Color")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Size, "tlp::Size")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Graph *, "tlp::Graph*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::DataSet, "tlp::DataSet")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::ColorScale, "tlp::ColorScale")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::StringCollection, "tlp::StringCollection")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::BooleanProperty *, "tlp::BooleanProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::ColorProperty *, "tlp::ColorProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::DoubleProperty *, "tlp::DoubleProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::IntegerProperty *, "tlp::IntegerProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::LayoutProperty *, "tlp::LayoutProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::SizeProperty *, "tlp::SizeProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::StringProperty *, "tlp::StringProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::PropertyInterface *, "tlp::PropertyInterface*")

  CHECK_SIP_TYPE_CONVERSION(std::set<std::string>, "std::set<std::string>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::node>, "std::set<tlp::node>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::edge>, "std::set<tlp::edge>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::Coord>, "std::set<tlp::Coord>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::Color>, "std::set<tlp::Color>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::Size>, "std::set<tlp::Size>")

  return false;
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
