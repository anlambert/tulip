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

#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <ctime>

#include <talipot/ExportModule.h>
#include <talipot/Graph.h>
#include <talipot/GraphProperty.h>
#include <talipot/TlpTools.h>
#include <talipot/MutableContainer.h>
#include <talipot/StringCollection.h>
#include <talipot/Release.h>
#include <talipot/PropertyTypes.h>
#include <talipot/StableIterator.h>

#define TLP_FILE_VERSION "2.3"

using namespace std;
using namespace tlp;

static string convert(const string &s) {
  string newStr;

  for (char c : s) {
    if (c == '\"') {
      newStr += "\\\"";
    } else if (c == '\n') {
      newStr += "\\n";
    } else if (c == '\\') {
      newStr += "\\\\";
    } else {
      newStr += c;
    }
  }

  return newStr;
}

static const char *paramHelp[] = {
    // name
    "Name of the graph being exported.",

    // author
    "Authors",

    // comments
    "Description of the graph."};

namespace tlp {
/**
 * This plugin records a Talipot graph structure in a file using the TLP format.
 * TLP is the Tulip Software Graph Format.
 */
class TLPExport : public ExportModule {
public:
  PLUGININFORMATION("TLP Export", "Auber David", "31/07/2001",
                    "<p>Supported extensions: tlp, tlpz (compressed), tlp.gz "
                    "(compressed)</p><p>Exports a graph in a file using the TLP format (Tulip "
                    "Software Graph Format).<br/>See <b>http://tulip.labri.fr->Framework->TLP File "
                    "Format</b> for more details.</p>",
                    "1.1", "File")

  string fileExtension() const override {
    return "tlp";
  }

  DataSet controller;
  int progress;

