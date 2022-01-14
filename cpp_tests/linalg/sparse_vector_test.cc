/*
 * Copyright 2021 The Clink Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "clink/linalg/sparse_vector.h"

#include "clink/cpp_tests/test_util.h"
#include "gtest/gtest.h"

namespace clink {

namespace {

TEST(SparseVectorTest, CreatesVector) {
  SparseVector vector(5);
  EXPECT_EQ(vector.size(), 5);
}

TEST(SparseVectorTest, SetGetValue) {
  SparseVector vector(5);
  vector.set(1, 1.0);
  vector.set(2, 3.0);
  vector.set(4, 2.5);
  EXPECT_EQ(vector.get(0).get(), 0.0);
  EXPECT_EQ(vector.get(1).get(), 1.0);
  EXPECT_EQ(vector.get(2).get(), 3.0);
  EXPECT_EQ(vector.get(3).get(), 0.0);
  EXPECT_EQ(vector.get(4).get(), 2.5);
  EXPECT_TRUE(vector.get(4).IsConcrete());
  EXPECT_FALSE(vector.get(5).IsConcrete());
}

} // namespace
} // namespace clink
