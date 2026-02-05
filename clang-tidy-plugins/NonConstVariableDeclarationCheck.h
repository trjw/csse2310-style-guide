#ifndef NON_CONST_VARIABLE_DECLARATION_CHECK_H
#define NON_CONST_VARIABLE_DECLARATION_CHECK_H

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

namespace clang::tidy::csse2310 {

class NonConstVariableDeclarationCheck : public ClangTidyCheck {
  public:
    NonConstVariableDeclarationCheck(StringRef Name, ClangTidyContext* Context)
        : ClangTidyCheck(Name, Context) {}

    void registerMatchers(MatchFinder* Finder) override;
    void check(const MatchFinder::MatchResult& Result) override;
};

} // namespace clang::tidy::csse2310

#endif // NON_CONST_VARIABLE_DECLARATION_CHECK_H
