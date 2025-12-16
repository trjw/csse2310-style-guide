#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "StaticLocalCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::tidy::csse2310;
using namespace clang::ast_matchers;

void StaticLocalCheck::registerMatchers(MatchFinder* Finder)
{
    // Match every function definition
    Finder->addMatcher(traverse(TK_IgnoreUnlessSpelledInSource,
                               varDecl(isStaticLocal()).bind("staticlocal")),
            this);
}

void StaticLocalCheck::check(const MatchFinder::MatchResult& Result)
{
    const auto* varDecl = Result.Nodes.getNodeAs<VarDecl>("staticlocal");
    if (!varDecl) {
        return;
    }

    diag(varDecl->getLocation(), "Static local variable found: %0")
            << varDecl->getName(),
            DiagnosticIDs::Warning;
}
