//===--- PersistenceStreamCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PersistenceStreamCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

void PersistenceStreamCheck::registerMatchers(MatchFinder *Finder) {
  auto AnyOf = anyOf(
      matchesName("[f|F]ile"),
      hasType(asString("std::fstream")),
      hasType(asString("std::ifstream")),
      hasType(asString("std::ofstream")),
      hasType(cxxRecordDecl(isSameOrDerivedFrom("std::fstream"))),
      hasType(cxxRecordDecl(isSameOrDerivedFrom("std::ifstream"))),
      hasType(cxxRecordDecl(isSameOrDerivedFrom("std::ofstream"))));
  Finder->addMatcher(
      recordDecl(unless(isExpansionInSystemHeader()),
                 unless(matchesName("[d|D]ata[a|A]ccess|[P|p]ersiste[nce|r]")),
                 forEach(fieldDecl(AnyOf).bind("file-field"))),
      this);
  Finder->addMatcher(
      recordDecl(unless(isExpansionInSystemHeader()),
                 unless(matchesName("[d|D]ata[a|A]ccess|[P|p]ersiste[nce|r]")),
                 forEach(functionDecl(hasAnyParameter(AnyOf)).bind("file-param"))),
      this);
}

void PersistenceStreamCheck::check(const MatchFinder::MatchResult &Result) {
    const auto * FileField = Result.Nodes.getNodeAs<FieldDecl>("file-field");
    if (FileField != nullptr) {
      diag(FileField->getLocation(), "found file stream field outside persistence class");
    }
    const auto *FunctionWithFileParam = Result.Nodes.getNodeAs<FunctionDecl>("file-param");
    if (FunctionWithFileParam != nullptr) {
      diag(FunctionWithFileParam->getLocation(),
           "function has file stream parameter outside persistence class");
    }
}

} // namespace clang::tidy::misc
