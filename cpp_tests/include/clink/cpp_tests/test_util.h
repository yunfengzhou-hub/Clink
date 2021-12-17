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

#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <stdio.h>

namespace clink {
namespace test {

std::string createTemporaryFolder() {
  char dir_template[] = "/tmp/clink-test-tmp.XXXXXX";
  std::string dir_name = std::string(mkdtemp(dir_template));
  return dir_name;
}

// Implementation referenced from https://stackoverflow.com/a/440240.
std::string generateRandomString() {
  const int len = 6;
  static const char alphanum[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";
  std::string tmp_s;
  tmp_s.reserve(len);

  for (int i = 0; i < len; ++i) {
    tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return tmp_s;
}

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

} // namespace test
} // namespace clink

#endif // CLINK_CPP_TESTS_TEST_UTIL_H_
