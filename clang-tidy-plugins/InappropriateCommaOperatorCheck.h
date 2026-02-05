#ifndef UNNECSSARY_COMMA_OPERATOR_CHECK_H
#define UNNECSSARY_COMMA_OPERATOR_CHECK_H

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

namespace clang::tidy::csse2310 {

class InappropriateCommaOperatorCheck : public ClangTidyCheck {
  public:
    InappropriateCommaOperatorCheck(StringRef Name, ClangTidyContext* Context)
        : ClangTidyCheck(Name, Context) {}

    void registerMatchers(MatchFinder* Finder) override;
    void check(const MatchFinder::MatchResult& Result) override;
};

} // namespace clang::tidy::csse2310

#endif // UNNECSSARY_COMMA_OPERATOR_CHECK_H
