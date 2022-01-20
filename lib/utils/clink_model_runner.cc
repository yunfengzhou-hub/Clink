/*
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

#include "clink/utils/clink_model_runner.h"
#include "tfrt/host_context/host_context.h"

namespace clink {

ClinkModelRunner ClinkModelRunner::load(tfrt::HostContext *host_context,
                                        mlir::MLIRContext *mlir_context,
                                        const std::string &path,
                                        const std::string &model_name) {
  std::string model_load_script = R"mlir(
    func @main(%path: !tfrt.string) -> !clink.model {
      %model = clink.load.${model_name} %path
      tfrt.return %model : !clink.model
    }
  )mlir";
  model_load_script.replace(model_load_script.find("${model_name}"),
                            strlen("${model_name}"), model_name);

  clink::ClinkRunner::Builder builder;
  builder.set_mlir_fn_name("main")
      .set_mlir_input(model_load_script)
      .set_host_context(host_context)
      .set_mlir_context(mlir_context);
  auto model_load_runner = builder.Compile();

  llvm::SmallVector<RCReference<AsyncValue>> model_load_inputs{
      tfrt::MakeAvailableAsyncValueRef<std::string>(path)};
  auto model_load_outputs = model_load_runner.Run(model_load_inputs);
  host_context->Await(model_load_outputs);
  auto model_ref = model_load_outputs[0];

  static const std::string model_transform_script = R"mlir(
    func @main(%model: !clink.model, %inputs: !clink.arrayref) -> !clink.smallvector {
        %outputs = clink.transform %model, %inputs
        tfrt.return %outputs : !clink.smallvector
    }
  )mlir";

  builder.set_mlir_input(model_transform_script);

  return ClinkModelRunner(builder.Compile(), host_context, model_ref);
}

llvm::SmallVector<tfrt::RCReference<tfrt::AsyncValue>, 4> ClinkModelRunner::Run(
    llvm::ArrayRef<tfrt::RCReference<tfrt::AsyncValue>> inputs) {
  llvm::SmallVector<RCReference<AsyncValue>, 4> mlir_inputs;
  mlir_inputs.push_back(model_ref_);
  mlir_inputs.push_back(
      MakeAvailableAsyncValueRef<
          llvm::ArrayRef<tfrt::RCReference<tfrt::AsyncValue>>>(inputs));

  auto results = transform_runner_.Run(mlir_inputs);
  host_->Await(results);
  return results[0]
      ->get<llvm::SmallVector<tfrt::RCReference<tfrt::AsyncValue>, 4>>();
}

} // namespace clink
