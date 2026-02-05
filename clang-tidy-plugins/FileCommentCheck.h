#ifndef FILE_COMMENT_CHECK_H
#define FILE_COMMENT_CHECK_H

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace csse2310 {

class FileCommentCheck : public ClangTidyCheck {
  public:
    FileCommentCheck(StringRef Name, ClangTidyContext* Context);

    void registerMatchers(ast_matchers::MatchFinder* Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult& Result) override;
    void storeOptions(ClangTidyOptions::OptionMap& Opts) override;

  private:
    std::string RequiredCommand;
    std::string InsertTemplate;

    // Helpers
    bool parseAndCheckFileComment(ASTContext& Context, StringRef CommentText,
            SourceLocation Loc, StringRef FileBaseName);
};

} // namespace csse2310
} // namespace tidy
} // namespace clang

#endif // FILE_COMMENT_CHECK_H
