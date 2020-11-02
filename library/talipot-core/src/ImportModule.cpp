/**
 *
 * Copyright (C) 2020  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <talipot/ImportModule.h>
#include <talipot/TlpTools.h>
#include <talipot/PluginProgress.h>

#include <cerrno>
#include <cstring>

using namespace std;
using namespace tlp;

// Function to retrieve the original size of a file compressed with gzip.
// The uncompressed file size (modulo 2^32) is stored in the last four bytes of a Gzip file.
// So that method is unreliable if the original file size was greater than 4GB
// (which is pretty rare for Talipot supported files ...).
static int getUncompressedSizeOfGzipFile(const string &gzipFilePath) {
  istream *is = getInputFileStream(gzipFilePath);
  is->seekg(-4, is->end);
  int uncompressedSize = 0;
  is->read(reinterpret_cast<char *>(&uncompressedSize), 4);
  delete is;
  return uncompressedSize;
}

ImportModule::InputData ImportModule::getInputData() const {
  istream *input = nullptr;
  size_t size = 0;
  string filename;
  if (dataSet->exists("file::filename")) {
    dataSet->get("file::filename", filename);
    tlp_stat_t infoEntry;
    bool pathExists = (statPath(filename, &infoEntry) == 0);

    if (!pathExists) {
      stringstream ess;
      ess << filename << ": " << strerror(errno);
      pluginProgress->setError(ess.str());
      error() << pluginProgress->getError() << endl;
      return InputData();
    }

    size = infoEntry.st_size;

    bool gzip = false;
    bool zstd = false;

    for (const auto &gezxt : gzipFileExtensions()) {
      if (filename.rfind(gezxt) == (filename.length() - gezxt.length())) {
        size = getUncompressedSizeOfGzipFile(filename);
        input = getZlibInputFileStream(filename);
        gzip = true;
        break;
      }
    }

    for (const auto &zsext : zstdFileExtensions()) {
      if (filename.rfind(zsext) == (filename.length() - zsext.length())) {
        input = getZstdInputFileStream(filename);
        zstd = true;
        break;
      }
    }

    if (!gzip && !zstd) {
      input = getInputFileStream(filename);
    }

  } else if (dataSet->exists("file::data")) {
    string data;
    dataSet->get("file::data", data);
    size = data.size();
    auto tmpss = new stringstream;
    *tmpss << data;
    input = tmpss;
  } else {
    pluginProgress->setError("No file to open: 'file::filename' parameter is missing");
    error() << pluginProgress->getError() << std::endl;
    return InputData();
  }
  return InputData(input, size, filename);
}