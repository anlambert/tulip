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

#include <cstring>
#include <cerrno>
#include <talipot/PluginHeaders.h>
#include "dotImportCLUT.h"

using namespace std;
using namespace tlp;

#ifdef _WIN32
#define uint unsigned int
#include <utf8.h>
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
namespace {
#include "dotImportParser.h"
#include "dotImportLexer.h"
} // namespace
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

static const char *paramHelp[] = {
    // filename
    "The dot file to import."};

/** \addtogroup import */

/** This plugin enables to import a graph coded with in dot format
 *
 *  File format:
 *     [ http://www.research.att.com/sw/tools/graphviz/ ]
 *
 *  First (quick) support of the AT&T DOT language
 *    - main graph entities are extracted (node/edges)
 *    - subgraphs are not already supported
 *    - several attributes (node & edge) are supported
 *    - based on a modified grammar file available with the graphviz software
 *    - this parser can be largely optimized ...
 *
 */
class DotImport : public ImportModule {
public:
  PLUGININFORMATION("graphviz", "Gerald Gainant", "01/03/2004",
                    "<p>Supported extensions: dot</p><p>Imports a new graph from a file in the dot "
                    "input format.</p>",
                    "1.0", "File")
  std::list<std::string> fileExtensions() const override {
    std::list<std::string> l;
    l.push_back("dot");
    return l;
  }
  DotImport(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<string>("file::filename", paramHelp[0], "");
  }
  ~DotImport() override {}

  std::string icon() const override {
    return ":/talipot/app/icons/32/import_graphviz.png";
  }

  bool importGraph() override {

    // Open input stream
    string fn;
    dataSet->get("file::filename", fn);
#ifndef WIN32
    FILE *fd = fopen(fn.c_str(), "r");
#else
    std::u16string wfn = utf8::utf8to16(fn);
    FILE *fd = _wfopen(reinterpret_cast<const wchar_t *>(wfn.c_str()), L"r");
#endif

    if (!fd) {
      if (pluginProgress) {
        pluginProgress->setError(strerror(errno));
      }

      return false;
    }

    // Create & Init YY global data
    DOT_YY _dotyy(fd, graph, pluginProgress);

    if (pluginProgress) {
      pluginProgress->showPreview(false);
      pluginProgress->progress(1, 100000);
    }

    dotyy = &_dotyy;
    yyrestart(fd);
    yyparse();

    fclose(fd);

    return _dotyy.pStatus != TLP_CANCEL;
  }
};

PLUGIN(DotImport)
