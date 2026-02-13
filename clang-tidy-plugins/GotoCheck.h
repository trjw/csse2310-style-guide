#ifndef GOTO_CHECK_H
#define GOTO_CHECK_H

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

namespace clang::tidy::csse2310 {

class GotoCheck : public ClangTidyCheck {
  public:
    GotoCheck(StringRef Name, ClangTidyContext* Context)
        : ClangTidyCheck(Name, Context) {}

    void registerMatchers(MatchFinder* Finder) override;
    void check(const MatchFinder::MatchResult& Result) override;
};

} // namespace clang::tidy::csse2310

#endif // GOTO_CHECK_H
