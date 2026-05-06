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
                        isExpansionInMainFile()
                    ).bind("comma-op")
            ),
            this
    );
}

static const Stmt* getParentStmt(const Stmt* S, ASTContext& Ctx) {
    auto Parents = Ctx.getParents(*S);
    if (Parents.empty()) {
        return nullptr;
    }
    return Parents[0].get<Stmt>();
}

bool isInForHeader(const Stmt* Node, const ForStmt* FS, ASTContext& Ctx) {
    const Stmt* Init = FS->getInit();
    const Expr* Cond = FS->getCond();
    const Expr* Inc  = FS->getInc();

    const Stmt* Cur = Node;

    while (Cur) {
        // If we reach one of the header expressions first → allowed
        if (Cur == Init || Cur == Cond || Cur == Inc) {
            return true;
        }

        // If we reach the for-statement itself → we're in the body
        if (Cur == FS) {
            return false;
        }

        Cur = getParentStmt(Cur, Ctx);
    }

    return false;
}

bool isInWhileCondition(const Stmt* Node, const WhileStmt* WS, ASTContext& Ctx) {
    const Expr* Cond = WS->getCond();

    const Stmt* Cur = Node;

    while (Cur) {
        // If we reach the condition expression → allowed
        if (Cur == Cond) {
            return true;
        }

        // If we reach the while-statement itself → body
        if (Cur == WS) {
            return false;
        }

        Cur = getParentStmt(Cur, Ctx);
    }

    return false;
}

void InappropriateCommaOperatorCheck::check(
        const MatchFinder::MatchResult& Result)
{

    const auto* Op = Result.Nodes.getNodeAs<BinaryOperator>("comma-op");
    ASTContext& Ctx = *Result.Context;

    const Stmt* Cur = Op;

    while (Cur) {
        if (const auto* FS = llvm::dyn_cast<ForStmt>(Cur)) {
            if (isInForHeader(Op, FS, Ctx)) {
                return; // allowed
            }
            break;
        }

        if (const auto* WS = llvm::dyn_cast<WhileStmt>(Cur)) {
            if (isInWhileCondition(Op, WS, Ctx)) {
                return; // allowed
            }
            break;
        }

        Cur = getParentStmt(Cur, Ctx);
    }

    diag(Op->getOperatorLoc(),
            "Inappropriate use of the comma operator - use separate statements",
            DiagnosticIDs::Warning);
}
