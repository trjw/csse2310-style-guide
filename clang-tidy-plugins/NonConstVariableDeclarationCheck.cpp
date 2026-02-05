#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "NonConstVariableDeclarationCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::tidy::csse2310;
using namespace clang::ast_matchers;

void NonConstVariableDeclarationCheck::registerMatchers(MatchFinder* Finder)
{
    // Find all declarations with two or more declarations in the same statement
    Finder->addMatcher(traverse(TK_IgnoreUnlessSpelledInSource,
                               declStmt(declCountIs(2), isExpansionInMainFile())
                                       .bind("decl")),
            this);
}

void NonConstVariableDeclarationCheck::check(
        const MatchFinder::MatchResult& Result)
{
    const auto* DS = Result.Nodes.getNodeAs<DeclStmt>("decl");
    if (DS) {

        // Check each declaration to make sure they only have constant
        // initialisers
        for (const Decl* D : DS->decls()) {
            if (const auto* VD = dyn_cast<VarDecl>(D)) {
                if (VD->hasInit()
                        && !VD->getInit()->isConstantInitializer(
                                *Result.Context, false)) {
                    diag(VD->getLocation(),
                            "Variable declarations with non-constant "
                            "initialisers should be on a line by themselves - "
                            "do not use a comma to separate these",
                            DiagnosticIDs::Warning);
                }
            }
        }
    }
}
