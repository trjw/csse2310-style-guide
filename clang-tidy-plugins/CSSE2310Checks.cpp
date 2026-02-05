#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"

#include "StaticLocalCheck.h"
#include "DoubleAssignmentCheck.h"
#include "LLVMHeaderGuardCheck.h"
#include "FileCommentCheck.h"
#include "FileCommentCompleteCheck.h"
#include "InappropriateCommaOperatorCheck.h"
#include "NonConstVariableDeclarationCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::tidy::csse2310;

class CSSE2310Module : public ClangTidyModule {
    void addCheckFactories(ClangTidyCheckFactories& Factories) override
    {
        Factories.registerCheck<StaticLocalCheck>("csse2310-static-local");
        Factories.registerCheck<DoubleAssignmentCheck>("csse2310-double-assignment");
        Factories.registerCheck<LLVMHeaderGuardCheck>("csse2310-header-guard");
        Factories.registerCheck<FileCommentCheck>("csse2310-file-comment");
        Factories.registerCheck<FileCommentCompleteCheck>("csse2310-file-comment-complete");
        Factories.registerCheck<InappropriateCommaOperatorCheck>("csse2310-inappropriate-comma-operator");
        Factories.registerCheck<NonConstVariableDeclarationCheck>("csse2310-non-const-variable-declaration");
    }
};

namespace clang::tidy {

static ClangTidyModuleRegistry::Add<::CSSE2310Module> CSSE2310ModuleInit(
        "csse2310", "Adds CSSE2310 specific checks.");

// This forces linker to link generated object file and register module
// volatile int StaticLocalAnchorSource = 0;

} // namespace clang::tidy
