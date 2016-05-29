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

#ifndef GRAPH_EDGE_
#define GRAPH_EDGE_

// Internal headers
#include "graph/Key.hpp"
#include "graph/Weight.hpp"

namespace graph {

struct Edge {
  Key key;
  Weight weight;

  friend bool operator<(const Edge& lhs, const Edge& rhs) {
    return lhs.weight < rhs.weight;
  }
};

}  // namespace graph

#endif  // GRAPH_EDGE_
