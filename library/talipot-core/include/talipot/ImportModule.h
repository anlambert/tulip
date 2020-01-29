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

#ifndef TALIPOT_IMPORT_MODULE_H
#define TALIPOT_IMPORT_MODULE_H

#include <talipot/Plugin.h>
#include <talipot/MaterialDesignIcons.h>

#include <list>
#include <string>

namespace tlp {

static const std::string IMPORT_CATEGORY = "Import";

class PluginProgress;
class Graph;
class DataSet;

/**
 * @addtogroup Plugins
 * @brief Base class for import plug-ins.
 **/
class ImportModule : public tlp::Plugin {
public:
  ImportModule(const tlp::PluginContext *context) {
    if (context != nullptr) {
      const tlp::AlgorithmContext *algoritmContext =
          static_cast<const tlp::AlgorithmContext *>(context);
      graph = algoritmContext->graph;
      pluginProgress = algoritmContext->pluginProgress;
      dataSet = algoritmContext->dataSet;
    }
  }

  /**
   * @brief Gets the extensions of the file formats the plugin can import.
   * e.g. a TLP import would return 'tlp'.
   *
   * @return the list of file extensions the plugin can import.
   **/
  virtual std::list<std::string> fileExtensions() const {
    return std::list<std::string>();
  }

  /**
   * @brief Gets the extensions of the gzipped file formats this plugin can import.
   * e.g. a TLP import would return 'tlp.gz and tlpz'.
   *
   * @return the list of gzipped file extensions the plugin can import.
   **/
  virtual std::list<std::string> gzipFileExtensions() const {
    return std::list<std::string>();
  }

  /**
   * @brief Gets all the extensions (normal and gzipped) of the file formats this plugin can import.
   *
   * @return the list of file extensions the plugin can import.
   **/
  std::list<std::string> allFileExtensions() const {
    std::list<std::string> zext(gzipFileExtensions());
    std::list<std::string> ext(fileExtensions());
    ext.splice(ext.end(), zext);
    return ext;
  }

  std::string category() const override {
    return IMPORT_CATEGORY;
  }

  std::string icon() const override {
    return MaterialDesignIcons::Import;
  }

  /**
   * @brief The import operations should take place here.
   *
   * @return bool Whether the import was successful or not.
   **/
  virtual bool importGraph() = 0;

  /**
   * @brief The Graph in which to write the data to import.
   **/
  Graph *graph;

  /**
   * @brief A means to report progress to the user.
   **/
  PluginProgress *pluginProgress;

  /**
   * @brief A container for the parameters of this import plug-in.
   **/
  DataSet *dataSet;
};
}
#endif // TALIPOT_IMPORT_MODULE_H
