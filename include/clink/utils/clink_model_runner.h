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

#ifndef CLINK_UTILS_CLINK_MODEL_RUNNER_H_
#define CLINK_UTILS_CLINK_MODEL_RUNNER_H_

#include "clink/utils/clink_runner.h"

namespace clink {

// This class is a utility class that executes a given Model's transform method
// based on ClinkRunner.
class ClinkModelRunner {
public:
  // Applies the Model's transform() method on the provided input and returns
  // the transformation result.
  llvm::SmallVector<tfrt::RCReference<tfrt::AsyncValue>, 4>
  Run(llvm::ArrayRef<tfrt::RCReference<tfrt::AsyncValue>> inputs);

  // Creates a ClinkModelRunner object from the given context and data from the
  // given path.
  //
  // The path should be a directory containing params and model data saved
  // through the corresponding Java Model operator's save() method.
  //
  // The model name should correspond to the suffix of a registered clink.load
  // kernel, for example, `onehotencoder`.
  static ClinkModelRunner load(tfrt::HostContext *host_context,
                               mlir::MLIRContext *mlir_context,
                               const std::string &path,
                               const std::string &model_name);

private:
  // Use ClinkModelRunner::load() to get a ClinkModelRunner object.
  ClinkModelRunner(ClinkRunner &&transform_runner,
                   tfrt::HostContext *host_context,
                   RCReference<AsyncValue> model_ref)
      : transform_runner_(std::move(transform_runner)), host_(host_context),
        model_ref_(model_ref) {}

  tfrt::HostContext *host_;
  RCReference<AsyncValue> model_ref_;
  ClinkRunner transform_runner_;
};

} // namespace clink

#endif // CLINK_UTILS_CLINK_MODEL_RUNNER_H_
