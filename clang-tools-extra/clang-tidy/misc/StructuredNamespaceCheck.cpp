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

void StructuredNamespaceCheck::addCXXRecordMatcher(MatchFinder *Finder, const std::string &Regex,
                                                   const std::string & Id) {
  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   isSameOrDerivedFrom(matchesName(Regex)))
                      .bind(Id),
                     this);
}

void StructuredNamespaceCheck::registerMatchers(MatchFinder *Finder) {
  addCXXRecordMatcher(Finder, utils::ModelRegex, utils::ModelNodeId);
  addCXXRecordMatcher(Finder, utils::ViewRegex, utils::ViewNodeId);
  addCXXRecordMatcher(Finder, utils::PersistenceRegex, utils::PersistenceNodeId);
}

void StructuredNamespaceCheck::checkOne(const MatchFinder::MatchResult &Result, const std::string &Id,
                                        const std::string &Layer, const std::string &Namespace) {
  const auto *MatchedClass = Result.Nodes.getNodeAs<CXXRecordDecl>(Id);
  if (MatchedClass == nullptr)
  {
    return;
  }
  auto Namespaces = utils::splitLowercase(MatchedClass->getQualifiedNameAsString(), "::");
  // the last element is the class name, should be left out
  for (int i = 0; i < Namespaces.size() - 1; ++i) {
    if (Namespaces[i].find(Namespace) != std::string::npos) {
      diag(MatchedClass->getLocation(), Id + " class cannot be in " + Layer + " namespace");
      break;
    }
  }
}

void StructuredNamespaceCheck::check(const MatchFinder::MatchResult &Result) {
  checkOne(Result, utils::ModelNodeId, utils::ViewNodeId, utils::ViewNodeId);
  checkOne(Result, utils::ViewNodeId, utils::ModelNodeId, utils::ModelNodeId);
  for (auto const & variant : utils::PersistenceVariants) {
    checkOne(Result, utils::ModelNodeId, utils::PersistenceNodeId, variant);
    checkOne(Result, utils::ViewNodeId, utils::PersistenceNodeId, variant);
  }
  checkOne(Result, utils::PersistenceNodeId, utils::ViewNodeId, utils::ViewNodeId);
  checkOne(Result, utils::PersistenceNodeId, utils::ModelNodeId, utils::ModelNodeId);
}

} // namespace clang::tidy::misc
