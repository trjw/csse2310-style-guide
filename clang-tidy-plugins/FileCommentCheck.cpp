#include "FileCommentCheck.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Comment.h"
#include "clang/AST/CommentVisitor.h"
#include "clang/Lex/Lexer.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/SmallVector.h"

using namespace clang;
using namespace clang::comments;
using namespace clang::tidy;
using namespace clang::ast_matchers;
using namespace clang::tidy::csse2310;

namespace {

static llvm::SmallVector<StringRef, 8> parseRequiredCommands(StringRef CSV)
{
    llvm::SmallVector<StringRef, 8> Result;
    CSV.split(Result, ',', -1, false);
    for (auto& S : Result) {
        S = S.trim();
    }
    return Result;
}

static SourceLocation getFileStart(ASTContext& Context)
{
    const SourceManager& SM = Context.getSourceManager();
    return SM.getLocForStartOfFile(SM.getMainFileID());
}

} // namespace

FileCommentCheck::FileCommentCheck(StringRef Name, ClangTidyContext* Context)
    : ClangTidyCheck(Name, Context),
      RequiredCommand(Options.get("RequiredCommand", "file")),
      InsertTemplate(Options.get("InsertTemplate", "/** @file <filename> ..."))
{
}

void FileCommentCheck::storeOptions(ClangTidyOptions::OptionMap& Opts)
{
    Options.store(Opts, "RequiredCommand", RequiredCommand);
    Options.store(Opts, "InsertTemplate", InsertTemplate);
}

void FileCommentCheck::registerMatchers(ast_matchers::MatchFinder* Finder)
{
    Finder->addMatcher(translationUnitDecl().bind("tu"), this);
}

bool FileCommentCheck::parseAndCheckFileComment(
        ASTContext& Context, StringRef CommentText, SourceLocation Loc)
{
    llvm::StringSet<> CommandsFound;
    llvm::SmallVector<llvm::StringRef, 16> Lines;
    CommentText.split(Lines, '\n', -1, false);

    for (StringRef Line : Lines) {
        Line = Line.ltrim(" \t/*");
        if (Line.starts_with("@") || Line.starts_with("\\")) {
            StringRef Cmd = Line.drop_front(1).split(' ').first;
            CommandsFound.insert(Cmd);
        }
    }

    // @file (or whatever the RequiredCommand is must exist
    return CommandsFound.contains(RequiredCommand);
}
#if 0
    if (!CommandsFound.contains(RequiredCommand)) {
        auto D = diag(Loc,
                         "Doxygen comment at the start of file must contain "
                         "@%0",
                         DiagnosticIDs::Error)
                << RequiredCommand;
        if (!InsertTemplate.empty()) {
            // We use an array of fix it hints to ensure one gets printed
            std::vector<FixItHint> FixIts;
            FixIts.push_back(FixItHint::CreateInsertion(Loc, InsertTemplate));
            D << FixIts;
        }
    }
}
#endif

void FileCommentCheck::check(
        const ast_matchers::MatchFinder::MatchResult& Result)
{
    ASTContext& Context = *Result.Context;
    const SourceManager& SM = Context.getSourceManager();

    StringRef Buffer = SM.getBufferData(SM.getMainFileID());
    size_t Pos = Buffer.find_first_not_of(" \t\r\n");
    if (Pos == StringRef::npos) {
        return;
    }

    StringRef Start = Buffer.substr(Pos);
    bool found = false;

    // Look for /**, ///, or //! at the top
    if (Start.starts_with("/**") || Start.starts_with("///")
            || Start.starts_with("//!")) {
        size_t EndPos = Start.find("*/");
        if (EndPos != StringRef::npos) {
            EndPos += 2; // Include */
        } else {
            EndPos = Start.find("\n"); // single-line style
        }

        StringRef CommentText = Start.substr(0, EndPos);
        SourceLocation Loc = SM.getLocForStartOfFile(SM.getMainFileID())
                                     .getLocWithOffset(Pos);

        found = parseAndCheckFileComment(Context, CommentText, Loc);
    } 
    if (!found) {
        // No file comment → insert template
        SourceLocation StartOfFile = SM.getLocForStartOfFile(SM.getMainFileID());
        auto D = diag(StartOfFile,
                         "File must begin with a Doxygen comment containing "
                         "@%0",
                         DiagnosticIDs::Error)
                << RequiredCommand;
        if (!InsertTemplate.empty()) {
            D << FixItHint::CreateInsertion( StartOfFile, InsertTemplate);
        }
    }
}
