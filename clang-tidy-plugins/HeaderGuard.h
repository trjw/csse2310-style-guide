//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef HEADERGUARD_H
#define HEADERGUARD_H

#include "clang-tidy/ClangTidyCheck.h"
#include "FileExtensionsUtils.h"

namespace clang::tidy::csse2310 {

/// Finds and fixes header guards.
class HeaderGuardCheck2310 : public ClangTidyCheck {
public:
  HeaderGuardCheck2310(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context),
        HeaderFileExtensions(Context->getHeaderFileExtensions()) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

  /// Ensure that the provided header guard is a non-reserved identifier.
  std::string sanitizeHeaderGuard(StringRef Guard);

  /// Returns ``true`` if the check should suggest inserting a trailing comment
  /// on the ``#endif`` of the header guard. It will use the same name as
  /// returned by ``HeaderGuardCheck2310::getHeaderGuard``.
  virtual bool shouldSuggestEndifComment(StringRef Filename);
  /// Returns ``true`` if the check should suggest changing an existing header
  /// guard to the string returned by ``HeaderGuardCheck2310::getHeaderGuard``.
  virtual bool shouldFixHeaderGuard(StringRef Filename);
  /// Returns ``true`` if the check should add a header guard to the file
  /// if it has none.
  virtual bool shouldSuggestToAddHeaderGuard(StringRef Filename);
  /// Returns a replacement for the ``#endif`` line with a comment mentioning
  /// \p HeaderGuard. The replacement should start with ``endif``.
  virtual std::string formatEndIf(StringRef HeaderGuard);
  /// Gets the canonical header guard for a file.
  virtual std::string getHeaderGuard(StringRef Filename,
                                     StringRef OldGuard = StringRef()) = 0;

private:
  FileExtensionsSet HeaderFileExtensions;
};

} // namespace clang::tidy::csse2310

#endif // HEADERGUARD_H