  TLPExport(tlp::PluginContext *context) : ExportModule(context), progress(0) {
    addInParameter<string>("name", paramHelp[0], "");
    addInParameter<string>("author", paramHelp[1], "");
    addInParameter<string>("text::comments", paramHelp[2], "This file was generated by Talipot.");
  }
  //=====================================================
  ~TLPExport() override = default;
  //=====================================================
  std::string icon() const override {
    return ":/talipot/gui/icons/logo32x32.png";
  }
  //====================================================
  node getNode(node n) {
    return node(graph->nodePos(n));
  }
  //====================================================
  edge getEdge(edge e) {
    return edge(graph->edgePos(e));
  }
  //=====================================================
  void saveGraphElements(ostream &os, Graph *g) {
    pluginProgress->setComment("Saving Graph Elements");
    pluginProgress->progress(progress, g->numberOfEdges() + g->numberOfNodes());

    if (g->getSuperGraph() != g) {
      os << "(cluster " << g->getId() << endl;

      const std::vector<node> &nodes = g->nodes();
      unsigned int nbNodes = nodes.size();
      const std::vector<edge> &edges = g->edges();
      unsigned int nbEdges = edges.size();
      node beginNode, previousNode;
      unsigned int progupdate = 1 + (nbNodes + nbEdges) / 100;

      if (nbNodes) {
        os << "(nodes";

        for (unsigned int i = 0; i < nbNodes; ++i) {
          if (progress % progupdate == 0) {
            pluginProgress->progress(progress, nbNodes + nbEdges);
          }

          ++progress;
          node current = getNode(nodes[i]);

          if (!beginNode.isValid()) {
            beginNode = previousNode = current;
            os << " " << beginNode.id;
          } else {
            if (current.id == previousNode.id + 1) {
              previousNode = current;

              if (i == nbNodes - 1) {
                os << ".." << current.id;
              }
            } else {
              if (previousNode != beginNode) {
                os << ".." << previousNode.id;
              }

              os << " " << current.id;
              beginNode = previousNode = current;
            }
          }
        }

        os << ")" << endl;
      }

      edge beginEdge, previousEdge;

      if (nbEdges) {
        os << "(edges";

        for (unsigned int i = 0; i < nbEdges; ++i) {
          if (progress % progupdate == 0) {
            pluginProgress->progress(progress, nbNodes + nbEdges);
          }

          ++progress;
          edge current = getEdge(edges[i]);

          if (!beginEdge.isValid()) {
            beginEdge = previousEdge = current;
            os << " " << beginEdge.id;
          } else {
            if (current.id == previousEdge.id + 1) {
              previousEdge = current;

              if (i == nbEdges - 1) {
                os << ".." << current.id;
              }
            } else {
              if (previousEdge != beginEdge) {
                os << ".." << previousEdge.id;
              }

              os << " " << current.id;
              beginEdge = previousEdge = current;
            }
          }
        }

        os << ")" << endl;
      }
    } else {
      unsigned int nbElts = g->numberOfNodes();

      os << "(nb_nodes " << nbElts << ")" << endl;

      os << ";(nodes <node_id> <node_id> ...)" << endl;

      switch (nbElts) {
      case 0:
        os << "(nodes)" << endl;
        break;

      case 1:
        os << "(nodes 0)" << endl;
        break;

      case 2:
        os << "(nodes 0 1)" << endl;
        break;

      default:
        os << "(nodes 0.." << nbElts - 1 << ")" << endl;
      }

      nbElts = g->numberOfEdges();
      os << "(nb_edges " << nbElts << ")" << endl;

      os << ";(edge <edge_id> <source_id> <target_id>)" << endl;
      unsigned int progupdate = 1 + nbElts / 100;
      const std::vector<edge> &edges = g->edges();

      for (unsigned i = 0; i < nbElts; ++i) {
        if (progress % progupdate == 0) {
          pluginProgress->progress(progress, nbElts);
        }

        ++progress;
        edge e = edges[i];
        const auto &[src, tgt] = g->ends(e);
        os << "(edge " << i << " " << getNode(src).id << " " << getNode(tgt).id << ")";

        if (i != nbElts - 1) {
          os << endl;
        }
      }

      os << endl;
    }

    for (Graph *sg : g->subGraphs()) {
      saveGraphElements(os, sg);
    }

    if (g->getSuperGraph() != g) {
      os << ")" << endl;
    }
  }
  //=====================================================
  void saveLocalProperties(ostream &os, Graph *g) {
    pluginProgress->setComment("Saving Graph Properties");
    progress = 0;
    Iterator<PropertyInterface *> *itP = nullptr;

    if (g->getSuperGraph() == g) {
      itP = g->getObjectProperties();
    } else {
      itP = g->getLocalObjectProperties();
    }

    int nonDefaultvaluatedElementCount = 1;

    for (PropertyInterface *prop : itP) {
      nonDefaultvaluatedElementCount += prop->numberOfNonDefaultValuatedNodes();
      nonDefaultvaluatedElementCount += prop->numberOfNonDefaultValuatedEdges();
    }

    if (g->getSuperGraph() == g) {
      itP = g->getObjectProperties();
    } else {
      itP = g->getLocalObjectProperties();
    }

    for (PropertyInterface *prop : itP) {
      stringstream tmp;
      tmp << "Saving Property [" << prop->getName() << "]";
      pluginProgress->setComment(tmp.str());

      if (g->getSuperGraph() == g) {
        os << "(property "
           << " 0 " << prop->getTypename() << " ";
      } else {
        os << "(property "
           << " " << g->getId() << " " << prop->getTypename() << " ";
      }

      os << "\"" << convert(prop->getName()) << "\"" << endl;
      string nDefault = prop->getNodeDefaultStringValue();
      string eDefault = prop->getEdgeDefaultStringValue();

      bool isPathViewProp =
          (prop->getName() == string("viewFont") || prop->getName() == string("viewTexture"));

      // replace real path with symbolic one using TalipotBitmapDir
      if (isPathViewProp && !TalipotBitmapDir.empty()) {
        if (size_t pos = nDefault.find(TalipotBitmapDir); pos != string::npos) {
          nDefault.replace(pos, TalipotBitmapDir.size(), "TalipotBitmapDir/");
        }

        if (size_t pos = eDefault.find(TalipotBitmapDir); pos != string::npos) {
          eDefault.replace(pos, TalipotBitmapDir.size(), "TalipotBitmapDir/");
        }
      }

      os << "(default \"" << convert(nDefault) << "\" \"" << convert(eDefault) << "\")" << endl;

      for (auto n : prop->getNonDefaultValuatedNodes(g)) {

        if (progress % (1 + nonDefaultvaluatedElementCount / 100) == 0) {
          pluginProgress->progress(progress, nonDefaultvaluatedElementCount);
        }

        ++progress;
        string tmp = prop->getNodeStringValue(n);

        // replace real path with symbolic one using TalipotBitmapDir
        if (isPathViewProp && !TalipotBitmapDir.empty()) {
          if (size_t pos = tmp.find(TalipotBitmapDir); pos != string::npos) {
            tmp.replace(pos, TalipotBitmapDir.size(), "TalipotBitmapDir/");
          }
        } else if (g->getId() != 0 && // if it is not the real root graph
                   prop->getTypename() == GraphProperty::propertyTypename) {
          unsigned int id = strtoul(tmp.c_str(), nullptr, 10);

          // we must check if the pointed subgraph
          // is a descendant of the currently export graph
          if (!graph->getDescendantGraph(id)) {
            continue;
          }
        }

        os << "(node " << getNode(n).id << " \"" << convert(tmp) << "\")" << endl;
      }

      for (auto e : prop->getNonDefaultValuatedEdges(g)) {

        if (progress % (1 + nonDefaultvaluatedElementCount / 100) == 0) {
          pluginProgress->progress(progress, nonDefaultvaluatedElementCount);
        }

        ++progress;

        if (prop->getTypename() == GraphProperty::propertyTypename) {

          // for GraphProperty we must ensure the reindexing
          // of embedded edges
          const set<edge> &edges = static_cast<GraphProperty *>(prop)->getEdgeValue(e);
          set<edge> rEdges;

          for (auto ee : edges) {
            edge rEdge = getEdge(ee);
            // do not export edges that are not elements of the root graph
            if (rEdge.isValid()) {
              rEdges.insert(rEdge);
            }
          }

          if (!rEdges.empty()) {
            // finally save the vector
            os << "(edge " << getEdge(e).id << " \"";
            EdgeSetType::write(os, rEdges);
            os << "\")" << endl;
          }

        } else {

          // replace real path with symbolic one using TalipotBitmapDir
          string tmp = prop->getEdgeStringValue(e);

          if (isPathViewProp && !TalipotBitmapDir.empty()) {
            if (size_t pos = tmp.find(TalipotBitmapDir); pos != string::npos) {
              tmp.replace(pos, TalipotBitmapDir.size(), "TalipotBitmapDir/");
            }
          }

          os << "(edge " << getEdge(e).id << " \"" << convert(tmp) << "\")" << endl;
        }
      }
      os << ")" << endl;
    }
  }
  //=====================================================
  void saveProperties(ostream &os, Graph *g) {
    saveLocalProperties(os, g);

    for (Graph *sg : g->subGraphs()) {
      saveProperties(os, sg);
    }
  }
  //=====================================================
  void saveAttributes(ostream &os, Graph *g) {
    const DataSet &attributes = g->getAttributes();

    if (!attributes.empty()) {

      // If nodes and edges are stored as graph attributes
      // we need to update their id before serializing them
      // as nodes and edges have been reindexed
      for (const auto &[key, value] : attributes.getValues()) {
        if (value->getTypeName() == string(typeid(node).name())) {
          node *n = static_cast<node *>(value->value);
          n->id = getNode(*n).id;
        } else if (value->getTypeName() == string(typeid(edge).name())) {
          edge *e = static_cast<edge *>(value->value);
          e->id = getEdge(*e).id;
        } else if (value->getTypeName() == string(typeid(vector<node>).name())) {
          auto *vn = static_cast<vector<node> *>(value->value);

          for (auto n : *vn) {
            n.id = getNode(n).id;
          }
        } else if (value->getTypeName() == string(typeid(vector<edge>).name())) {
          auto *ve = static_cast<vector<edge> *>(value->value);

          for (auto e : *ve) {
            e.id = getEdge(e).id;
          }
        }
      }

      if (g->getSuperGraph() == g) {
        os << "(graph_attributes 0 ";
      } else {
        os << "(graph_attributes " << g->getId() << " ";
      }

      DataSet::write(os, attributes);
      os << ")" << endl;
    }

    // save subgraph attributes

    for (Graph *sg : g->subGraphs()) {
      saveAttributes(os, sg);
    }
  }
  //=====================================================
  void saveController(ostream &os, DataSet &data) {
    os << "(controller ";
    DataSet::write(os, data);
    os << ")" << endl;
  }

