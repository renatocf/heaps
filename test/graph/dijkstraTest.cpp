/******************************************************************************/
/*   Heaps - a heap library implementing heaps to compare their performance   */
/*   Copyright (C) 2016 Renato Cordeiro Ferreira                              */
/*                                                                            */
/*   This program is free software: you can redistribute it and/or modify     */
/*   it under the terms of the GNU General Public License as published by     */
/*   the Free Software Foundation, either version 3 of the License, or        */
/*   (at your option) any later version.                                      */
/*                                                                            */
/*   This program is distributed in the hope that it will be useful,          */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/*   GNU General Public License for more details.                             */
/*                                                                            */
/*   You should have received a copy of the GNU General Public License        */
/*   along with this program.  If not, see <www.gnu.org/licenses/>.           */
/******************************************************************************/

// External headers
#include "gmock/gmock.h"

// Internal headers
#include "heap/binary.hpp"
#include "heap/fibonacci.hpp"

// Tested header
#include "graph/dijkstra.hpp"

/*----------------------------------------------------------------------------*/
/*                             USING DECLARATIONS                             */
/*----------------------------------------------------------------------------*/

using ::testing::Eq;
using ::testing::ElementsAre;

/*----------------------------------------------------------------------------*/
/*                                  FIXTURES                                  */
/*----------------------------------------------------------------------------*/

struct AnEmptyGraph : public ::testing::Test {
  graph::Graph graph;
};

struct ADirectedGraph : public ::testing::Test {
  graph::Graph graph;

  ADirectedGraph() : graph(6) {
    graph[0].push_back(graph::Edge{1, 7});   // edge 0->1 weight = 7
    graph[0].push_back(graph::Edge{2, 9});   // edge 0->2 weight = 9
    graph[0].push_back(graph::Edge{5, 14});  // edge 0->5 weight = 14
    graph[1].push_back(graph::Edge{2, 10});  // edge 1->2 weight = 10
    graph[1].push_back(graph::Edge{3, 15});  // edge 1->3 weight = 15
    graph[2].push_back(graph::Edge{5, 2});   // edge 2->5 weight = 2
    graph[2].push_back(graph::Edge{3, 11});  // edge 2->3 weight = 11
    graph[3].push_back(graph::Edge{4, 6});   // edge 3->4 weight = 6
    graph[4].push_back(graph::Edge{5, 9});   // edge 4->5 weight = 9
  }
};

struct AnUndirectedGraph : public ::testing::Test {
  graph::Graph graph;

  AnUndirectedGraph() : graph(6) {
    // edge 0->1 weight = 7
    graph[0].push_back(graph::Edge{1, 7});
    graph[1].push_back(graph::Edge{0, 7});

    // edge 0->2 weight = 9
    graph[0].push_back(graph::Edge{2, 9});
    graph[2].push_back(graph::Edge{0, 9});

    // edge 0->5 weight = 14
    graph[0].push_back(graph::Edge{5, 14});
    graph[5].push_back(graph::Edge{0, 14});

    // edge 1->2 weight = 10
    graph[1].push_back(graph::Edge{2, 10});
    graph[2].push_back(graph::Edge{1, 10});

    // edge 1->3 weight = 15
    graph[1].push_back(graph::Edge{3, 15});
    graph[3].push_back(graph::Edge{1, 15});

    // edge 2->5 weight = 2
    graph[2].push_back(graph::Edge{5, 2});
    graph[5].push_back(graph::Edge{2, 2});

    // edge 2->3 weight = 11
    graph[2].push_back(graph::Edge{3, 11});
    graph[3].push_back(graph::Edge{2, 11});

    // edge 3->4 weight = 6
    graph[3].push_back(graph::Edge{4, 6});
    graph[4].push_back(graph::Edge{3, 6});

    // edge 4->5 weight = 9
    graph[4].push_back(graph::Edge{5, 9});
    graph[5].push_back(graph::Edge{4, 9});
  }
};

