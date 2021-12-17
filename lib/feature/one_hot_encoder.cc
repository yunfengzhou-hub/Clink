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

#include "clink/feature/one_hot_encoder.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace clink;
using namespace std;

OneHotEncoderModel::OneHotEncoderModel(){}

SparseVector OneHotEncoderModel::transform(int x, int columnIndex) {
  int len = modelData.featuresize(columnIndex);
  if (params.isdroplast()) {
    len -= 1;
  }
  if (x >= len) {
    int indices[0];
    double values[0];
    return SparseVector(len, indices, 0, values, 0);
  }

  int indices[1]{x};
  double values[1]{1.0};
  return SparseVector(len, indices, 1, values, 1);
}

OneHotEncoderModel * OneHotEncoderModel::load(std::string path) {
  OneHotEncoderModel * model = new OneHotEncoderModel();

  {
    fstream input(path + "/modeldata", ios::in | ios::binary);
    if (!model->modelData.ParseFromIstream(&input)) {
      cerr << "Failed to parse modeldata." << endl;
      return NULL;
    }
  }

  {
    fstream input(path + "/metadata", ios::in | ios::binary);
    if (!model->params.ParseFromIstream(&input)) {
      cerr << "Failed to parse metadata." << endl;
      return NULL;
    }
  }

  return model;

}

#ifdef __cplusplus
}
#endif
