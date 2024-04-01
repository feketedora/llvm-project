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

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

void StructuredNamespaceCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      cxxRecordDecl(unless(isExpansionInSystemHeader()),
                    matchesName("::.*[m|M]odel$"))
          .bind("model"),
      this);
}

void StructuredNamespaceCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedClass = Result.Nodes.getNodeAs<CXXRecordDecl>("model");
 // auto x = QualType::getAsString(
 //     MatchedClass->getASTContext().getTypeDeclType(MatchedClass).split(),
 //     PrintingPolicy());
  auto DeclCon = MatchedClass->getEnclosingNamespaceContext();
  bool mismatched = false;
  //while (DeclCon->isNamespace() && !mismatched) {
    //mismatched |= DeclCon->??? == "model";
    //DeclCon = DeclCon->getEnclosingNamespaceContext();
  //}

  if (mismatched) {
    diag(MatchedClass->getLocation(), "mismatched namespace and class");
  }
}

} // namespace clang::tidy::misc
