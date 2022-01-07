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

// This file defines utilities related to setting up unit tests.
#ifndef CLINK_CPP_TESTS_TEST_UTIL_H_
#define CLINK_CPP_TESTS_TEST_UTIL_H_

#include <dirent.h>
#include <fstream>
#include <sys/stat.h>

#include "clink/kernels/opdefs/clink_kernels.h"
#include "clink/utils/clink_runner.h"
#include "clink/utils/clink_utils.h"
#include "google/protobuf/message.h"
#include "nlohmann/json.hpp"
#include "tfrt/basic_kernels/opdefs/basic_kernels.h"

namespace clink {
namespace test {
namespace {
std::unique_ptr<MLIRContext> createTestMLIRContext() {
  auto context = std::make_unique<MLIRContext>();
  context->allowUnregisteredDialects();
  context->printOpOnDiagnostic(true);
  mlir::DialectRegistry registry;
  registry.insert<clink::ClinkDialect>();
  registry.insert<tfrt::compiler::TFRTDialect>();
  context->appendDialectRegistry(registry);
  return context;
}

} // namespace

std::unique_ptr<MLIRContext> test_mlir_context = createTestMLIRContext();

extern std::unique_ptr<tfrt::HostContext> test_host_context =
    CreateHostContext("mstd", tfrt::HostAllocatorType::kLeakCheckMalloc);

// This class represents a temporary folder used for unit tests. The folder will
// be deleted automatically once this object is freed.
class TemporaryFolder {
public:
  TemporaryFolder() {
    char dir_template[] = "/tmp/clink-test-tmp.XXXXXX";
    dir_name = std::string(mkdtemp(dir_template));
  }

  ~TemporaryFolder() { deleteFolderRecursively(dir_name); }

  const std::string getAbsolutePath() { return dir_name; }

private:
  void deleteFolderRecursively(std::string path) {
    struct dirent *entry;
    struct stat st;
    DIR *dir = opendir(path.c_str());

    if (dir == NULL) {
      return;
    }
    while ((entry = readdir(dir)) != NULL) {
      const std::string full_file_name = path + "/" + entry->d_name;
      if (stat(full_file_name.c_str(), &st) == -1)
        continue;
      bool is_directory = (st.st_mode & S_IFDIR) != 0;
      if (is_directory) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
          continue;
        deleteFolderRecursively(full_file_name);
      } else {
        remove(full_file_name.c_str());
      }
    }
    closedir(dir);
    remove(path.c_str());
  }

  std::string dir_name;
};

void saveMetaDataModelData(std::string dir_name, nlohmann::json params,
                           google::protobuf::Message &model_data) {
  std::ofstream params_output(dir_name + "/metadata");
  params_output << params;
  params_output.close();

  mkdir((dir_name + "/data").c_str(), S_IRUSR | S_IWUSR);
  std::ofstream model_data_output(
      dir_name + "/data/part-1234abcd-ef56-78gh-ij90-123abc45d6ef-0");
  model_data.SerializeToOstream(&model_data_output);
  model_data_output.close();
}

llvm::SmallVector<RCReference<AsyncValue>>
runMlirScript(string_view mlir_script,
              llvm::SmallVector<RCReference<AsyncValue>> inputs) {
  // Initializes ClinkRunner.
  clink::ClinkRunner::Builder builder;
  builder.set_mlir_fn_name("main")
      .set_mlir_input(mlir_script)
      .set_host_context(test_host_context.get())
      .set_mlir_context(test_mlir_context.get());
  auto runner = builder.Compile();

  return runner.Run(inputs);
}

} // namespace test
} // namespace clink

#endif // CLINK_CPP_TESTS_TEST_UTIL_H_
