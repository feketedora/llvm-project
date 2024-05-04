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
#include <string>
#include <iostream> // TODO remove

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

void RepresentationLeakCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxRecordDecl(unless(isExpansionInSystemHeader()),
                                   forEach(fieldDecl(isPublic(),
                                                     hasType(cxxRecordDecl(matchesName(utils::ModelRegex))))
                                            .bind("field"))),
                     this);
  Finder->addMatcher(cxxMethodDecl(isPublic(),
                                   unless(isExpansionInSystemHeader()))
                      .bind("func"),
                     this);
  Finder->addMatcher(cxxMethodDecl(hasName("saveGame")).bind("TEST"), this);
}

void RepresentationLeakCheck::check(const MatchFinder::MatchResult &Result) {
  auto *Test = Result.Nodes.getNodeAs<CXXMethodDecl>("TEST");
  if (Test != nullptr)
  {
    diag(Test->getLocation(), "TEST TEST TEST warning");
  }
  const auto *Field = Result.Nodes.getNodeAs<FieldDecl>("field");
  if (Field != nullptr)
  {
    diag(Field->getLocation(), "model is leaking (member)");
  }

  const auto *Func = Result.Nodes.getNodeAs<CXXMethodDecl>("func");
  if (Func != nullptr) {
      auto Ret = Func->getReturnType();
    if (auto TheType = Ret.getTypePtrOrNull(); TheType != nullptr) {
      if (TheType->isVoidType()) {
        return;
      }
      auto BTI = Ret.getBaseTypeIdentifier();
      if (BTI->getName().str() == "Model") {
        if (TheType->isPointerType()) {
          diag(Func->getLocation(), "model is leaking (pointer)");
        } else if (TheType->isReferenceType() && !Ret.isConstQualified()) {
          // TODO non-const???????
          std::cout << "we have a non-const reference return type\n";
          diag(Func->getLocation(), "model is leaking (reference)");
        }
      }
    }
  }
}

} // namespace clang::tidy::misc
