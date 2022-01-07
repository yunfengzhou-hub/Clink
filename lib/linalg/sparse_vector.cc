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

namespace clink {

tfrt::AsyncValueRef<double> SparseVector::get(const int &index) {
  if (index >= n_) {
    return tfrt::MakeErrorAsyncValueRef("Index out of range.");
  }

  for (int i = 0; i < indices_.size(); i++) {
    if (indices_[i] == index) {
      return tfrt::MakeAvailableAsyncValueRef<double>(values_[i]);
    }
  }
  return tfrt::MakeAvailableAsyncValueRef<double>(0.0);
}

tfrt::AsyncValueRef<tfrt::Chain> SparseVector::set(const int &index,
                                                   const double &value) {
  if (index >= n_) {
    return tfrt::MakeErrorAsyncValueRef("Index out of range.");
  }

  for (int i = 0; i < indices_.size(); i++) {
    if (indices_[i] == index) {
      values_[i] = value;
      return tfrt::MakeAvailableAsyncValueRef<tfrt::Chain>();
    }
  }

  indices_.push_back(index);
  values_.push_back(value);
  return tfrt::MakeAvailableAsyncValueRef<tfrt::Chain>();
}

int SparseVector::size() { return n_; }

std::string SparseVector::toString() {
  std::string result = "(";
  result += std::to_string(n_) + ", (";
  for (int i = 0; i < indices_.size(); i++) {
    result += std::to_string(indices_[i]);
    if (i < indices_.size() - 1) {
      result += ", ";
    }
  }
  result += "), (";
  for (int i = 0; i < values_.size(); i++) {
    result += std::to_string(values_[i]);
    if (i < values_.size() - 1) {
      result += ", ";
    }
  }
  result += ")";
  return result;
}

} // namespace clink
