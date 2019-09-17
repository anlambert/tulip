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

#include <cmath>
#include <talipot/ImportModule.h>
#include <talipot/TlpTools.h>
#include <talipot/PluginProgress.h>
#include <talipot/Graph.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // n
    "Number of nodes.",

    // m
    "Number of nodes added at each time step.",

    // p
    "p defines the probality a new node is wired to an existing one"};

/**
 *
 * This plugin is an implementation of the model
 * described in
 * Michele Catanzaro, Guido Caldarelli, and Luciano Pietronero.
 * Assortative model for social networks. Physical Review E (Statistical, Nonlinear, and Soft Matter
 * Physics), 70(3), (2004).
 *
 */
struct Catanzaro : public ImportModule {
  PLUGININFORMATION("Catanzaro and al. Model", "Arnaud Sallaberry", "21/02/2011",
                    "Randomly generates a graph using the model described in<br/>Michele "
                    "Catanzaro, Guido Caldarelli, and Luciano Pietronero.<br/><b>Assortative model "
                    "for social networks.</b><br/>Physical Review E (Statistical, Nonlinear, and "
                    "Soft Matter Physics), 70(3), (2004).",
                    "1.0", "Social network")

  Catanzaro(PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "300");
    addInParameter<unsigned int>("m", paramHelp[1], "5");
    addInParameter<double>("p", paramHelp[2], "0.5");
  }

  bool importGraph() override {
    unsigned int n = 300;
    unsigned int m = 5;
    double p = 0.5;

    if (dataSet != nullptr) {
      dataSet->get("nodes", n);
      dataSet->get("m", m);
      dataSet->get("p", p);
    }

    // check arguments
    if (m > n) {
      pluginProgress->setError("The m parameter cannot be greater than the number of nodes");
      return false;
    } else if (p < 0 || p > 1) {
      pluginProgress->setError("p is not a probability,\nit does not belong to [0, 1]");
      return false;
    }

    pluginProgress->showPreview(false);
    tlp::initRandomSequence();

    unsigned int i, j;

    /*
     * Initial ring construction
     */
    graph->addNodes(n);
    const vector<node> &nodes = graph->nodes();

    for (i = 1; i < 3; ++i) {
      graph->addEdge(nodes[i - 1], nodes[i]);
    }

    graph->addEdge(nodes[2], nodes[0]);

    /*
     * Main loop
     */
    for (i = 3; i < n; ++i) {
      double k_sum = 0;

      for (j = 0; j < i; j++) {
        k_sum += graph->deg(nodes[j]);
      }

      for (j = 0; j < m; j++) {

        double pr = tlp::randomDouble();
        double pr_sum = 0;
        unsigned int u = 0;

        while (pr_sum < pr && u < (i - 1)) {
          pr_sum += graph->deg(nodes[u]) / (k_sum + j);
          ++u;
        }

        if (tlp::randomDouble() <= p) { // PA
          if (!graph->hasEdge(nodes[i], nodes[u], false))
            graph->addEdge(nodes[i], nodes[u]);
        } else {
          pr_sum = 0;
          k_sum = 0;
          unsigned int k, l = 0;

          for (k = 0; k < i; ++k) {
            for (l = 0; l < k; ++l) {
              k_sum += graph->deg(nodes[k]) / (k_sum + j) *
                       exp(-fabs(double(graph->deg(nodes[k])) - double(graph->deg(nodes[l]))));
            }
          }

          pr = tlp::randomDouble(ceil(k_sum));

          for (k = 0; k < i; ++k) {
            for (l = 0; l < k; ++l) {
              pr_sum += graph->deg(nodes[k]) / (k_sum + j) *
                        exp(-fabs(double(graph->deg(nodes[k])) - double(graph->deg(nodes[l]))));
            }

            if (pr_sum > pr) {
              break;
            }
          }

          if (!graph->hasEdge(nodes[l], nodes[k], false))
            graph->addEdge(nodes[l], nodes[k]);
        }
      }
    }

    return true;
  }
};

PLUGIN(Catanzaro)
