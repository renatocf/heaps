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
#include "heap/Fibonacci.hpp"

// Aliases
using FibonacciHeap = heap::Fibonacci<int>;

/*----------------------------------------------------------------------------*/
/*                             USING DECLARATIONS                             */
/*----------------------------------------------------------------------------*/

using ::testing::Eq;

/*----------------------------------------------------------------------------*/
/*                                  FIXTURES                                  */
/*----------------------------------------------------------------------------*/

struct AFibonacciHeap : public ::testing::Test {
  FibonacciHeap fib { 3, 5, 8, 13, 21, 34, 55 };

  // Final heap: (03) (05) (08) (13) (21) (34) (55)
};

struct AReorganizedFibonacciHeap : public ::testing::Test {
  FibonacciHeap fib;
  FibonacciHeap::node_ptr node03, node05, node08, node13,
                          node21, node34, node55, node42,
                          node24, node33, node72, node88;

  AReorganizedFibonacciHeap() : fib() {
    node03 = fib.insert(3);
    node05 = fib.insert(5);
    node08 = fib.insert(8);
    node13 = fib.insert(13);
    node21 = fib.insert(21);
    node34 = fib.insert(34);
    node55 = fib.insert(55);
    node42 = fib.insert(42);
    node72 = fib.insert(72);
    node88 = fib.insert(88);
    fib.delete_minimum();  // To reorganize heap
  }

  // Final heap: (05 (08) (13 (21)) (34 (55) (42 (72)))) (88)
};

/*----------------------------------------------------------------------------*/
/*                                SIMPLE TESTS                                */
/*----------------------------------------------------------------------------*/

TEST(FibonacciHeap, CanBeEmptyConstructed) {
  FibonacciHeap fib;

  ASSERT_THAT(fib.size(), Eq(0u));
  ASSERT_THAT(fib.empty(), Eq(true));
  ASSERT_THAT(fib.get_minimum(), Eq(nullptr));
  ASSERT_THAT(fib.to_string(), Eq(""));
}

/*----------------------------------------------------------------------------*/

TEST(FibonacciHeap, CanBeConstructedWithOneElement) {
  FibonacciHeap fib {1};

  ASSERT_THAT(fib.size(), Eq(1u));
  ASSERT_THAT(fib.empty(), Eq(false));
  ASSERT_THAT(fib.find_minimum(), Eq(1));
  ASSERT_THAT(fib.to_string(), Eq("(01)"));
}

/*----------------------------------------------------------------------------*/
/*                             TESTS WITH FIXTURE                             */
/*----------------------------------------------------------------------------*/

TEST_F(AFibonacciHeap, CanInsertANewNode) {
  fib.insert(1);

  ASSERT_THAT(fib.size(), Eq(8u));
  ASSERT_THAT(fib.find_minimum(), Eq(1));
  ASSERT_THAT(fib.to_string(), Eq("(03) (05) (08) (13) (21) (34) (55) (01)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AFibonacciHeap, CanBeMergedWithCopiedFibonacciHeap) {
  FibonacciHeap oh {1};
  fib.merge(oh);

  ASSERT_THAT(fib.size(), Eq(8u));
  ASSERT_THAT(fib.find_minimum(), Eq(1));
  ASSERT_THAT(fib.to_string(), Eq("(03) (05) (08) (13) (21) (34) (55) (01)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AFibonacciHeap, CanBeMergedWithMovedFibonacciHeap) {
  FibonacciHeap oh {1};
  fib.merge(std::move(oh));

  ASSERT_THAT(fib.size(), Eq(8u));
  ASSERT_THAT(fib.find_minimum(), Eq(1));
  ASSERT_THAT(fib.to_string(), Eq("(03) (05) (08) (13) (21) (34) (55) (01)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AFibonacciHeap, CanRemoveMinimumElement) {
  auto deleted_node = fib.remove_minimum();

  ASSERT_THAT(deleted_node->key, Eq(3));

  ASSERT_THAT(fib.size(), Eq(6u));
  ASSERT_THAT(fib.find_minimum(), Eq(5));
  ASSERT_THAT(fib.to_string(), Eq("(05 (08) (13 (21))) (34 (55))"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AFibonacciHeap, CanDeleteMinimumElement) {
  auto deleted_key = fib.delete_minimum();

  ASSERT_THAT(deleted_key, Eq(3));

  ASSERT_THAT(fib.size(), Eq(6u));
  ASSERT_THAT(fib.find_minimum(), Eq(5));
  ASSERT_THAT(fib.to_string(), Eq("(05 (08) (13 (21))) (34 (55))"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanDecreaseKeyOfMinimum) {
  fib.decrease_key(node05, 2);

  ASSERT_THAT(fib.size(), Eq(9u));
  ASSERT_THAT(fib.find_minimum(), Eq(2));
  ASSERT_THAT(fib.to_string(),
      Eq("(02 (08) (13 (21)) (34 (55) (42 (72)))) (88)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanDecreaseKeyOfNonMinimumRoot) {
  fib.decrease_key(node88, 7);

  ASSERT_THAT(fib.size(), Eq(9u));
  ASSERT_THAT(fib.find_minimum(), Eq(5));
  ASSERT_THAT(fib.to_string(),
      Eq("(05 (08) (13 (21)) (34 (55) (42 (72)))) (07)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, ThrowsWhenNodeKeyIsBiggerThanCurrentKey) {
  ASSERT_THROW(fib.decrease_key(node88, 90), std::invalid_argument);
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanDecreaseKeyChangingMinimum) {
  fib.decrease_key(node88, 0);

  ASSERT_THAT(fib.size(), Eq(9u));
  ASSERT_THAT(fib.find_minimum(), Eq(0));
  ASSERT_THAT(fib.to_string(),
      Eq("(05 (08) (13 (21)) (34 (55) (42 (72)))) (00)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanDecreaseKeyOfNonRootWithUnmarkedParent) {
  fib.decrease_key(node42, 7);

  ASSERT_THAT(fib.size(), Eq(9u));
  ASSERT_THAT(fib.find_minimum(), Eq(5));
  ASSERT_THAT(fib.to_string(),
      Eq("(05 (08) (13 (21)) (34* (55))) (88) (07 (72))"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanDecreaseKeyOfNonRootWithMarkedParent) {
  fib.decrease_key(node42, 7);
  fib.decrease_key(node55, 6);

  ASSERT_THAT(fib.size(), Eq(9u));
  ASSERT_THAT(fib.find_minimum(), Eq(5));
  ASSERT_THAT(fib.to_string(),
      Eq("(05 (08) (13 (21))) (88) (07 (72)) (06) (34)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanRemoveMinimum) {
  fib.remove(node05);

  ASSERT_THAT(fib.size(), Eq(8u));
  ASSERT_THAT(fib.find_minimum(), Eq(8));
  ASSERT_THAT(fib.to_string(), Eq("(08 (88) (13 (21)) (34 (55) (42 (72))))"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanRemoveNonMinimumRootNode) {
  fib.remove(node88);

  ASSERT_THAT(fib.size(), Eq(8u));
  ASSERT_THAT(fib.find_minimum(), Eq(5));
  ASSERT_THAT(fib.to_string(), Eq("(05 (08) (13 (21)) (34 (55) (42 (72))))"));
}

/*----------------------------------------------------------------------------*/
