/*
 * Copyright 2021 Andrew Rossignol andrew.rossignol@gmail.com
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "nerfnet/util/file.h"

#include <fstream>

namespace nerfnet {

bool ReadFileToString(const std::string& filename, std::string* contents) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (!in) {
    return false;
  }

  in.seekg(0, std::ios::end);
  contents->reserve(in.tellg());
  in.seekg(0, std::ios::beg);
  contents->assign((std::istreambuf_iterator<char>(in)),
                   std::istreambuf_iterator<char>());
  return true;
}

}  // namespace nerfneg
