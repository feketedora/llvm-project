//===--- ModelIsNoQwidgetCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_QWIDGETBASECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_QWIDGETBASECHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::misc {

/// Checks whether the persistence and model components do not derive from or use QWidgets,
/// in case of the view component, checks whether it has QWidget as base class
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misc/qwidget-base.html
class QwidgetBaseCheck : public ClangTidyCheck {
public:
  QwidgetBaseCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
private:
  void addNoQWidgetMatchers(ast_matchers::MatchFinder *Finder, const std::string & Regex, const std::string & Id);
};

} // namespace clang::tidy::misc

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_QWIDGETBASECHECK_H
