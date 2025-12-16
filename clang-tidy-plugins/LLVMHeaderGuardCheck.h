//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVMHEADERGUARDCHECK_H
#define LLVMHEADERGUARDCHECK_H

#include "HeaderGuard.h"

namespace clang::tidy::csse2310 {

/// Finds and fixes header guards that do not adhere to LLVM style.
/// For the user-facing documentation see:
/// https://clang.llvm.org/extra/clang-tidy/checks/llvm/header-guard.html
class LLVMHeaderGuardCheck : public HeaderGuardCheck2310 {
public:
  LLVMHeaderGuardCheck(StringRef Name, ClangTidyContext *Context);

  //bool shouldSuggestEndifComment(StringRef Filename) override { return false; }
  std::string getHeaderGuard(StringRef Filename, StringRef OldGuard) override;
};

} // namespace clang::tidy::csse2310

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_LLVM_HEADERGUARDCHECK_H
