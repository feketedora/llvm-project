//===---------- Utilities.h - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_UTILITIES_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_UTILITIES_H

#include <string>

namespace clang::tidy::utils {

const std::string ModelRegex = "::.*[m|M]odel$";

const std::string ModelNodeId = "model";

} // namespace clang::tidy::utils

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_UTILITIES_H

