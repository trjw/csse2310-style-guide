#ifndef DOUBLE_ASSIGNMENT_CHECK_H
#define DOUBLE_ASSIGNMENT_CHECK_H

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

namespace clang::tidy::csse2310 {

class DoubleAssignmentCheck : public ClangTidyCheck {
  public:
    DoubleAssignmentCheck(StringRef Name, ClangTidyContext* Context)
        : ClangTidyCheck(Name, Context) {}

    void registerMatchers(MatchFinder* Finder) override;
    void check(const MatchFinder::MatchResult& Result) override;
};

} // namespace clang::tidy::csse2310

#endif // DOUBLE_ASSIGNMENT_CHECK_H
