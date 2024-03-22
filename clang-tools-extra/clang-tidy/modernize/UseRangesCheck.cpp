//===--- UseRangesCheck.cpp - clang-tidy ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UseRangesCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"
#include "LoopConvertUtils.h"

using namespace clang::ast_matchers;

namespace clang::tidy::modernize {

void UseRangesCheck::registerMatchers(MatchFinder *Finder) {
  auto beginExpr = callExpr(callee(functionDecl(hasName("begin"))));
  auto endExpr = callExpr(callee(functionDecl(hasName("end"))));
  Finder->addMatcher(callExpr(callee(functionDecl(hasName("::std::copy"))),
                              has(implicitCastExpr(has(declRefExpr().bind("name")))),
                              hasArgument(0, beginExpr.bind("begin")),
                              hasArgument(1, endExpr.bind("end"))).bind("match"), this);
}

static const Expr * findContainerExprFromBeginEnd (const CallExpr * BeginEndExpr)
{
  // begin (xxx)
  // xxx.begin()
  const auto * Callee = BeginEndExpr->getCallee();
  if (const auto * Member = dyn_cast<MemberExpr>(Callee))
  {
    // xxx.begin()
    return Member->getBase();
  }
  else
  {
    // begin (xxx)
    return BeginEndExpr->getArg(0);
  }
}

static const Expr * findContainerExpr (const MatchFinder::MatchResult & Result)
{
  const auto * Begin = Result.Nodes.getNodeAs<CallExpr>("begin");
  const auto * End = Result.Nodes.getNodeAs<CallExpr>("end");
  const auto * BeginContainerExpr = findContainerExprFromBeginEnd(Begin);
  const auto * EndContainerExpr = findContainerExprFromBeginEnd(End);
  if (!areSameExpr(Result.Context, BeginContainerExpr, EndContainerExpr))
  {
    return nullptr;
  }
  return BeginContainerExpr;
}

void UseRangesCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchExpr = Result.Nodes.getNodeAs<CallExpr>("match");
  const auto *Name = Result.Nodes.getNodeAs<DeclRefExpr>("name");
  const auto * Begin = Result.Nodes.getNodeAs<CallExpr>("begin");
  const auto * End = Result.Nodes.getNodeAs<CallExpr>("end");
  auto Diag = diag(MatchExpr->getExprLoc(),
       "Consider to replace 'std::copy' by 'std::ranges::copy'")
      << FixItHint::CreateReplacement(Name->getSourceRange(), "std::ranges::copy");
  const auto * ContainerExpr = findContainerExpr(Result);
  if (ContainerExpr)
  {
    const auto ContainerText = Lexer::getSourceText(CharSourceRange::getTokenRange(ContainerExpr->getSourceRange()),
                                               *Result.SourceManager, Result.Context->getLangOpts());
    Diag << FixItHint::CreateReplacement(CharSourceRange::getTokenRange(Begin->getBeginLoc(), End->getEndLoc()), ContainerText);
  }
}

} // namespace clang::tidy::modernize
