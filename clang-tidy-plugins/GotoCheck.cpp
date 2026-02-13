#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "GotoCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::tidy::csse2310;
using namespace clang::ast_matchers;

void GotoCheck::registerMatchers(MatchFinder* Finder)
{
    // Match every function definition
    Finder->addMatcher(traverse(TK_IgnoreUnlessSpelledInSource,
                               gotoStmt().bind("goto")),
            this);
}

void GotoCheck::check(const MatchFinder::MatchResult& Result)
{
    const auto* gotoStmt = Result.Nodes.getNodeAs<GotoStmt>("goto");
    if (!gotoStmt) {
        return;
    }

    diag(gotoStmt->getBeginLoc(), "Use of 'goto' found", 
            DiagnosticIDs::Error);
}
