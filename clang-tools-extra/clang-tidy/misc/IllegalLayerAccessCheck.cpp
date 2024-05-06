//===--- IllegalLayerAccessCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IllegalLayerAccessCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "../utils/Utilities.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

namespace {
  using Record = std::tuple<std::string, std::string, std::string, std::string>;
  const std::vector<Record> Data = {
    {utils::ModelNodeId, utils::ViewNodeId, utils::ModelRegex, utils::ViewRegex},
    {utils::ViewNodeId, utils::PersistenceNodeId, utils::ViewRegex, utils::PersistenceRegex}
  };
  std::string getId(const Record & R) {
    return std::get<0>(R) + "-" + std::get<1>(R);
  }
}

void IllegalLayerAccessCheck::addMatchers(MatchFinder *Finder, const std::string & ContainerRegex,
                                          const std::string & ContainedRegex, const std::string & Id) {
  auto ContainerMatch = allOf(matchesName(ContainerRegex), unless(matchesName(utils::UiPrefixRegex)));
  auto ContainerRecord = cxxRecordDecl(ContainerMatch);
  auto ContainedRecord = cxxRecordDecl(isSameOrDerivedFrom(matchesName(ContainedRegex)),
                                       unless(matchesName(utils::UiPrefixRegex)));
  auto ContainedTypes = anyOf(hasType(ContainedRecord),
                              hasType(pointsTo(ContainedRecord)),
                              hasType(references(ContainedRecord)));

  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   ContainerMatch,
                                   forEach(fieldDecl(ContainedTypes)
                                            .bind(Id + "-illegal-field"))),
                     this);
  Finder->addMatcher(cxxMethodDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   ofClass(ContainerRecord),
                                   forEachDescendant(varDecl(ContainedTypes)
                                                      .bind(Id + "-illegal-variable"))),
                     this);
}

void IllegalLayerAccessCheck::registerMatchers(MatchFinder *Finder) {
  for (auto const & R : Data) {
    addMatchers(Finder, std::get<2>(R), std::get<3>(R), getId(R));
  }
}

void IllegalLayerAccessCheck::check(const MatchFinder::MatchResult &Result) {
  for (auto const & R : Data) {
    auto Id = getId(R);
    const auto *MatchedField = Result.Nodes.getNodeAs<FieldDecl>(Id + "-illegal-field");
    if (MatchedField != nullptr) {
      diag(MatchedField->getLocation(), std::get<0>(R) + " class must not have " + std::get<1>(R) + " or its descendant fields");
    }
    const auto *MatchedVariable = Result.Nodes.getNodeAs<VarDecl>(Id + "-illegal-variable");
    if (MatchedVariable != nullptr) {
      diag(MatchedVariable->getLocation(), std::get<0>(R) + " class must not use " + std::get<1>(R) + " or its descendants");
    }
  }
}

} // namespace clang::tidy::misc
