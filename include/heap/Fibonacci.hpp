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

#ifndef HEAP_FIBONACCI_
#define HEAP_FIBONACCI_

// Standard headers
#include <list>
#include <cmath>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <functional>

namespace heap {

/**
 * @class Fibonacci
 * @brief Fibonacci Heap data structure
 */
template<typename K, typename Comparator = std::less<K>>
class Fibonacci {
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
    std::weak_ptr<node> parent = {};
    std::list<node_ptr> children = {};
    bool marked = false;
    bool removed = false;

    // Concrete methods
    bool is_root() const { return parent.expired(); }
    size_t rank() const { return children.size(); }
  };

  // Constructors
  Fibonacci() : Fibonacci({}) {
  }

  explicit Fibonacci(std::initializer_list<key_type> keys)
      : trees(make_trees(keys)), num_elements(keys.size()),
        minimum(search_minimum()),
        cmp([] (const node_ptr& lhs, const node_ptr& rhs) -> bool {
          if (lhs->removed) return true;
          return Comparator()(lhs->key, rhs->key);
        }) {
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
   * @return Constant reference to the minimum node
   */
  node_ptr get_minimum() const {
    return minimum;
  }

  /**
   * Insert new node in time O(1)
   * @param key Key of the new node
   * @return Pointer to new node
   */
  node_ptr insert(key_type key) {
    trees.emplace_back(new node{key});
    num_elements++;

    if (num_elements == 1u || cmp(trees.back(), minimum))
      minimum = trees.back();

    return trees.back();
  }

  /**
   * Merge copy of nodes of other fibonacci heap in time O(n)
   * @param fh Lkey reference to fibonacci heap to be merged
   */
  void merge(const Fibonacci& fh) {
    trees.insert(trees.end(), fh.roots().begin(), fh.roots().end());

    num_elements += fh.size();

    if (cmp(fh.get_minimum(), minimum))
      minimum = fh.get_minimum();
  }

  /**
   * Merge nodes of other fibonacci heap in time O(1)
   * @param fh Rkey reference to fibonacci heap to be merged
   */
  void merge(Fibonacci&& fh) {
    trees.splice(trees.end(), fh.roots());

    num_elements += fh.size();

    if (cmp(fh.get_minimum(), minimum))
      minimum = fh.get_minimum();
  }

  /**
   * Delete minimum node in amortized time O(D) = O(lg n)
   * @return minimum value stored in the minimum node
   */
  key_type delete_minimum() {
    return remove_minimum()->key;
  }

  /**
   * Remove minimum node in amortized time O(D) = O(lg n)
   * @return pointer to the minimum node
   */
  node_ptr remove_minimum() {
    // Phase 1: remove minimum and merge its children as roots [T(n) = O(lg n)]
    auto deleted = minimum;
    trees.remove(deleted);
    num_elements--;

    trees.splice(trees.end(), deleted->children);

    // Phase 2: link trees with the same rank [T(n) = O(lg n + m)]
    consolidate();

    // Phase 3: search new minimum [T(n) = O(lg n)]
    minimum = search_minimum();

    return deleted;
  }

  /**
   * Decrease key of existent node in amortized time O(1)
   * @return pointer to the minimum node
   */
  void decrease_key(node_ptr& node, const key_type& new_key) {
    // Check if key is being decreased
    if (Comparator()(node->key, new_key)) {
      std::ostringstream oss;
      oss << "Key " << new_key << " is bigger current key " << node->key;
      throw std::invalid_argument(oss.str());
    }

    // Set new key
    node->key = new_key;
    if (cmp(node, minimum))
      minimum = node;

    // Node is root: nothing to do
    if (node->is_root()) return;

    // Heap property not violated: nothing to do
    auto parent = node->parent.lock();
    if (cmp(parent, node)) return;

    // 1st child to violate heap property: parent is marked and node is cut
    if (!parent->marked) {
      mark(parent);
      cut(node);
      return;
    }

    // 2nd child to violate heap property: cascade cut untill non-marked/root
    cascade_cut(node);
  }

  /**
   * Delete arbitrary node in amortized time O(D) = O(lg n)
   * @param node Pointer to node to be deleted
   */
  void remove(node_ptr& node) {
    node->removed = true;
    decrease_key(node, node->key);
    delete_minimum();
  }

  /**
   * @return Number of elements stored in the heap
   */
  std::size_t size() const {
    return num_elements;
  }

  /**
   * @return True if heap is empty; false otherwise
   */
  bool empty() const {
    return num_elements == 0u;
  }

  /**
   * @return SExpr-like representation of the heap
   */
  std::string to_string() const {
    std::ostringstream oss;
    operator<<(oss, *this);
    return oss.str();
  }

  /**
   * @return List of roots of trees
   */
  std::list<node_ptr>& roots() {
    return trees;
  }

  /**
   * @return List of roots of trees
   */
  const std::list<node_ptr>& roots() const {
    return trees;
  }

 private:
  // Instance variables
  std::list<node_ptr> trees;
  size_t num_elements = 0;
  node_ptr minimum;

  std::function<bool(const node_ptr&, const node_ptr&)> cmp;

  // Concrete methods

  /**
   * Make single node trees from list of keys
   * @return List of trees
   */
  std::list<node_ptr>
  make_trees(const std::initializer_list<key_type>& keys) const {
    std::list<node_ptr> trees(keys.size(), nullptr);
    std::transform(keys.begin(), keys.end(), trees.begin(),
        [this](const auto& k) { return std::shared_ptr<node>(new node{k}); });
    return trees;
  }

  /**
   * Search minimum in time O(n)
   * @return Pointer to the minimum node
   */
  node_ptr search_minimum() const {
    auto it = std::min_element(trees.begin(), trees.end(),
        [](const auto& a, const auto& b) { return a->key < b->key; });
    return it != trees.end() ? *it : nullptr;
  }

  /**
   * Link two trees in a single tree, with minimum element being root
   * @return Reference to the root node
   */
  const node_ptr& link(const node_ptr& lhs, const node_ptr& rhs) const {
    if (cmp(lhs, rhs)) {
      lhs->children.push_back(rhs);
      rhs->parent = lhs;
      return lhs;
    } else {
      rhs->children.push_back(lhs);
      lhs->parent = rhs;
      return rhs;
    }
  }

  /**
   * Shrinks root list to O(lg n) elements
   */
  void consolidate() {
    if (num_elements == 0) return;

    auto max_rank = static_cast<size_t>(std::floor(std::log2(num_elements)));

    using node_list_iterator = typename std::list<node_ptr>::iterator;
    std::vector<node_list_iterator> root_with_rank(max_rank + 1, trees.end());

    for (auto it = trees.begin(); it != trees.end(); ++it) {
      auto curr_it = it;
      ++it;

      while (root_with_rank[(*curr_it)->rank()] != trees.end()) {
        auto other_it = root_with_rank[(*curr_it)->rank()];
        (*other_it) = link(*curr_it, *other_it);

        root_with_rank[(*curr_it)->rank()] = trees.end();
        trees.erase(curr_it);

        curr_it = other_it;
      }

      root_with_rank[(*curr_it)->rank()] = curr_it;
      --it;
    }
  }

  /**
   * Cut node from its parent and insert in the root list
   * @param node Node to be cut
   */
  void cut(node_ptr& node) {
    node->parent.lock()->children.remove(node);
    trees.push_back(node);
    node->marked = false;
    node->parent.reset();
  }

  /**
   * Mark node as having a cut child (unless it's root)
   * @param node Node to be marked
   */
  void mark(node_ptr& node) {
    if (node->is_root()) return;
    node->marked = true;
  }

  /**
   * Cut nodes repeatedly until a non-marked/root node is found
   * @param node Node that will start cascading cut
   */
  void cascade_cut(node_ptr node) {
    while (!node->is_root() && node->parent.lock()->marked) {
      auto parent = node->parent.lock();
      cut(node);
      node = parent;
    }
    auto parent = node->parent.lock();
    mark(parent);
    cut(node);
  }

  /**
   * Print tree as a SExpr
   * @param os Output stream to print tree
   * @param roots list of trees to be printed
   */
  void print_trees(std::ostream& os, const std::list<node_ptr>& roots) const {
    for (auto it = std::begin(roots); it != std::end(roots); ++it) {
      auto root = *it;
      os << "(" << std::setw(2) << std::setfill('0') << root->key;
      if (root->marked) os << "*";
      if (!root->children.empty()) {
        os << " ";
        print_trees(os, root->children);
      }
      os << ")";
      if (it != std::prev(std::end(roots))) os << ' ';
    }
  }

  // Friend overloaded operators
  friend std::ostream& operator<<(std::ostream& os, const Fibonacci& fh) {
    fh.print_trees(os, fh.trees);
    return os;
  }
};

}  // namespace heap

#endif  // HEAP_FIBONACCI_
