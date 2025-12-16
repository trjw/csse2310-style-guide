#ifndef STATIC_LOCAL_CHECK_H
#define STATIC_LOCAL_CHECK_H

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

namespace clang::tidy::csse2310 {

class StaticLocalCheck : public ClangTidyCheck {
  public:
    StaticLocalCheck(StringRef Name, ClangTidyContext* Context)
        : ClangTidyCheck(Name, Context) {}

    void registerMatchers(MatchFinder* Finder) override;
    void check(const MatchFinder::MatchResult& Result) override;
};

} // namespace clang::tidy::csse2310

#endif // STATIC_LOCAL_CHECK_H
