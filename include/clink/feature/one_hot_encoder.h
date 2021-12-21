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

#ifndef ONE_HOT_ENCODER_H_
#define ONE_HOT_ENCODER_H_

#include "../../../protos/OneHotEncoder.pb.h"
#include "clink/linalg/sparse_vector.h"

namespace clink {

class OneHotEncoderModel {
  public:
    OneHotEncoderParams params;
    OneHotEncoderModelData modelData;
    OneHotEncoderModel();
    SparseVector transform(int x, int columnIndex);
    static OneHotEncoderModel * load(std::string);
};

} // namespace clink

#endif // ONE_HOT_ENCODER_H_
