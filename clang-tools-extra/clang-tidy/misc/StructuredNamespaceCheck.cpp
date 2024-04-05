//===--- StructuredNamespaceCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StructuredNamespaceCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <iostream>
#include <algorithm>
#include <string>

using namespace clang::ast_matchers;

namespace {
std::vector<std::string> splitLowercase(std::string const &s,
                                        std::string const &delimiter) {
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
}

namespace clang::tidy::misc {

void StructuredNamespaceCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      cxxRecordDecl(unless(isExpansionInSystemHeader()),
                    isDefinition(),
                    matchesName("::.*[m|M]odel$"))
          .bind("model"),
      this);
  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   matchesName("::.*[v|V]iew$"))
                         .bind("view"),
                     this);
}

void StructuredNamespaceCheck::checkOne(const MatchFinder::MatchResult& Result, std::string const& match, std::string const& compareTo)
{
  const auto *MatchedClass = Result.Nodes.getNodeAs<CXXRecordDecl>(match);
  if (MatchedClass == nullptr)
  {
    return;
  }
  auto namespaces =
      splitLowercase(MatchedClass->getQualifiedNameAsString(), "::");

  // the last part is the class name
  for (int i = 0; i < namespaces.size() - 1; ++i) {
    if (namespaces[i].find(compareTo) != std::string::npos) {
      std::string message =
          match + " class cannot be in " + compareTo + " namespace";
      diag(MatchedClass->getLocation(), message);
    }
  }
}

void StructuredNamespaceCheck::check(const MatchFinder::MatchResult &Result) {
  checkOne(Result, "model", "view");
  checkOne(Result, "view", "model");
}

} // namespace clang::tidy::misc
