//===--- PublicMembersCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PublicMembersCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "../utils/Utilities.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

void PublicMembersCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   isClass(),
                                   unless(matchesName(utils::UiPrefixRegex)),
                                   unless(matchesName(utils::DataRegex)),
                                   forEach(fieldDecl(isPublic())
                                            .bind("public-field"))),
                     this);
}

void PublicMembersCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FieldDecl>("public-field");
  if (MatchedDecl != nullptr) {
    diag(MatchedDecl->getBeginLoc(), "avoid using public members, use private members with getter and setter methods instead");
  }
}

} // namespace clang::tidy::misc
