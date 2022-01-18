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

#include "clink/cpp_tests/test_util.h"
#include "clink/feature/one_hot_encoder.h"
#include "gtest/gtest.h"

namespace clink {

namespace {

// TEST(OneHotEncoderTest, Param) {
//   RCReference<OneHotEncoderModel> model =
//       tfrt::TakeRef(test::test_host_context->Construct<OneHotEncoderModel>(
//           test::test_host_context.get()));
//   model->setDropLast(false);
//   EXPECT_FALSE(model->getDropLast());
//   model->setDropLast(true);
//   EXPECT_TRUE(model->getDropLast());
// }

TEST(OneHotEncoderTest, Transform) {
  OneHotEncoderModelDataProto model_data;
  model_data.add_featuresizes(2);
  model_data.add_featuresizes(3);
  std::string model_data_str;
  model_data.SerializeToString(&model_data_str);

  RCReference<OneHotEncoderModel> model =
      tfrt::TakeRef(test::test_host_context->Construct<OneHotEncoderModel>(
          test::test_host_context.get()));
  model->setDropLast(false);
  llvm::Error err = model->setModelData(std::move(model_data_str));
  EXPECT_FALSE(err);

  std::vector<tfrt::AsyncValue *> inputs_vec = {
      MakeAvailableAsyncValueRef<int>(1).release(),
      MakeAvailableAsyncValueRef<int>(0).release()
  };

  auto vector = model->transform(inputs_vec)[0];
  EXPECT_TRUE(vector->IsAvailable());
  EXPECT_EQ(vector->get<SparseVector>().get(1).get(), 1.0);

  // auto invalid_value_vector = model->transform(1, 5);
  // EXPECT_FALSE(invalid_value_vector.IsConcrete());

  // auto invalid_index_vector = model->transform(5, 0);
  // EXPECT_FALSE(invalid_index_vector.IsConcrete());
}

// TEST(OneHotEncoderTest, Load) {
//   test::TemporaryFolder tmp_folder;

//   nlohmann::json params;
//   params["paramMap"]["dropLast"] = "false";

//   OneHotEncoderModelDataProto model_data;
//   model_data.add_featuresizes(2);
//   model_data.add_featuresizes(3);

//   test::saveMetaDataModelData(tmp_folder.getAbsolutePath(), params, model_data);

//   auto model = OneHotEncoderModel::load(tmp_folder.getAbsolutePath(),
//                                         test::test_host_context.get());
//   EXPECT_FALSE(model.takeError());

//   auto vector = model.get()->transform(1, 0);
//   EXPECT_TRUE(vector.IsConcrete());
//   EXPECT_EQ(vector->get(1).get(), 1.0);
// }

// TEST(OneHotEncoderTest, Mlir) {
//   test::TemporaryFolder tmp_folder;

//   nlohmann::json params;
//   params["paramMap"]["dropLast"] = "false";

//   OneHotEncoderModelDataProto model_data;
//   model_data.add_featuresizes(2);
//   model_data.add_featuresizes(3);

//   test::saveMetaDataModelData(tmp_folder.getAbsolutePath(), params, model_data);

//   // TODO: Separate the load process that is triggered only once and the
//   // repeatedly triggered transform process into different scripts.
//   auto mlir_script = R"mlir(
//     func @main(%path: !tfrt.string, %value: i32, %column_index: i32) -> !tfrt.string {
//       %model = clink.onehotencoder_load %path
//       %vector = clink.onehotencoder_transform %model, %value, %column_index
//       %vector_string = clink.sparsevector_tostring %vector
//       tfrt.return %vector_string : !tfrt.string
//     }
//   )mlir";

//   llvm::SmallVector<RCReference<AsyncValue>> inputs;
//   inputs.push_back(tfrt::MakeAvailableAsyncValueRef<std::string>(
//       tmp_folder.getAbsolutePath()));
//   inputs.push_back(tfrt::MakeAvailableAsyncValueRef<int32_t>(1));
//   inputs.push_back(tfrt::MakeAvailableAsyncValueRef<int32_t>(0));

//   auto results = test::runMlirScript(mlir_script, inputs);
//   EXPECT_EQ(results.size(), 1);
//   test::test_host_context->Await(results[0]);
//   EXPECT_EQ(results[0]->get<std::string>(), "(2, (1), (1.000000)");
// }

} // namespace
} // namespace clink
