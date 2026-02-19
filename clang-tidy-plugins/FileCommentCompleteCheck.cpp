#include "FileCommentCompleteCheck.h"

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

} // namespace

FileCommentCompleteCheck::FileCommentCompleteCheck(StringRef Name, ClangTidyContext* Context)
    : ClangTidyCheck(Name, Context),
      RequiredCommands(Options.get("RequiredCommands", "author,ai"))
{
}

void FileCommentCompleteCheck::storeOptions(ClangTidyOptions::OptionMap& Opts)
{
    Options.store(Opts, "RequiredCommands", RequiredCommands);
}

void FileCommentCompleteCheck::registerMatchers(ast_matchers::MatchFinder* Finder)
{
    Finder->addMatcher(translationUnitDecl().bind("tu"), this);
}

void FileCommentCompleteCheck::parseAndCheckFileComment(
        ASTContext& Context, StringRef CommentText, SourceRange CommentRange)
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

    // Check required commands
    for (StringRef Cmd : parseRequiredCommands(RequiredCommands)) {
        if (!CommandsFound.contains(Cmd)) {
            diag(CommentRange.getBegin(), 
                    "Doxygen comment at the start of file must contain @%0",
                    DiagnosticIDs::Warning)
                    << Cmd << CharSourceRange::getCharRange(CommentRange);
        }
    }
}

void FileCommentCompleteCheck::check(
        const ast_matchers::MatchFinder::MatchResult& Result)
{
    ASTContext& Context = *Result.Context;
    const SourceManager& SM = Context.getSourceManager();

    StringRef Buffer = SM.getBufferData(SM.getMainFileID());
    size_t Pos = Buffer.find_first_not_of(" \t\r\n");
    if (Pos == StringRef::npos) {
        return;
    }

    SourceLocation StartOfFile = SM.getLocForStartOfFile(SM.getMainFileID());
    StringRef Start = Buffer.substr(Pos);

    // Look for /**, ///, or //! at the top - and find the end of the comment
    size_t EndPos = 0;
    if (Start.starts_with("/**") || Start.starts_with("/*!")) {
        // Block comment
        EndPos = Start.find("*/");
        if (EndPos != StringRef::npos) {
            EndPos += 2; // include */
        } else {
            return; // malformed block comment
        }
    } else if (Start.starts_with("///") || Start.starts_with("//!")) {
        // Line comment(s)

        while (true) {
            // find end of current line
            size_t LineEnd = Start.find('\n', EndPos);
            if (LineEnd == StringRef::npos)
                LineEnd = Start.size();

            EndPos = LineEnd + 1;

            if (EndPos >= Start.size()) {
                // Can't go any further
                break;
            }
            StringRef NextLine = Start.substr(EndPos);

            if(!(NextLine.starts_with("///") || NextLine.starts_with("//!"))) {
                break;
            }
        }

        // EndPos now spans the full /// comment block
    } else {
        // else no Doxygen comment at top of file - do nothing
        // (We have a separate check for the presence of a file comment)
        return;
    }

    StringRef CommentText = Start.substr(0, EndPos);
    SourceLocation Loc = SM.getLocForStartOfFile(SM.getMainFileID())
                                 .getLocWithOffset(Pos);
    // -1 present to not include the newline
    SourceLocation EndLoc = Loc.getLocWithOffset(EndPos - 1);
    parseAndCheckFileComment(Context, CommentText, SourceRange(Loc, EndLoc));
}
