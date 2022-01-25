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

// Mocks Flink ML's `Stage.save(String path)` method to save metadata and model
// data of an Clink operator to a given directory.
void saveMetaDataModelData(std::string dir_name, nlohmann::json params,
                           google::protobuf::Message &model_data) {
  std::ofstream params_output(dir_name + "/metadata");
  params_output << params;
  params_output.close();

  mkdir((dir_name + "/data").c_str(), S_IRUSR | S_IWUSR);
  std::ofstream model_data_output(dir_name + "/data/part-0");
  model_data.SerializeToOstream(&model_data_output);
  model_data_output.close();
}

} // namespace test
} // namespace clink

#endif // CLINK_CPP_TESTS_TEST_UTIL_H_
