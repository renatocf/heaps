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
#include "heap/Binary.hpp"

// Aliases
using BinaryHeap = heap::Binary<int>;

/*----------------------------------------------------------------------------*/
/*                             USING DECLARATIONS                             */
/*----------------------------------------------------------------------------*/

using ::testing::Eq;

/*----------------------------------------------------------------------------*/
/*                                  FIXTURES                                  */
/*----------------------------------------------------------------------------*/

struct ABinaryHeap : public ::testing::Test {
  BinaryHeap bin { 3, 5, 8, 13, 21, 34, 55 };

  // Final heap: (03) (05) (08) (13) (21) (34) (55)
};

struct AReorganizedBinaryHeap : public ::testing::Test {
  BinaryHeap bin;
  BinaryHeap::node_ptr node03, node05, node08, node13,
                       node21, node34, node55, node42,
                       node24, node33, node72, node88;

  AReorganizedBinaryHeap() : bin() {
    node03 = bin.insert(3);
    node05 = bin.insert(5);
    node08 = bin.insert(8);
    node13 = bin.insert(13);
    node21 = bin.insert(21);
    node34 = bin.insert(34);
    node55 = bin.insert(55);
    node42 = bin.insert(42);
    node72 = bin.insert(72);
    node88 = bin.insert(88);
    bin.delete_minimum();  // To reorganize heap
  }

  // Final heap: (05 (08) (13 (21)) (34 (55) (42 (72)))) (88)
};

/*----------------------------------------------------------------------------*/
/*                                SIMPLE TESTS                                */
/*----------------------------------------------------------------------------*/

TEST(BinaryHeap, CanBeEmptyConstructed) {
  BinaryHeap bin;

  ASSERT_THAT(bin.size(), Eq(0u));
  ASSERT_THAT(bin.empty(), Eq(true));
  ASSERT_THAT(bin.get_minimum(), Eq(nullptr));
  ASSERT_THAT(bin.to_string(), Eq(""));
}

/*----------------------------------------------------------------------------*/

TEST(BinaryHeap, CanBeConstructedWithOneElement) {
  BinaryHeap bin {1};

  ASSERT_THAT(bin.size(), Eq(1u));
  ASSERT_THAT(bin.empty(), Eq(false));
  ASSERT_THAT(bin.find_minimum(), Eq(1));
  ASSERT_THAT(bin.to_string(), Eq("01"));
}

/*----------------------------------------------------------------------------*/
/*                             TESTS WITH FIXTURE                             */
/*----------------------------------------------------------------------------*/

TEST_F(ABinaryHeap, CanInsertANewNode) {
  bin.insert(1);

  ASSERT_THAT(bin.size(), Eq(8u));
  ASSERT_THAT(bin.find_minimum(), Eq(1));
  ASSERT_THAT(bin.to_string(), Eq("01 03 08 05 21 34 55 13"));
}

/*----------------------------------------------------------------------------*/

TEST_F(ABinaryHeap, CanBeMergedWithCopiedBinaryHeap) {
  BinaryHeap oh {1};
  bin.merge(oh);

  ASSERT_THAT(bin.size(), Eq(8u));
  ASSERT_THAT(bin.find_minimum(), Eq(1));
  ASSERT_THAT(bin.to_string(), Eq("01 03 08 05 21 34 55 13"));
}

/*----------------------------------------------------------------------------*/

TEST_F(ABinaryHeap, CanBeMergedWithMovedBinaryHeap) {
  BinaryHeap oh {1};
  bin.merge(std::move(oh));

  ASSERT_THAT(bin.size(), Eq(8u));
  ASSERT_THAT(bin.find_minimum(), Eq(1));
  ASSERT_THAT(bin.to_string(), Eq("01 03 08 05 21 34 55 13"));
}

/*----------------------------------------------------------------------------*/

TEST_F(ABinaryHeap, CanRemoveMinimumElement) {
  auto deleted_node = bin.remove_minimum();

  ASSERT_THAT(deleted_node->key, Eq(3));

  ASSERT_THAT(bin.size(), Eq(6u));
  ASSERT_THAT(bin.find_minimum(), Eq(5));
  ASSERT_THAT(bin.to_string(), Eq("05 13 08 55 21 34"));
}

/*----------------------------------------------------------------------------*/

TEST_F(ABinaryHeap, CanDeleteMinimumElement) {
  auto deleted_key = bin.delete_minimum();

  ASSERT_THAT(deleted_key, Eq(3));

  ASSERT_THAT(bin.size(), Eq(6u));
  ASSERT_THAT(bin.find_minimum(), Eq(5));
  ASSERT_THAT(bin.to_string(), Eq("05 13 08 55 21 34"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedBinaryHeap, CanDecreaseKeyOfMinimum) {
  bin.decrease_key(node05, 2);

  ASSERT_THAT(bin.size(), Eq(9u));
  ASSERT_THAT(bin.find_minimum(), Eq(2));
  ASSERT_THAT(bin.to_string(), Eq("02 13 08 42 21 34 55 88 72"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedBinaryHeap, CanDecreaseKeyOfNonMinimumRoot) {
  bin.decrease_key(node88, 7);

  ASSERT_THAT(bin.size(), Eq(9u));
  ASSERT_THAT(bin.find_minimum(), Eq(5));
  ASSERT_THAT(bin.to_string(), Eq("05 07 08 13 21 34 55 42 72"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedBinaryHeap, ThrowsWhenNodeKeyIsBiggerThanCurrentKey) {
  ASSERT_THROW(bin.decrease_key(node88, 90), std::invalid_argument);
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedBinaryHeap, CanDecreaseKeyChangingMinimum) {
  bin.decrease_key(node88, 0);

  ASSERT_THAT(bin.size(), Eq(9u));
  ASSERT_THAT(bin.find_minimum(), Eq(0));
  ASSERT_THAT(bin.to_string(), Eq("00 05 08 13 21 34 55 42 72"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedBinaryHeap, CanRemoveMinimum) {
  bin.remove(node05);

  ASSERT_THAT(bin.size(), Eq(8u));
  ASSERT_THAT(bin.find_minimum(), Eq(8));
  ASSERT_THAT(bin.to_string(), Eq("08 13 34 42 21 72 55 88"));
}

/*----------------------------------------------------------------------------*/

TEST_F(AReorganizedBinaryHeap, CanRemoveNonMinimumRootNode) {
  bin.remove(node88);

  ASSERT_THAT(bin.size(), Eq(8u));
  ASSERT_THAT(bin.find_minimum(), Eq(5));
  ASSERT_THAT(bin.to_string(), Eq("05 13 08 42 21 34 55 72"));
}

/*----------------------------------------------------------------------------*/
