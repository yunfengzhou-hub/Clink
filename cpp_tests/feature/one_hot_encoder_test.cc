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

class OneHotEncoderTest : public testing::Test {
protected:
  static void SetUpTestSuite() {
    assert(host_context == nullptr);
    host_context =
        CreateHostContext("mstd", tfrt::HostAllocatorType::kLeakCheckMalloc)
            .release();
    assert(mlir_context == nullptr);
    mlir_context = new MLIRContext();
    mlir_context->allowUnregisteredDialects();
    mlir_context->printOpOnDiagnostic(true);
    mlir::DialectRegistry registry;
    registry.insert<clink::ClinkDialect>();
    registry.insert<tfrt::compiler::TFRTDialect>();
    mlir_context->appendDialectRegistry(registry);

    assert(exec_context == nullptr);
    exec_context = new ExecutionContext(
        *tfrt::RequestContextBuilder(host_context, nullptr).build());
  }

  static void TearDownTestSuite() {
    delete host_context;
    host_context = nullptr;
    delete mlir_context;
    mlir_context = nullptr;
    delete exec_context;
    exec_context = nullptr;
  }

  static tfrt::HostContext *host_context;
  static MLIRContext *mlir_context;
  static ExecutionContext *exec_context;
};

tfrt::HostContext *OneHotEncoderTest::host_context = nullptr;

MLIRContext *OneHotEncoderTest::mlir_context = nullptr;

ExecutionContext *OneHotEncoderTest::exec_context = nullptr;

TEST_F(OneHotEncoderTest, Param) {
  RCReference<OneHotEncoderModel> model =
      tfrt::TakeRef(host_context->Construct<OneHotEncoderModel>(host_context));
  model->setDropLast(false);
  EXPECT_FALSE(model->getDropLast());
  model->setDropLast(true);
  EXPECT_TRUE(model->getDropLast());
}

TEST_F(OneHotEncoderTest, Transform) {
  OneHotEncoderModelDataProto model_data;
  model_data.add_featuresizes(2);
  model_data.add_featuresizes(3);
  std::string model_data_str;
  model_data.SerializeToString(&model_data_str);

  RCReference<OneHotEncoderModel> model =
      tfrt::TakeRef(host_context->Construct<OneHotEncoderModel>(host_context));
  model->setDropLast(false);
  llvm::Error err = model->setModelData(std::move(model_data_str));
  EXPECT_FALSE(err);

  SparseVector expected_vector(2);
  expected_vector.set(1, 1.0);

  {
    llvm::SmallVector<tfrt::RCReference<tfrt::AsyncValue>, 4> inputs = {
        MakeAvailableAsyncValueRef<int>(1), MakeAvailableAsyncValueRef<int>(0)};

    auto outputs = model->transform(inputs, *exec_context);
    host_context->Await(outputs);
    SparseVector &actual_vector = outputs[0]->get<SparseVector>();
    EXPECT_EQ(actual_vector, expected_vector);
  }

  {
    llvm::SmallVector<tfrt::RCReference<tfrt::AsyncValue>, 4> inputs = {
        MakeAvailableAsyncValueRef<int>(5), MakeAvailableAsyncValueRef<int>(5)};

    auto outputs = model->transform(inputs, *exec_context);
    host_context->Await(outputs);
    EXPECT_TRUE(outputs[0]->IsError());
  }
}

TEST_F(OneHotEncoderTest, Load) {
  test::TemporaryFolder tmp_folder;

  nlohmann::json params;
  params["paramMap"]["dropLast"] = "false";

  OneHotEncoderModelDataProto model_data;
  model_data.add_featuresizes(2);
  model_data.add_featuresizes(3);

  test::saveMetaDataModelData(tmp_folder.getAbsolutePath(), params, model_data);

  auto model =
      OneHotEncoderModel::load(tmp_folder.getAbsolutePath(), host_context);
  EXPECT_FALSE((bool)model.takeError());

  SparseVector expected_vector(2);
  expected_vector.set(1, 1.0);

  llvm::SmallVector<tfrt::RCReference<tfrt::AsyncValue>, 4> inputs = {
      MakeAvailableAsyncValueRef<int>(1), MakeAvailableAsyncValueRef<int>(0)};

  auto outputs = model.get()->transform(inputs, *exec_context);
  host_context->Await(outputs);
  SparseVector &actual_vector = outputs[0]->get<SparseVector>();
  EXPECT_EQ(actual_vector, expected_vector);
}

