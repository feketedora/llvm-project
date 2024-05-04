//===---------- Utilities.cpp - clang-tidy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Utilities.h"
#include <algorithm>

namespace clang::tidy::utils {

std::vector<std::string> splitLowercase(std::string const &s, std::string const &delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    std::transform(token.begin(), token.end(), token.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    res.push_back(token);
    pos_start = pos_end + delim_len;
  }

  res.push_back(s.substr(pos_start));
  return res;
}

} // namespace clang::tidy::utils
