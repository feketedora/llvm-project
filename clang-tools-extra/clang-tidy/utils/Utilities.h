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
#include <vector>

namespace clang::tidy::utils {

const std::string ModelRegex = "::.*[m|M]odel$";
const std::string ViewRegex = "::.*[v|V]iew$";
const std::string PersistenceRegex = "::.*((d|D)ata(a|A)ccess|(P|p)ersiste(nce|r))$";

const std::string ModelNodeId = "model";
const std::string ViewNodeId = "view";
const std::string PersistenceNodeId = "persistence";
const std::vector<std::string> PersistenceVariants = {PersistenceNodeId, "persister", "dataaccess"};

std::vector<std::string> splitLowercase(std::string const &s, std::string const &delimiter);

} // namespace clang::tidy::utils

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_UTILITIES_H

