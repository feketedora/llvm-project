//===--- RepresentationLeakCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RepresentationLeakCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "../utils/Utilities.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

void RepresentationLeakCheck::addCXXMethodMatcher(MatchFinder *Finder, const std::string &Regex,
                                                  const std::string & Id) {
  auto RecordType = cxxRecordDecl(isSameOrDerivedFrom(matchesName(Regex)), unless(matchesName(utils::UiPrefixRegex)));
  auto PointerReturnType = returns(pointsTo(RecordType));
  auto NonConstReferenceReturnType = allOf(returns(references(RecordType)),
                                           returns(referenceType(pointee(unless(isConstQualified())))));
  Finder->addMatcher(cxxMethodDecl(unless(isExpansionInSystemHeader()),
                                   isPublic(),
                                   unless(hasName("operator=")),
                                   anyOf(PointerReturnType, NonConstReferenceReturnType))
                      .bind(Id + "-leaking-return-type"),
                     this);
}

void RepresentationLeakCheck::addFieldMatcher(MatchFinder *Finder, const std::string &Regex,
                                              const std::string & Id) {
  auto RecordType = cxxRecordDecl(isSameOrDerivedFrom(matchesName(Regex)), unless(matchesName(utils::UiPrefixRegex)));
  auto FieldType = anyOf(hasType(RecordType),
                         hasType(pointsTo(RecordType)),
                         hasType(references(RecordType)));
  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   unless(matchesName(utils::UiPrefixRegex)),
                                   forEach(fieldDecl(isPublic(),
                                                     FieldType)
                                            .bind(Id + "-leaking-field"))),
                     this);
}

void RepresentationLeakCheck::registerMatchers(MatchFinder *Finder) {
  addCXXMethodMatcher(Finder, utils::ModelRegex, utils::ModelNodeId);
  addCXXMethodMatcher(Finder, utils::ViewRegex, utils::ViewNodeId);
  addCXXMethodMatcher(Finder, utils::PersistenceRegex, utils::PersistenceNodeId);

  addFieldMatcher(Finder, utils::ModelRegex, utils::ModelNodeId);
  addFieldMatcher(Finder, utils::ViewRegex, utils::ViewNodeId);
  addFieldMatcher(Finder, utils::PersistenceRegex, utils::PersistenceNodeId);
}

void RepresentationLeakCheck::check(const MatchFinder::MatchResult &Result) {
  for (auto const & Id : utils::LayerIds) {
    const auto *LeakingReturnType = Result.Nodes.getNodeAs<CXXMethodDecl>(Id + "-leaking-return-type");
    if (LeakingReturnType != nullptr) {
      diag(LeakingReturnType->getLocation(), "method leaks representation via return type");
    }
    const auto *LeakingField = Result.Nodes.getNodeAs<FieldDecl>(Id + "-leaking-field");
    if (LeakingField != nullptr) {
      diag(LeakingField->getLocation(), "field leaks representation");
    }
  }
}

} // namespace clang::tidy::misc
