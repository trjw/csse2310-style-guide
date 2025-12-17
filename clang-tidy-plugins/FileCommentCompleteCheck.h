#ifndef FILE_COMMENT_COMPLETE_CHECK_H
#define FILE_COMMENT_COMPLETE_CHECK_H

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace csse2310 {

class FileCommentCompleteCheck : public ClangTidyCheck {
public:
  FileCommentCompleteCheck(StringRef Name, ClangTidyContext *Context);

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;

private:
  std::string RequiredCommands;

  // Helpers
  void parseAndCheckFileComment(ASTContext &Context, StringRef CommentText,
          SourceLocation Loc);
};

} // namespace csse2310
} // namespace tidy
} // namespace clang

#endif // FILE_COMMENT_COMPLETE_CHECK_H

