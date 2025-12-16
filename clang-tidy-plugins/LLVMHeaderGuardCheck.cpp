//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LLVMHeaderGuardCheck.h"
#include "HeaderGuard.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/Path.h"

namespace clang::tidy::csse2310 {

LLVMHeaderGuardCheck::LLVMHeaderGuardCheck(StringRef Name,
                                           ClangTidyContext *Context)
    : HeaderGuardCheck2310(Name, Context) {}

std::string LLVMHeaderGuardCheck::getHeaderGuard(StringRef Filename,
                                                 StringRef OldGuard) {
  // Get the basename of the Filename
  StringRef Basename = llvm::sys::path::filename(Filename);

  std::string Guard = Basename.lower();

  llvm::replace(Guard, '/', '_');
  llvm::replace(Guard, '.', '_');
  llvm::replace(Guard, '-', '_');

  return StringRef(Guard).upper();
}

} // namespace clang::tidy::csse2310
