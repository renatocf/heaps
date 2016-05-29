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

// Tested header
#include "graph/dijkstra.hpp"

/*----------------------------------------------------------------------------*/
/*                             USING DECLARATIONS                             */
/*----------------------------------------------------------------------------*/

using ::testing::Eq;
using ::testing::Gt;
using ::testing::Ge;
using ::testing::Lt;
using ::testing::Le;

/*----------------------------------------------------------------------------*/
/*                                SIMPLE TESTS                                */
/*----------------------------------------------------------------------------*/

TEST(ANewEmptyGraph, DiesIfRandomlyGeneratedWithMoreThanZeroEdges) {
  ASSERT_DEATH(graph::generateRandomGraph<std::mt19937>(0, 5, 10.0), "");
}

/*----------------------------------------------------------------------------*/

TEST(ANewGraph, DiesIfRandomlyGeneratedWithMoreThanNChoose2Edges) {
  ASSERT_DEATH(graph::generateRandomGraph<std::mt19937>(5, 11, 10.0), "");
}

/*----------------------------------------------------------------------------*/

TEST(ANewGraph, CanBeRandomlyGeneratedWithDefaultSeed) {
  auto graph = graph::generateRandomGraph<std::mt19937>(5, 5, 10.0);

  size_t num_nodes = graph.size();

  size_t num_edges = 0;
  for (unsigned int i = 0; i < graph.size(); i++)
    num_edges += graph[i].size();

  ASSERT_THAT(num_nodes, Eq(5u));
  ASSERT_THAT(num_edges, Eq(5u));

  for (unsigned int i = 0; i < graph.size(); i++) {
    for (unsigned int j = 0; j < graph[i].size(); j++) {
      ASSERT_THAT(graph[i][j].key, Ge(0u));
      ASSERT_THAT(graph[i][j].key, Le(5u));
      ASSERT_THAT(graph[i][j].weight, Ge(0.0));
      ASSERT_THAT(graph[i][j].weight, Le(10.0));
    }
  }
}

/*----------------------------------------------------------------------------*/
