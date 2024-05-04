//===--- StructuredNamespaceCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StructuredNamespaceCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "../utils/Utilities.h"
#include <string>

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

void StructuredNamespaceCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   matchesName(utils::ModelRegex))
                      .bind(utils::ModelNodeId),
                     this);
  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   matchesName(utils::ViewRegex))
                      .bind(utils::ViewNodeId),
                     this);
}

void StructuredNamespaceCheck::checkOne(const MatchFinder::MatchResult& Result, std::string const& match, std::string const& compareTo) {
  const auto *MatchedClass = Result.Nodes.getNodeAs<CXXRecordDecl>(match);
  if (MatchedClass == nullptr)
  {
    return;
  }
  auto Namespaces = utils::splitLowercase(MatchedClass->getQualifiedNameAsString(), "::");

  // the last element is the class name
  for (int i = 0; i < Namespaces.size() - 1; ++i) {
    if (Namespaces[i].find(compareTo) != std::string::npos) {
      std::string message = match + " class cannot be in " + compareTo + " namespace";
      diag(MatchedClass->getLocation(), message);
    }
  }
}

void StructuredNamespaceCheck::check(const MatchFinder::MatchResult &Result) {
  checkOne(Result, utils::ModelNodeId, utils::ViewNodeId);
  checkOne(Result, utils::ViewNodeId, utils::ModelNodeId);
}

} // namespace clang::tidy::misc