/*----------------------------------------------------------------------------*/
/*                                SIMPLE TESTS                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                             TESTS WITH FIXTURE                             */
/*----------------------------------------------------------------------------*/

TEST_F(AnEmptyGraph, AbortsWhenCalculatingMinimumPathWithBinaryHeap) {
  ASSERT_DEATH(graph::dijkstra<heap::binary>(graph, 0, 4), "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AnEmptyGraph, AbortsWhenCalculatingMinimumPathWithFibonacciHeap) {
  ASSERT_DEATH(graph::dijkstra<heap::binary>(graph, 0, 4), "");
}

/*----------------------------------------------------------------------------*/

TEST_F(ADirectedGraph, CanFindMinPathBetweenUnconnectedNodesWithBinaryHeap) {
  auto minimum_path = graph::dijkstra<heap::binary>(graph, 5, 0);
  ASSERT_THAT(minimum_path, ElementsAre(5));
}

/*----------------------------------------------------------------------------*/

TEST_F(ADirectedGraph, CanFindMinPathBetweenSameNodeWithBinaryHeap) {
  auto minimum_path = graph::dijkstra<heap::binary>(graph, 0, 0);
  ASSERT_THAT(minimum_path, ElementsAre(0));
}

/*----------------------------------------------------------------------------*/

TEST_F(ADirectedGraph, CanFindMinPathBetweenDistinctNodesWithBinaryHeap) {
  auto minimum_path = graph::dijkstra<heap::binary>(graph, 0, 4);
  ASSERT_THAT(minimum_path, ElementsAre(0, 2, 3, 4));
}

/*----------------------------------------------------------------------------*/

TEST_F(ADirectedGraph, CanFindMinPathBetweenUnconnectedNodesWithFibonacciHeap) {
  auto minimum_path = graph::dijkstra<heap::fibonacci>(graph, 5, 0);
  ASSERT_THAT(minimum_path, ElementsAre(5));
}

/*----------------------------------------------------------------------------*/

TEST_F(ADirectedGraph, CanFindMinPathBetweenSameNodeWithFibonacciHeap) {
  auto minimum_path = graph::dijkstra<heap::fibonacci>(graph, 0, 0);
  ASSERT_THAT(minimum_path, ElementsAre(0));
}

/*----------------------------------------------------------------------------*/

TEST_F(ADirectedGraph, CanFindMinPathBetweenDistinctNodesWithFibonacciHeap) {
  auto minimum_path = graph::dijkstra<heap::fibonacci>(graph, 0, 4);
  ASSERT_THAT(minimum_path, ElementsAre(0, 2, 3, 4));
}

/*----------------------------------------------------------------------------*/

TEST_F(AnUndirectedGraph, CanFindMinPathBetweenSameNodeWithBinaryHeap) {
  auto minimum_path = graph::dijkstra<heap::binary>(graph, 0, 0);
  ASSERT_THAT(minimum_path, ElementsAre(0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AnUndirectedGraph, CanFindMinPathBetweenDistinctNodesWithBinaryHeap) {
  auto minimum_path = graph::dijkstra<heap::binary>(graph, 0, 4);
  ASSERT_THAT(minimum_path, ElementsAre(0, 2, 5, 4));
}

/*----------------------------------------------------------------------------*/

TEST_F(AnUndirectedGraph, CanFindMinPathBetweenSameNodeWithFibonacciHeap) {
  auto minimum_path = graph::dijkstra<heap::fibonacci>(graph, 0, 0);
  ASSERT_THAT(minimum_path, ElementsAre(0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AnUndirectedGraph, CanFindMinPathBetweenDistinctNodesWithFibonacciHeap) {
  auto minimum_path = graph::dijkstra<heap::fibonacci>(graph, 0, 4);
  ASSERT_THAT(minimum_path, ElementsAre(0, 2, 5, 4));
}

/*----------------------------------------------------------------------------*/
