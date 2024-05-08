//===--- QwidgetBaseCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "QwidgetBaseCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "../utils/Utilities.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

void QwidgetBaseCheck::addNoQWidgetMatchers(ast_matchers::MatchFinder *Finder, const std::string & Regex, const std::string & Id) {
  auto DerivedFromQWidget = isSameOrDerivedFrom("::QWidget");
  auto CxxRecordDerivedFromQWidget = cxxRecordDecl(DerivedFromQWidget);
  auto CxxRecordDerivedFromQWidgetPointer = pointsTo(CxxRecordDerivedFromQWidget);
  auto CxxRecordDerivedFromQWidgetReference = references(CxxRecordDerivedFromQWidget);
  auto CxxRecordDerivedFromQWidgetType = anyOf(hasType(CxxRecordDerivedFromQWidget),
                                               hasType(CxxRecordDerivedFromQWidgetPointer),
                                               hasType(CxxRecordDerivedFromQWidgetReference));
  auto CxxRecordDerivedFromQWidgetReturns = returns(anyOf(hasDeclaration(CxxRecordDerivedFromQWidget),
                                                          qualType(CxxRecordDerivedFromQWidgetPointer),
                                                          qualType(CxxRecordDerivedFromQWidgetReference)));
  auto Match = matchesName(Regex);
  auto Record = cxxRecordDecl(Match, unless(matchesName(utils::UiPrefixRegex)));

  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   unless(matchesName(utils::UiPrefixRegex)),
                                   Match,
                                   DerivedFromQWidget)
                      .bind(Id + "-qwidget-class"),
                     this);
  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   unless(matchesName(utils::UiPrefixRegex)),
                                   Match,
                                   forEach(fieldDecl(CxxRecordDerivedFromQWidgetType)
                                            .bind(Id + "-qwidget-field"))),
                     this);
  Finder->addMatcher(cxxMethodDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   ofClass(Record),
                                   forEachDescendant(varDecl(CxxRecordDerivedFromQWidgetType)
                                                      .bind(Id + "-qwidget-variable"))),
                     this);
  Finder->addMatcher(cxxMethodDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   ofClass(Record),
                                   CxxRecordDerivedFromQWidgetReturns)
                      .bind(Id + "-qwidget-return-type"),
                     this);
}

void QwidgetBaseCheck::registerMatchers(MatchFinder *Finder) {
  addNoQWidgetMatchers(Finder, utils::ModelRegex, utils::ModelNodeId);
  addNoQWidgetMatchers(Finder, utils::PersistenceRegex, utils::PersistenceNodeId);
  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   unless(matchesName(utils::UiPrefixRegex)),
                                   matchesName(utils::ViewRegex),
                                   unless(isSameOrDerivedFrom("::QWidget")),
                                   unless(isSameOrDerivedFrom(matchesName(utils::UiPrefixRegex))))
                      .bind("view-no-qwidget-class"),
                     this);
}

void QwidgetBaseCheck::check(const MatchFinder::MatchResult &Result) {
  for (auto const & Id : {utils::ModelNodeId, utils::PersistenceNodeId}) {
    const auto *MatchedClass = Result.Nodes.getNodeAs<CXXRecordDecl>(Id + "-qwidget-class");
    if (MatchedClass != nullptr) {
      diag(MatchedClass->getLocation(), Id + " class must not derive from QWidget");
    }
    const auto *MatchedField = Result.Nodes.getNodeAs<FieldDecl>(Id + "-qwidget-field");
    if (MatchedField != nullptr) {
      diag(MatchedField->getLocation(), Id + " class must not have QWidget or its descendant fields");
    }
    const auto *MatchedVariable = Result.Nodes.getNodeAs<VarDecl>(Id + "-qwidget-variable");
    if (MatchedVariable != nullptr) {
      diag(MatchedVariable->getLocation(), Id + " class must not use QWidget or its descendants");
    }
    const auto *MatchedReturnType = Result.Nodes.getNodeAs<CXXMethodDecl>(Id + "-qwidget-return-type");
    if (MatchedReturnType != nullptr) {
      diag(MatchedReturnType->getLocation(), Id + " class methods must not return QWidget or its descendants");
    }
  }
  const auto *MatchedViewClass = Result.Nodes.getNodeAs<CXXRecordDecl>("view-no-qwidget-class");
  if (MatchedViewClass != nullptr) {
    diag(MatchedViewClass->getLocation(), "view class must derive from QWidget");
  }
}

} // namespace clang::tidy::misc
