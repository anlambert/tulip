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

#include <talipot/Graph.h>
#include <talipot/Iterator.h>
#include <talipot/TlpTools.h>

#include "CppUnitIncludes.h"

using namespace std;

class GraphTraversalTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(GraphTraversalTest);
  CPPUNIT_TEST(testBFS);
  CPPUNIT_TEST(testDFS);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {
    graph = tlp::newGraph();
    nodes = graph->addNodes(13);
    graph->addEdges({{nodes[0], nodes[1]},
                     {nodes[0], nodes[7]},
                     {nodes[0], nodes[8]},
                     {nodes[0], nodes[9]},
                     {nodes[1], nodes[2]},
                     {nodes[1], nodes[3]},
                     {nodes[1], nodes[6]},
                     {nodes[3], nodes[4]},
                     {nodes[4], nodes[5]},
                     {nodes[9], nodes[10]},
                     {nodes[9], nodes[12]},
                     {nodes[10], nodes[11]}});
  }

  void tearDown() {
    delete graph;
  }

  void testDFS() {
    /*      O________
     *     /   \  \  \
     *    1___  7  8  9
     *   / \  \      / \
     *  2   3  6    10 12
     *      |        |
     *      4       11
     *      |
     *      5
     */
    vector<tlp::node> dfs = {nodes[0], nodes[1], nodes[2], nodes[3],  nodes[4],  nodes[5], nodes[6],
                             nodes[7], nodes[8], nodes[9], nodes[10], nodes[11], nodes[12]};
    CPPUNIT_ASSERT_EQUAL(dfs, iteratorVector(graph->dfs()));
  }

  void testBFS() {
    /*      O________
     *     /   \  \  \
     *    1___  2  3  4
     *   / \  \      / \
     *  5   6  7    8   9
     *      |       |
     *     10      11
     *      |
     *     12
     */
    vector<tlp::node> bfs = {nodes[0], nodes[1],  nodes[7], nodes[8],  nodes[9],
                             nodes[2], nodes[3],  nodes[6], nodes[10], nodes[12],
                             nodes[4], nodes[11], nodes[5]};
    CPPUNIT_ASSERT_EQUAL(bfs, iteratorVector(graph->bfs()));
  }

private:
  tlp::Graph *graph;
  std::vector<tlp::node> nodes;
};

CPPUNIT_TEST_SUITE_REGISTRATION(GraphTraversalTest);