  bool exportGraph(ostream &os) override {

    // change graph parent in hierarchy temporarily to itself as
    // it will be the new root of the exported hierarchy
    Graph *superGraph = graph->getSuperGraph();
    graph->setSuperGraph(graph);

    string format(TLP_FILE_VERSION);

    string name;
    string author;
    string comments = "This file was generated by Talipot.";

    if (dataSet != nullptr) {
      dataSet->get("name", name);
      dataSet->get("author", author);
      dataSet->get("text::comments", comments);
    }

    if (name.length() > 0) {
      graph->setAttribute("name", name);
    }

    // get ostime
    time_t ostime = time(nullptr);
    // get local time
    struct tm *currTime = localtime(&ostime);
    // format date
    char currDate[32];
    strftime(currDate, 32, "%m-%d-%Y", currTime);

    // output tlp format version
    os << "(tlp \"" << format.c_str() << '"' << endl;
    // current date
    os << "(date \"" << currDate << "\")" << endl;

    // author
    if (author.length() > 0) {
      os << "(author \"" << author << "\")" << endl;
    }

    // comments
    os << "(comments \"" << comments << "\")" << endl;

    saveGraphElements(os, graph);
    saveProperties(os, graph);
    saveAttributes(os, graph);

    // Save views
    if (dataSet != nullptr && dataSet->get<DataSet>("controller", controller)) {
      saveController(os, controller);
    }

    os << ')' << endl; // end of (tlp ...

    // restore original parent in hierarchy
    graph->setSuperGraph(superGraph);
    return true;
  }
};
PLUGIN(TLPExport)
}
