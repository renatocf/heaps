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

#ifndef GRAPH_GRAPH_
#define GRAPH_GRAPH_

// Standard headers
#include <vector>
#include <random>
#include <iostream>

// Internal headers
#include "graph/Edge.hpp"

namespace graph {

using Graph = std::vector<std::vector<Edge>>;

template<typename RandomNumberGenerator>
graph::Graph generateRandomGraph(
    size_t num_nodes, size_t num_edges, Weight max_weight,
    RandomNumberGenerator rng
      = RandomNumberGenerator{ RandomNumberGenerator::default_seed }) {
  assert((num_nodes == 0 && num_edges == 0)
         || (num_edges <= num_nodes*(num_nodes-1)/2.0));

  std::uniform_int_distribution<Key> node_generator(0, num_nodes-1);
  std::uniform_real_distribution<Weight> weight_generator(0, max_weight);

  Graph graph(num_nodes);

  for (unsigned int i = 0; i < num_edges; i++) {
    auto src = node_generator(rng);
    auto dst = node_generator(rng);
    auto weight = weight_generator(rng);
    graph[src].push_back(Edge{dst, weight});
  }

  return graph;
}

}  // namespace graph

#endif  // GRAPH_GRAPH_
