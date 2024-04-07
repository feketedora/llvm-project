//===--- PublicMembersCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PublicMembersCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <string>
#include <iostream>

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

void PublicMembersCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      cxxRecordDecl(unless(isExpansionInSystemHeader()),
                    forEach(
          fieldDecl(isPublic(), hasType(cxxRecordDecl(hasName("Model"))))
              .bind("field"))),
      this);
  Finder->addMatcher(
      cxxRecordDecl(unless(isExpansionInSystemHeader()),
                    forEach(cxxMethodDecl(isPublic())
                                .bind("func"))),
      this);
}

void PublicMembersCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Field = Result.Nodes.getNodeAs<FieldDecl>("field");
  if (Field != nullptr)
  {
    diag(Field->getLocation(), "model is leaking");
  }

  const auto *Func = Result.Nodes.getNodeAs<CXXMethodDecl>("func");
  if (Func != nullptr) {
      auto Ret = Func->getReturnType();
      if (auto TheType = Ret.getTypePtrOrNull(); TheType != nullptr)
      {
          if (TheType->isVoidType())
          {
             return;
          }
          auto BTI = Ret.getBaseTypeIdentifier();
          if (BTI->getName().str() == "Model")
          {
              if (TheType->isPointerType())
              {
                  diag(Func->getLocation(), "model is leaking");
              }
              else if (TheType->isReferenceType() && !Ret.isLocalConstQualified())
              {
                  // TODO non-const???????
                  std::cout << "we have a non-const reference return type\n";
                  diag(Func->getLocation(), "model is leaking");
              }
          }
      }
  }
}

} // namespace clang::tidy::misc
