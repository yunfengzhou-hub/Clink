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

#include <iostream>
#include <fstream>
#include <string>

#include "clink/linalg/sparse_vector.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace clink;
using namespace std;

SparseVector::SparseVector(const int n, const int * indices, const int indicesLen, const double * values, const int valuesLen) {
    this->n = n;
    this->indicesLen = indicesLen;
    this->indices = new int[this->indicesLen];
    for (int i = 0; i < this->indicesLen; i++) {
      this->indices[i] = indices[i];
    }
    this->valuesLen = valuesLen;
    this->values = new double[this->valuesLen];
    for (int i = 0; i < this->valuesLen; i++) {
      this->values[i] = values[i];
    }
}

SparseVector::~SparseVector(void) {
  delete [] this->indices;
  this->indices = NULL;
  delete [] this->values;
  this->values = NULL;
}

#ifdef __cplusplus
}
#endif
