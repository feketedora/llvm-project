//===--- EarlyExitCheck.cpp - clang-tidy ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "EarlyExitCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::readability {

void EarlyExitCheck::registerMatchers(MatchFinder *Finder) {
  auto Compound = compoundStmt(forEach(
                                   ifStmt(hasCondition(expr().bind("cond")),
                                          hasThen(compoundStmt().bind("then")),
                                          unless(hasConditionVariableStatement(declStmt())),
                                          unless(hasElse(stmt()))).bind("if"))).bind("comp");
  Finder->addMatcher(functionDecl(has(Compound)).bind("func"), this);
  Finder->addMatcher(forStmt(has(Compound)).bind("for"), this);
}

void EarlyExitCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *If = Result.Nodes.getNodeAs<IfStmt>("if");
  const auto *Compound = Result.Nodes.getNodeAs<CompoundStmt>("comp");
  if (If != Compound->body_back()) return;

  const auto * Condition = Result.Nodes.getNodeAs<Expr>("cond");
  const auto FixCondStart = FixItHint::CreateInsertion(Condition->getBeginLoc(), "!(");
  const auto FixCondEnd = FixItHint::CreateInsertion(Lexer::getLocForEndOfToken(Condition->getEndLoc(), 0, *Result.SourceManager, Result.Context->getLangOpts()), ")");

  const auto * Then = Result.Nodes.getNodeAs<CompoundStmt>("then");
  const auto OpeningBraceRange = CharSourceRange::getTokenRange(SourceRange(Then->getBeginLoc(), Then->getBeginLoc()));
  const auto FixExit = FixItHint::CreateReplacement(OpeningBraceRange, "return;");

  const auto ClosingBraceRange = CharSourceRange::getTokenRange(SourceRange(Then->getEndLoc(), Then->getEndLoc()));
  const auto FixBrace = FixItHint::CreateRemoval(ClosingBraceRange);

  diag(If->getBeginLoc(), "use early exit") << FixCondStart << FixCondEnd << FixExit << FixBrace;
}


} // namespace clang::tidy::readability
