#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "InappropriateCommaOperatorCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::tidy::csse2310;
using namespace clang::ast_matchers;

void InappropriateCommaOperatorCheck::registerMatchers(MatchFinder* Finder)
{
    // Match every function definition
    Finder->addMatcher(
            traverse(TK_IgnoreUnlessSpelledInSource,
                    binaryOperator(hasOperatorName(","),
                            isExpansionInMainFile(),

                            // Exclude use in for and while loops
                            // (Keep do-while loops)
                            unless(
                                anyOf(
                                    hasAncestor(forStmt(stmt())),
                                    hasAncestor(whileStmt(stmt()))
                                )
                            )
                        ).bind("comma-op")
            ),
        this);
}

void InappropriateCommaOperatorCheck::check(
        const MatchFinder::MatchResult& Result)
{
    const auto* commaOp = Result.Nodes.getNodeAs<BinaryOperator>("comma-op");
    if(!commaOp) {
        return;
    }
    diag(commaOp->getOperatorLoc(),
            "Inappropriate use of the comma operator - use separate statements",
            DiagnosticIDs::Warning);
}
