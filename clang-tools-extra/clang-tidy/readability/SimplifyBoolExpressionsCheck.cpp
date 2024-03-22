//===--- SimplifyBoolExpressionsCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "SimplifyBoolExpressionsCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::readability {

void SimplifyBoolExpressionsCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(binaryOperator(hasOperatorName("=="),
                                    hasLHS(has(cxxBoolLiteral(equals(true))))).bind("eq-left-true"), this);
  Finder->addMatcher(binaryOperator(hasOperatorName("=="),
                                    hasRHS(has(cxxBoolLiteral(equals(true))))).bind("eq-right-true"), this);
  Finder->addMatcher(binaryOperator(hasOperatorName("=="),
                                    hasLHS(has(cxxBoolLiteral(equals(false))))).bind("eq-left-false"), this);
  Finder->addMatcher(binaryOperator(hasOperatorName("=="),
                                    hasRHS(has(cxxBoolLiteral(equals(false))))).bind("eq-right-false"), this);
  Finder->addMatcher(binaryOperator(hasOperatorName("!="),
                                    hasLHS(has(cxxBoolLiteral(equals(true))))).bind("neq-left-true"), this);
  Finder->addMatcher(binaryOperator(hasOperatorName("!="),
                                    hasRHS(has(cxxBoolLiteral(equals(true))))).bind("neq-right-true"), this);
  Finder->addMatcher(binaryOperator(hasOperatorName("!="),
                                    hasLHS(has(cxxBoolLiteral(equals(false))))).bind("neq-left-false"), this);
  Finder->addMatcher(binaryOperator(hasOperatorName("!="),
                                    hasRHS(has(cxxBoolLiteral(equals(false))))).bind("neq-right-false"), this);
}

void SimplifyBoolExpressionsCheck::checkOne(BinaryOperator const * Match, const MatchFinder::MatchResult &Result,
                                            Expr const * Replacement, bool negate)
{
    auto Diag = diag(Match->getBeginLoc(), "expression can be simplified");
    auto Text = Lexer::getSourceText(CharSourceRange::getTokenRange(Replacement->getSourceRange()),
                                           *Result.SourceManager, Result.Context->getLangOpts());
    if (negate)
    {
      Diag << FixItHint::CreateInsertion(Match->getLHS()->getBeginLoc(), "!");
    }
    const auto Fix = FixItHint::CreateReplacement(
        CharSourceRange::getTokenRange(SourceRange(Match->getLHS()->getBeginLoc(), Match->getRHS()->getEndLoc())), Text);

    Diag << Fix;
}

void SimplifyBoolExpressionsCheck::check(const MatchFinder::MatchResult &Result) {
  const auto * Match = Result.Nodes.getNodeAs<BinaryOperator>("eq-left-true");
  if (Match != nullptr)
  {
    checkOne(Match, Result, Match->getRHS(), false);
  }

  Match = Result.Nodes.getNodeAs<BinaryOperator>("eq-right-true");
  if (Match != nullptr)
  {
    checkOne(Match, Result, Match->getLHS(), false);
  }

  Match = Result.Nodes.getNodeAs<BinaryOperator>("eq-left-false");
  if (Match != nullptr)
  {
    checkOne(Match, Result, Match->getRHS(), true);
  }

  Match = Result.Nodes.getNodeAs<BinaryOperator>("eq-right-false");
  if (Match != nullptr)
  {
    checkOne(Match, Result, Match->getLHS(), true);
  }

  Match = Result.Nodes.getNodeAs<BinaryOperator>("neq-left-true");
  if (Match != nullptr)
  {
    checkOne(Match, Result, Match->getRHS(), true);
  }

  Match = Result.Nodes.getNodeAs<BinaryOperator>("neq-right-true");
  if (Match != nullptr)
  {
    checkOne(Match, Result, Match->getLHS(), true);
  }

  Match = Result.Nodes.getNodeAs<BinaryOperator>("neq-left-false");
  if (Match != nullptr)
  {
    checkOne(Match, Result, Match->getRHS(), false);
  }

  Match = Result.Nodes.getNodeAs<BinaryOperator>("neq-right-false");
  if (Match != nullptr)
  {
    checkOne(Match, Result, Match->getLHS(), false);
  }

}

} // namespace clang::tidy::readability
