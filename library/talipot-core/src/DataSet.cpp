/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <string>
#include <talipot/PropertyTypes.h>
#include <talipot/Graph.h>
#include <talipot/BooleanProperty.h>
#include <talipot/DoubleProperty.h>
#include <talipot/GraphProperty.h>
#include <talipot/IntegerProperty.h>
#include <talipot/LayoutProperty.h>
#include <talipot/Color.h>
#include <talipot/ColorProperty.h>
#include <talipot/Size.h>
#include <talipot/SizeProperty.h>
#include <talipot/StringProperty.h>
#include <talipot/TlpTools.h>

#include <talipot/DataSet.h>

using namespace std;
using namespace tlp;

bool DataType::isTalipotProperty(const std::string &typeName) {
#define ISPROP(T) typeName.compare(typeid(T).name()) == 0
  return (ISPROP(tlp::BooleanProperty *) || ISPROP(tlp::BooleanVectorProperty *) ||
          ISPROP(tlp::DoubleProperty *) || ISPROP(tlp::DoubleVectorProperty *) ||
          ISPROP(tlp::LayoutProperty *) || ISPROP(tlp::CoordVectorProperty *) ||
          ISPROP(tlp::StringProperty *) || ISPROP(tlp::StringVectorProperty *) ||
          ISPROP(tlp::IntegerProperty *) || ISPROP(tlp::IntegerVectorProperty *) ||
          ISPROP(tlp::SizeProperty *) || ISPROP(tlp::SizeVectorProperty *) ||
          ISPROP(tlp::ColorProperty *) || ISPROP(tlp::ColorVectorProperty *) ||
          ISPROP(tlp::NumericProperty *) || ISPROP(tlp::PropertyInterface *) ||
          ISPROP(tlp::GraphProperty *) || ISPROP(tlp::BooleanProperty) ||
          ISPROP(tlp::BooleanVectorProperty) || ISPROP(tlp::DoubleProperty) ||
          ISPROP(tlp::DoubleVectorProperty) || ISPROP(tlp::LayoutProperty) ||
          ISPROP(tlp::CoordVectorProperty) || ISPROP(tlp::StringProperty) ||
          ISPROP(tlp::StringVectorProperty) || ISPROP(tlp::IntegerProperty) ||
          ISPROP(tlp::IntegerVectorProperty) || ISPROP(tlp::SizeProperty) ||
          ISPROP(tlp::SizeVectorProperty) || ISPROP(tlp::ColorProperty) ||
          ISPROP(tlp::ColorVectorProperty) || ISPROP(tlp::NumericProperty) ||
          ISPROP(tlp::PropertyInterface) || ISPROP(tlp::GraphProperty));
}

DataSet::DataSet(const DataSet &set) {
  *this = set;
}

DataSet &DataSet::operator=(const DataSet &set) {
  if (this != &set) {
    data.clear();

    for (std::list<std::pair<std::string, tlp::DataType *>>::const_iterator it = set.data.begin();
         it != set.data.end(); ++it) {
      data.push_back(std::pair<std::string, tlp::DataType *>((*it).first, (*it).second->clone()));
    }
  }

  return *this;
}

DataSet::~DataSet() {
  for (std::list<std::pair<std::string, tlp::DataType *>>::iterator it = data.begin();
       it != data.end(); ++it) {
    if (it->second)
      delete it->second;
  }
}

bool DataSet::exists(const string &str) const {
  for (std::list<std::pair<std::string, tlp::DataType *>>::const_iterator it = data.begin();
       it != data.end(); ++it) {
    if ((*it).first == str)
      return true;
  }

  return false;
}

std::string DataSet::getTypeName(const string &str) const {
  for (std::list<std::pair<std::string, tlp::DataType *>>::const_iterator it = data.begin();
       it != data.end(); ++it) {
    if (it->first == str)
      return it->second->getTypeName();
  }

  return std::string();
}

void DataSet::remove(const string &str) {
  for (std::list<std::pair<std::string, tlp::DataType *>>::iterator it = data.begin();
       it != data.end(); ++it) {
    if ((*it).first == str) {
      if (it->second)
        delete it->second;

      data.erase(it);
      break;
    }
  }
}

DataType *DataSet::getData(const string &str) const {
  for (std::list<std::pair<std::string, tlp::DataType *>>::const_iterator it = data.begin();
       it != data.end(); ++it) {
    if ((*it).first == str)
      return it->second ? it->second->clone() : nullptr;
  }

  return nullptr;
}

void DataSet::setData(const std::string &str, const DataType *value) {
  DataType *val = value ? value->clone() : nullptr;

  for (std::list<std::pair<std::string, tlp::DataType *>>::iterator it = data.begin();
       it != data.end(); ++it) {
    std::pair<std::string, tlp::DataType *> &p = *it;

    if (p.first == str) {
      if (p.second)
        delete p.second;

      p.second = val;
      return;
    }
  }

  data.push_back(std::pair<std::string, tlp::DataType *>(str, val));
}

unsigned int DataSet::size() const {
  return uint(data.size());
}

bool DataSet::empty() const {
  return data.empty();
}

Iterator<pair<string, DataType *>> *DataSet::getValues() const {
  list<pair<string, DataType *>>::const_iterator begin = data.begin();
  list<pair<string, DataType *>>::const_iterator end = data.end();

  return new StlIterator<pair<string, DataType *>, list<pair<string, DataType *>>::const_iterator>(
      begin, end);
}

