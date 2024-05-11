//===--- RepresentationLeakCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_REPRESENTATIONLEAKCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_REPRESENTATIONLEAKCHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::misc {

/// Checks whether a class leaks model, view or persistent component representation
/// via either a public field or function return type
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misc/representation-leak.html
class RepresentationLeakCheck : public ClangTidyCheck {
public:
  RepresentationLeakCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
private:
  void addCXXMethodMatcher(ast_matchers::MatchFinder *Finder, const std::string &Regex, const std::string & Id);
  void addFieldMatcher(ast_matchers::MatchFinder *Finder, const std::string &Regex, const std::string & Id);
};

} // namespace clang::tidy::misc

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_REPRESENTATIONLEAKCHECK_H
