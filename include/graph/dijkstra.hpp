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

#ifndef GRAPH_DIJKSTRA_
#define GRAPH_DIJKSTRA_

// Standard headers
#include <vector>
#include <limits>
#include <cassert>
#include <utility>
#include <algorithm>
#include <functional>

// Internal headers
#include "graph/Key.hpp"
#include "graph/Edge.hpp"
#include "graph/Graph.hpp"
#include "graph/Weight.hpp"

namespace graph {

template<template<typename K, typename Comparator> class PriorityQueue>
std::vector<Key> dijkstra(const Graph& G, const Key& source,
                                          const Key& destination) {
  assert(source < G.size());
  assert(destination < G.size());

  std::vector<Key> parent(G.size(), InvalidKey);
  std::vector<Weight> d(G.size(), Infinity);

  PriorityQueue<Edge, std::less<Edge>> Q;

  d[source] = 0.0;
  Q.insert(Edge{source, d[source]});

  while (!Q.empty()) {
    auto u = Q.find_minimum().key;
    if (u == destination) break;
    Q.delete_minimum();
    for (unsigned int i = 0; i < G[u].size(); i++) {
      auto v = G[u][i].key;
      auto w = G[u][i].weight;
      if (d[v] > d[u] + w) {
        d[v] = d[u] + w;
        parent[v] = u;
        Q.insert(Edge{v, d[v]});
      }
    }
  }

  std::vector<Key> path;
  for (Key p = destination; parent[p] != InvalidKey; p = parent[p])
    path.push_back(p);
  path.push_back(source);

  std::reverse(path.begin(), path.end());

  return path;
}

}  // namespace graph

#endif  // GRAPH_DIJKSTRA_
