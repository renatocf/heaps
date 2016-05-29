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

#ifndef HEAP_BINARY_
#define HEAP_BINARY_

// Standard headers
#include <queue>
#include <memory>
#include <limits>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <functional>

namespace heap {

/**
 * @class Binary
 * @brief Binary Heap data structure
 */
template<typename K, typename Comparator = std::less<K>>
class Binary {
 public:
  // Forward declaration
  struct node;

  // Aliases
  using key_type = K;
  using node_ptr = std::shared_ptr<node>;

  // Inner structs
  struct node {
    // Instance variables
    key_type key;

    // Operator overload
    friend bool operator<(const node_ptr& lhs, const node_ptr& rhs) {
      return Comparator()(rhs->key, lhs->key);
    }
  };

  // Constructors
  Binary() : Binary({}) {
  }

  explicit Binary(std::initializer_list<key_type> keys) {
    for (const auto& key : keys)
      heap.emplace_back(new node{key});
    std::make_heap(heap.begin(), heap.end());
  }

  // Concrete methods

  /**
   * Find minimum key in time O(1)
   * @return Copy of the minimum key
   */
  key_type find_minimum() const {
    return get_minimum()->key;
  }

  /**
   * Get minimum node in time O(1)
   * @return Constant pointer to the minimum node
   */
  node_ptr get_minimum() const {
    return heap.size() > 0 ? heap.front() : nullptr;
  }

  /**
   * Insert new node in time O(lg n)
   * @param key Key of the new node
   * @return Pointer to new node
   */
  node_ptr insert(key_type key) {
    auto new_node = node_ptr(new node{key});
    heap.push_back(new_node);
    std::push_heap(heap.begin(), heap.end());
    return new_node;
  }

  /**
   * Merge copy of nodes of other binary heap in time O(n)
   * @param bin Lkey reference to binary heap to be merged
   */
  void merge(const Binary& bin) {
    auto copy = bin;
    merge(std::move(copy));
  }

  /**
   * Merge nodes of other binary heap in time O(n)
   * @param bin Rkey reference to binary heap to be merged
   */
  void merge(Binary&& bin) {
    heap.insert(heap.end(), bin.nodes().begin(), bin.nodes().end());
    std::make_heap(heap.begin(), heap.end());
  }

  /**
   * Delete minimum node in time O(lg n)
   * @return minimum value stored in the minimum node
   */
  key_type delete_minimum() {
    return remove_minimum()->key;
  }

  /**
   * Remove minimum node in time O(lg n)
   * @return pointer to the minimum node
   */
  node_ptr remove_minimum() {
    std::pop_heap(heap.begin(), heap.end());
    auto deleted = heap.back();
    heap.pop_back();
    return deleted;
  }

  /**
   * Decrease key of existent node in time O(n)
   * @return pointer to the minimum node
   */
  void decrease_key(node_ptr& node, const key_type& new_key) {
    if (new_key > node->key) {
      std::ostringstream oss;
      oss << "Key " << new_key << " is bigger current key " << node->key;
      throw std::invalid_argument(oss.str());
    }

    node->key = new_key;
    auto modified = std::is_heap_until(heap.begin(), heap.end());
    if (modified != heap.end()) {
      std::push_heap(heap.begin(),
                     heap.begin() + std::distance(heap.begin(), modified) + 1);
    }
  }

  /**
   * Delete arbitrary node in amortized time O(n)
   * @param node Pointer to node to be deleted
   */
  void remove(node_ptr& node) {
    decrease_key(node, std::numeric_limits<key_type>::lowest());
    delete_minimum();
  }

  /**
   * @return Number of elements stored in the heap
   */
  std::size_t size() const {
    return heap.size();
  }

  /**
   * @return True if heap is empty; false otherwise
   */
  bool empty() const {
    return heap.empty();
  }

  /**
   * @return List-like representation of the heap
   */
  std::string to_string() const {
    std::ostringstream oss;
    operator<<(oss, *this);
    return oss.str();
  }

  /**
   * @return List of roots of trees
   */
  std::vector<node_ptr>& nodes() {
    return heap;
  }

  /**
   * @return List of roots of trees
   */
  const std::vector<node_ptr>& nodes() const {
    return heap;
  }

 private:
  // Instance variables
  std::vector<node_ptr> heap;

  // Friend overloaded operators
  friend std::ostream& operator<<(std::ostream& os, const Binary& bin) {
    auto it = std::begin(bin.nodes());
    auto end = std::end(bin.nodes());

    if (it == end) return os;

    while (it != std::prev(std::end(bin.nodes()))) {
      os << std::setw(2) << std::setfill('0') << (*it)->key << " ";
      ++it;
    }
    os << std::setw(2) << std::setfill('0') << (*it)->key;
    return os;
  }
};

}  // namespace heap

#endif  // HEAP_BINARY_
