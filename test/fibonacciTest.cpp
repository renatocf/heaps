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
#include "fibonacci.hpp"

/*----------------------------------------------------------------------------*/
/*                             USING DECLARATIONS                             */
/*----------------------------------------------------------------------------*/

using ::testing::Eq;

/*----------------------------------------------------------------------------*/
/*                                  FIXTURES                                  */
/*----------------------------------------------------------------------------*/

struct AFibonacciHeap : public ::testing::Test {
  heap::fibonacci fh { 3, 5, 8, 13, 21, 34, 55 };

  // Final heap: (03) (05) (08) (13) (21) (34) (55)
};

struct AReorganizedFibonacciHeap : public ::testing::Test {
  heap::fibonacci fh;
  heap::fibonacci::node_ptr node03, node05, node08, node13,
                            node21, node34, node55, node42,
                            node24, node33, node72, node88;

  AReorganizedFibonacciHeap() : fh() {
    node03 = fh.insert(3);
    node05 = fh.insert(5);
    node08 = fh.insert(8);
    node13 = fh.insert(13);
    node21 = fh.insert(21);
    node34 = fh.insert(34);
    node55 = fh.insert(55);
    node42 = fh.insert(42);
    node72 = fh.insert(72);
    node88 = fh.insert(88);
    fh.delete_minimum();  // To reorganize heap
  }

  // Final heap: (05 (08) (13 (21)) (34 (55) (42 (72)))) (88)
};

/*----------------------------------------------------------------------------*/
/*                                SIMPLE TESTS                                */
/*----------------------------------------------------------------------------*/

TEST(FibonacciHeap, CanBeEmptyConstructed) {
  heap::fibonacci fh;

  ASSERT_THAT(fh.size(), Eq(0u));
  ASSERT_THAT(fh.find_minimum(), Eq(nullptr));
  ASSERT_THAT(fh.to_string(), Eq(""));
}

/*----------------------------------------------------------------------------*/

TEST(FibonacciHeap, CanBeConstructedWithOneElement) {
  heap::fibonacci fh {1};

  ASSERT_THAT(fh.size(), Eq(1u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(1));
  ASSERT_THAT(fh.to_string(), Eq("(01)"));
}

/*----------------------------------------------------------------------------*/
/*                             TESTS WITH FIXTURE                             */
/*----------------------------------------------------------------------------*/

TEST_F(AFibonacciHeap, CanInsertANewNode) {
  fh.insert(1);

  ASSERT_THAT(fh.size(), Eq(8u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(1));
  ASSERT_THAT(fh.to_string(), Eq("(03) (05) (08) (13) (21) (34) (55) (01)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AFibonacciHeap, CanBeMergedWithCopiedFibonacciHeap) {
  heap::fibonacci oh {1};
  fh.merge(oh);

  ASSERT_THAT(fh.size(), Eq(8u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(1));
  ASSERT_THAT(fh.to_string(), Eq("(03) (05) (08) (13) (21) (34) (55) (01)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AFibonacciHeap, CanBeMergedWithMovedFibonacciHeap) {
  heap::fibonacci oh {1};
  fh.merge(std::move(oh));

  ASSERT_THAT(fh.size(), Eq(8u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(1));
  ASSERT_THAT(fh.to_string(), Eq("(03) (05) (08) (13) (21) (34) (55) (01)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AFibonacciHeap, CanDeleteMinimumElement) {
  auto deleted = fh.delete_minimum();

  ASSERT_THAT(deleted->key, Eq(3));

  ASSERT_THAT(fh.size(), Eq(6u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(5));
  ASSERT_THAT(fh.to_string(), Eq("(05 (08) (13 (21))) (34 (55))"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanDecreaseKeyOfMinimum) {
  fh.decrease_key(node05, 2);

  ASSERT_THAT(fh.size(), Eq(9u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(2));
  ASSERT_THAT(fh.to_string(),
      Eq("(02 (08) (13 (21)) (34 (55) (42 (72)))) (88)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanDecreaseKeyOfNonMinimumRoot) {
  fh.decrease_key(node88, 7);

  ASSERT_THAT(fh.size(), Eq(9u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(5));
  ASSERT_THAT(fh.to_string(),
      Eq("(05 (08) (13 (21)) (34 (55) (42 (72)))) (07)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, ThrowsWhenNodeKeyIsBiggerThanCurrentKey) {
  ASSERT_THROW(fh.decrease_key(node88, 90), std::invalid_argument);
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanDecreaseKeyChangingMinimum) {
  fh.decrease_key(node88, 0);

  ASSERT_THAT(fh.size(), Eq(9u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(0));
  ASSERT_THAT(fh.to_string(),
      Eq("(05 (08) (13 (21)) (34 (55) (42 (72)))) (00)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanDecreaseKeyOfNonRootWithUnmarkedParent) {
  fh.decrease_key(node42, 7);

  ASSERT_THAT(fh.size(), Eq(9u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(5));
  ASSERT_THAT(fh.to_string(),
      Eq("(05 (08) (13 (21)) (34* (55))) (88) (07 (72))"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanDecreaseKeyOfNonRootWithMarkedParent) {
  fh.decrease_key(node42, 7);
  fh.decrease_key(node55, 6);

  ASSERT_THAT(fh.size(), Eq(9u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(5));
  ASSERT_THAT(fh.to_string(),
      Eq("(05 (08) (13 (21))) (88) (07 (72)) (06) (34)"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanRemoveMinimum) {
  fh.remove(node05);

  ASSERT_THAT(fh.size(), Eq(8u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(8));
  ASSERT_THAT(fh.to_string(), Eq("(08 (88) (13 (21)) (34 (55) (42 (72))))"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedFibonacciHeap, CanRemoveNonMinimumRootNode) {
  fh.remove(node88);

  ASSERT_THAT(fh.size(), Eq(8u));
  ASSERT_THAT(fh.find_minimum()->key, Eq(5));
  ASSERT_THAT(fh.to_string(), Eq("(05 (08) (13 (21)) (34 (55) (42 (72))))"));
}

/*----------------------------------------------------------------------------*/
