//===--- PersistenceStreamCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PersistenceStreamCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "../utils/Utilities.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

void PersistenceStreamCheck::registerMatchers(MatchFinder *Finder) {
  auto PersistenceMatch = matchesName(utils::PersistenceRegex);
  auto FstreamRecord = cxxRecordDecl(isSameOrDerivedFrom("::std::fstream"));
  auto IfstreamRecord = cxxRecordDecl(isSameOrDerivedFrom("::std::ifstream"));
  auto OfstreamRecord = cxxRecordDecl(isSameOrDerivedFrom("::std::ofstream"));
  auto QFileRecord = cxxRecordDecl(isSameOrDerivedFrom("::QFile"));
  auto Types = anyOf(hasType(FstreamRecord), hasType(pointsTo(FstreamRecord)), hasType(references(FstreamRecord)),
                     hasType(IfstreamRecord), hasType(pointsTo(IfstreamRecord)), hasType(references(IfstreamRecord)),
                     hasType(OfstreamRecord), hasType(pointsTo(OfstreamRecord)), hasType(references(OfstreamRecord)),
                     hasType(QFileRecord), hasType(pointsTo(QFileRecord)), hasType(references(QFileRecord)));

  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   unless(PersistenceMatch),
                                   forEach(fieldDecl(Types)
                                            .bind("file-field"))),
                     this);
  Finder->addMatcher(cxxMethodDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   unless(ofClass(PersistenceMatch)),
                                   unless(ofClass(FstreamRecord)),
                                   unless(ofClass(IfstreamRecord)),
                                   unless(ofClass(OfstreamRecord)),
                                   unless(ofClass(QFileRecord)),
                                   forEachDescendant(varDecl(Types)
                                                      .bind("file-variable"))),
                     this);
}

void PersistenceStreamCheck::check(const MatchFinder::MatchResult &Result) {
  const auto * FileField = Result.Nodes.getNodeAs<FieldDecl>("file-field");
  if (FileField != nullptr) {
    diag(FileField->getLocation(), "file handling field outside persistence class");
  }
  const auto *FileVar = Result.Nodes.getNodeAs<VarDecl>("file-variable");
  if (FileVar != nullptr) {
    diag(FileVar->getLocation(), "function handles files outside persistence class");
  }
}

} // namespace clang::tidy::misc
