#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "DoubleAssignmentCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::tidy::csse2310;
using namespace clang::ast_matchers;

void DoubleAssignmentCheck::registerMatchers(MatchFinder* Finder)
{
    // Match every function definition
    Finder->addMatcher(
            traverse(TK_IgnoreUnlessSpelledInSource,
                    binaryOperator(hasOperatorName("="),
                            hasRHS(binaryOperator(hasOperatorName("="))),
                            isExpansionInMainFile())
                            .bind("double-assignment")),
            this);
}

void DoubleAssignmentCheck::check(const MatchFinder::MatchResult& Result)
{
    const auto* assign
            = Result.Nodes.getNodeAs<BinaryOperator>("double-assignment");
    if (!assign) {
        return;
    }

    diag(assign->getOperatorLoc(),
            "Multiple assignment operations in a single statement",
            DiagnosticIDs::Warning)
            << assign->getSourceRange();
}
