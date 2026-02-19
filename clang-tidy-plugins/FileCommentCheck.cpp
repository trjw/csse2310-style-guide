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
      InsertTemplate(Options.get("InsertTemplate",
              "/** @file <filename1> ... OR /// @file <filename2> ..."))
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

bool FileCommentCheck::parseAndCheckFileComment(ASTContext& Context,
        StringRef CommentText, SourceLocation Loc, StringRef FileBaseName)
{
    llvm::StringSet<> CommandsFound;
    llvm::SmallVector<llvm::StringRef, 16> Lines;
    CommentText.split(Lines, '\n', -1, false);

    // Code below would parse multiple lines, but we've modified the check
    // to only check the first line - the @file command must be present here
    StringRef Rest;
    for (StringRef Line : Lines) {
        Line = Line.ltrim(" \t/*");
        if (Line.starts_with("@") || Line.starts_with("\\")) {
            StringRef Cmd = Line.drop_front(1).split(' ').first;
            CommandsFound.insert(Cmd);
            // Get the next word after the command
            Rest = Line.drop_front(1 + Cmd.size()).ltrim(" \t").rtrim(" \t*/");
        }
    }

    // @file (or whatever the RequiredCommand is) must exist
    if (CommandsFound.contains(RequiredCommand)) {
        // Make sure that Rest is the filename (in FileBaseName)
        if (Rest == FileBaseName) {
            return true; // All good
        }
    }
    return false;
}

void FileCommentCheck::check(
        const ast_matchers::MatchFinder::MatchResult& Result)
{
    ASTContext& Context = *Result.Context;
    const SourceManager& SM = Context.getSourceManager();
    SourceLocation StartOfFile = SM.getLocForStartOfFile(SM.getMainFileID());
    // Get the name of the file
    StringRef Filename = SM.getFilename(StartOfFile);
    StringRef FileBaseName = llvm::sys::path::filename(Filename);

    StringRef Buffer = SM.getBufferData(SM.getMainFileID());
    size_t Pos = Buffer.find_first_not_of(" \t\r\n");
    if (Pos == StringRef::npos) {
        return;
    }

    StringRef Start = Buffer.substr(Pos);
    bool found = false;

    // Look for /**, /*!, ///, or //! at the top
    if (Start.starts_with("/**") || Start.starts_with("/*!")
            || Start.starts_with("///") || Start.starts_with("//!")) {
        size_t EndPos = Start.find("\n"); // Find the end of the line

        StringRef CommentText = Start.substr(0, EndPos);
        SourceLocation Loc = SM.getLocForStartOfFile(SM.getMainFileID())
                                     .getLocWithOffset(Pos);

        found = parseAndCheckFileComment(
                Context, CommentText, Loc, FileBaseName);
    }
    if (!found) {
        // No file comment → insert template
        auto D = diag(StartOfFile,
                         "File must begin with a Doxygen comment containing "
                         "@%0 and the name the file on the first line",
                         DiagnosticIDs::Error)
                << RequiredCommand;
        if (!InsertTemplate.empty()) {
            // Replace <filename*> in the template with the file base name
            size_t FilenamePos = InsertTemplate.find("<filename1>");
            if (FilenamePos != std::string::npos) {
                InsertTemplate.replace(FilenamePos, 11, FileBaseName.str());
            }
            // Replace <filename> in the template with the file base name
            // (again)
            FilenamePos = InsertTemplate.find("<filename2>");
            if (FilenamePos != std::string::npos) {
                InsertTemplate.replace(FilenamePos, 11, FileBaseName.str());
            }
            D << FixItHint::CreateInsertion(StartOfFile, InsertTemplate);
        }
    }
}
