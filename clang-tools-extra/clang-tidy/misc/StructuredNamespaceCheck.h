//===--- StructuredNamespaceCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_STRUCTUREDNAMESPACECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_STRUCTUREDNAMESPACECHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::misc {

/// Checks whether the components (model, view, persistence) belong to
/// the correct corresponding namespace structure, if any namespaces exist
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misc/structured-namespace.html
class StructuredNamespaceCheck : public ClangTidyCheck {
public:
  StructuredNamespaceCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void checkOne(const ast_matchers::MatchFinder::MatchResult &Result, const std::string &Layer,
                const std::string &Id, const std::string &Namespace);
  void addCXXRecordMatcher(ast_matchers::MatchFinder *Finder, const std::string &Regex, const std::string &Id);
};

} // namespace clang::tidy::misc

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_STRUCTUREDNAMESPACECHECK_H
