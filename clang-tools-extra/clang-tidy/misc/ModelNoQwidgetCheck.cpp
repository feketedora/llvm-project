//===--- ModelNoQwidgetCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ModelNoQwidgetCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "../utils/Utilities.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

void ModelNoQwidgetCheck::registerMatchers(MatchFinder *Finder) {
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
  auto ModelMatch = matchesName(utils::ModelRegex);
  auto ModelRecord = cxxRecordDecl(ModelMatch);

  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   ModelMatch,
                                   DerivedFromQWidget)
                      .bind("qwidget-class"),
                     this);
  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   ModelMatch,
                                   forEach(fieldDecl(CxxRecordDerivedFromQWidgetType)
                                            .bind("qwidget-field"))),
                     this);
  Finder->addMatcher(cxxMethodDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   ofClass(ModelRecord),
                                   forEachDescendant(varDecl(CxxRecordDerivedFromQWidgetType)
                                                      .bind("qwidget-variable"))),
                     this);
  Finder->addMatcher(cxxMethodDecl(unless(isExpansionInSystemHeader()),
                                   isDefinition(),
                                   ofClass(ModelRecord),
                                   CxxRecordDerivedFromQWidgetReturns)
                      .bind("qwidget-return-type"),
                     this);
}

void ModelNoQwidgetCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedClass = Result.Nodes.getNodeAs<CXXRecordDecl>("qwidget-class");
  if (MatchedClass != nullptr) {
    diag(MatchedClass->getLocation(), "model class must not derive from QWidget");
  }
  const auto *MatchedField = Result.Nodes.getNodeAs<FieldDecl>("qwidget-field");
  if (MatchedField != nullptr) {
    diag(MatchedField->getLocation(), "model class must not have QWidget or its descendant fields");
  }
  const auto *MatchedVariable = Result.Nodes.getNodeAs<VarDecl>("qwidget-variable");
  if (MatchedVariable != nullptr) {
    diag(MatchedVariable->getLocation(), "model class must not use QWidget or its descendants");
  }
  const auto *MatchedReturnType = Result.Nodes.getNodeAs<CXXMethodDecl>("qwidget-return-type");
  if (MatchedReturnType != nullptr) {
    diag(MatchedReturnType->getLocation(), "model class methods must not return QWidget or its descendants");
  }
}

} // namespace clang::tidy::misc
