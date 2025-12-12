#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

class StaticLocalCheck : public ClangTidyCheck {
  public:
    StaticLocalCheck(StringRef Name, ClangTidyContext* Context)
        : ClangTidyCheck(Name, Context)
    {
    }

    void registerMatchers(MatchFinder* Finder) override
    {
        // Match every function definition
        Finder->addMatcher(
                traverse(TK_IgnoreUnlessSpelledInSource,
                        varDecl(isStaticLocal()).bind("staticlocal")),
                this);
    }

    void check(const MatchFinder::MatchResult& Result) override
    {
        const auto* varDecl = Result.Nodes.getNodeAs<VarDecl>("staticlocal");
        if (!varDecl) {
            return;
        }

        diag(varDecl->getLocation(), "Static local variable found: %0")
                << varDecl->getName(),
                DiagnosticIDs::Warning;
    }
};

class CSSE2310Module : public ClangTidyModule {
    void addCheckFactories(ClangTidyCheckFactories& Factories) override
    {
        Factories.registerCheck<StaticLocalCheck>("csse2310-static-local");
    }
};

namespace clang::tidy {

static ClangTidyModuleRegistry::Add<::CSSE2310Module> CSSE2310ModuleInit(
        "csse2310", "Adds CSSE2310 specific checks.");

// This forces linker to link generated object file and register module
// volatile int StaticLocalAnchorSource = 0;

} // namespace clang::tidy