// management of the serialization
// the 2 hash maps

DataTypeSerializerContainer DataSet::serializerContainer;

// registering of a data type serializer
void DataSet::registerDataTypeSerializer(const std::string &typeName, DataTypeSerializer *dts) {

#ifndef NDEBUG
  std::unordered_map<std::string, DataTypeSerializer *>::iterator it =
      serializerContainer.tnTodts.find(typeName);

  if (it != serializerContainer.tnTodts.end())
    tlp::warning() << "Warning: a data type serializer is already registered for type "
                   << demangleClassName(typeName.c_str()).c_str() << std::endl;

  it = serializerContainer.otnTodts.find(dts->outputTypeName);

  if (it != serializerContainer.otnTodts.end())
    tlp::warning() << "Warning: a data type serializer is already registered for read type "
                   << dts->outputTypeName << std::endl;

#endif

  serializerContainer.tnTodts[typeName] = serializerContainer.otnTodts[dts->outputTypeName] = dts;
}

// data write
void DataSet::writeData(std::ostream &os, const std::string &prop, const DataType *dt) const {
  std::unordered_map<std::string, DataTypeSerializer *>::iterator it =
      serializerContainer.tnTodts.find(dt->getTypeName());

  if (it == serializerContainer.tnTodts.end()) {
#ifndef EMSCRIPTEN
    tlp::warning() << "Write error: No data serializer found for type "
                   << demangleClassName(dt->getTypeName().c_str()).c_str() << std::endl;
#endif
    return;
  }

  DataTypeSerializer *dts = (*it).second;
  os << '(' << dts->outputTypeName << " \"" << prop << "\" ";
  dts->writeData(os, dt);
  os << ')' << endl;
}

void DataSet::write(std::ostream &os, const DataSet &ds) {
  os << endl;
  // iterate over pair attribute/value
  for (const pair<string, DataType *> &p : ds.getValues()) {
    ds.writeData(os, p.first, p.second);
  }
}

// data read
bool DataSet::readData(std::istream &is, const std::string &prop,
                       const std::string &outputTypeName) {
  std::unordered_map<std::string, DataTypeSerializer *>::iterator it =
      serializerContainer.otnTodts.find(outputTypeName);

  if (it == serializerContainer.otnTodts.end()) {
    tlp::warning() << "Read error: No data type serializer found for read type " << outputTypeName
                   << std::endl;
    return false;
  }

  DataTypeSerializer *dts = (*it).second;
  DataType *dt = dts->readData(is);

  if (dt) {
    // replace any preexisting value associated to prop
    for (std::list<std::pair<std::string, tlp::DataType *>>::iterator it = data.begin();
         it != data.end(); ++it) {
      std::pair<std::string, tlp::DataType *> &p = *it;

      if (p.first == prop) {
        if (p.second)
          delete p.second;

        p.second = dt;
        return true;
      }
    }

    // no preexisting value
    data.push_back(std::pair<std::string, tlp::DataType *>(prop, dt));
    return true;
  }

  return false;
}

bool DataSet::read(std::istream &is, DataSet &ds) {
  is.unsetf(ios_base::skipws);

  for (;;) {
    char c;

    if (!(is >> c)) {
      return is.eof();
    }

    if (isspace(c))
      continue;

    if (c == ')') {
      // no open paren at the beginning
      // so the close paren must be read by the caller
      is.unget();
      return true;
    }

    if (c == '(') {
      bool ok;

      // skip spaces before output type name
      while ((ok = bool(is >> c)) && isspace(c)) {
      }

      if (!ok)
        return false;

      string otn;

      // read output type name until next space char
      do {
        otn.push_back(c);
      } while ((ok = bool(is >> c)) && !isspace(c));

      // skip spaces before prop
      while ((ok = bool(is >> c)) && isspace(c)) {
      }

      if (!ok)
        return false;

      if (c != '"')
        return false;

      string prop;

      // read prop until next "
      while ((ok = bool(is >> c)) && (c != '"')) {
        prop.push_back(c);
      }

      if (!ok)
        return false;

      // skip spaces before data type
      while ((ok = bool(is >> c)) && isspace(c)) {
      }

      if (!ok)
        return false;

      is.unget();

      // read data type
      if (!ds.readData(is, prop, otn))
        return false;

      // skip spaces before )
      while ((ok = bool(is >> c)) && isspace(c)) {
      }

      if (!ok)
        return false;

      if (c != ')')
        return false;

    } else
      return false;
  }
}

DataTypeSerializer *DataSet::typenameToSerializer(const std::string &name) {
  if (serializerContainer.tnTodts.count(name) == 0)
    return nullptr;

  return serializerContainer.tnTodts[name];
}

string DataSet::toString() const {
  stringstream ss;
  for (const pair<string, DataType *> &p : getValues()) {
    DataTypeSerializer *serializer = DataSet::typenameToSerializer(p.second->getTypeName());

    if (serializer) {
      ss << "'" << p.first << "'=";
      ss << serializer->toString(p.second).c_str();
      ss << " ";
    } else {
      if (p.second->isTalipotProperty()) {
        PropertyInterface *prop = *(static_cast<PropertyInterface **>(p.second->value));
        ss << "'" << p.first << "'=";

        if (prop)
          ss << '"' << prop->getName().c_str() << '"';
        else
          ss << "None";

        ss << " ";
      }
    }
  }
  return ss.str();
}