TEST_F(OneHotEncoderTest, Mlir) {
  test::TemporaryFolder tmp_folder;

  nlohmann::json params;
  params["paramMap"]["dropLast"] = "false";

  OneHotEncoderModelDataProto model_data;
  model_data.add_featuresizes(2);
  model_data.add_featuresizes(3);

  test::saveMetaDataModelData(tmp_folder.getAbsolutePath(), params, model_data);

  const std::string model_load_script = R"mlir(
    func @main(%path: !tfrt.string) -> !clink.model {
      %model = clink.load.onehotencoder %path
      tfrt.return %model : !clink.model
    }
  )mlir";

  clink::ClinkRunner::Builder builder;
  builder.set_mlir_fn_name("main")
      .set_mlir_input(model_load_script)
      .set_host_context(host_context)
      .set_mlir_context(mlir_context);
  auto model_load_runner = builder.Compile();

  llvm::SmallVector<RCReference<AsyncValue>> model_load_inputs{
      tfrt::MakeAvailableAsyncValueRef<std::string>(
          tmp_folder.getAbsolutePath())};
  auto model_load_outputs = model_load_runner.Run(model_load_inputs);
  host_context->Await(model_load_outputs);
  auto model_ref = model_load_outputs[0];

  const std::string model_transform_script = R"mlir(
    func @main(%model: !clink.model, %inputs: !clink.arrayref) -> !clink.smallvector {
        %outputs = clink.transform %model, %inputs
        tfrt.return %outputs : !clink.smallvector
    }
  )mlir";

  builder.set_mlir_input(model_transform_script);
  auto model_transform_runner = builder.Compile();

  {
    llvm::SmallVector<tfrt::RCReference<tfrt::AsyncValue>, 4> inputs = {
        MakeAvailableAsyncValueRef<int>(1), MakeAvailableAsyncValueRef<int>(0)};

    llvm::SmallVector<RCReference<AsyncValue>, 4> mlir_inputs;
    mlir_inputs.push_back(model_ref);
    mlir_inputs.push_back(
        MakeAvailableAsyncValueRef<
            llvm::ArrayRef<tfrt::RCReference<tfrt::AsyncValue>>>(inputs));

    auto results = model_transform_runner.Run(mlir_inputs);
    host_context->Await(results);
    auto outputs =
        results[0]
            ->get<llvm::SmallVector<tfrt::RCReference<tfrt::AsyncValue>, 4>>();

    host_context->Await(outputs);
    SparseVector &actual_vector = outputs[0]->get<SparseVector>();
    SparseVector expected_vector(2);
    expected_vector.set(1, 1.0);
    EXPECT_EQ(actual_vector, expected_vector);
  }

  {
    llvm::SmallVector<tfrt::RCReference<tfrt::AsyncValue>, 4> inputs = {
        MakeAvailableAsyncValueRef<int>(5), MakeAvailableAsyncValueRef<int>(5)};

    llvm::SmallVector<RCReference<AsyncValue>, 4> mlir_inputs;
    mlir_inputs.push_back(model_ref);
    mlir_inputs.push_back(
        MakeAvailableAsyncValueRef<
            llvm::ArrayRef<tfrt::RCReference<tfrt::AsyncValue>>>(inputs));

    auto results = model_transform_runner.Run(mlir_inputs);
    host_context->Await(results);
    auto outputs =
        results[0]
            ->get<llvm::SmallVector<tfrt::RCReference<tfrt::AsyncValue>, 4>>();

    host_context->Await(outputs);
    EXPECT_TRUE(outputs[0]->IsError());
  }
}

} // namespace
} // namespace clink
