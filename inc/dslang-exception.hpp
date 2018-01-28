// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_EXCEPTION_HPP
#define DSLANG_EXCEPTION_HPP

#include "cx-tracedebug.hpp"
#include "cx-exceptions.hpp"

#include <map>

namespace DSL
{

#define DSL_EXCEPTIONS                                                \
  X(NONE,       "Something ineffable happened.")                      \
  X(INTERNAL,   "An error internal to the s-expression processor "    \
                "has occurred.  It is unlikely that your expression " \
                "was fully evaluated, and a sane environment cannot " \
                "be guaranteed.")                                     \
  X(SYNTAX,     "Syntax Error")                                       \
  X(UNDEFINED,  "Unknown / undefined identifier")                     \
  X(CONTRACT,   "Contract violation")                                 \
  X(ARITY,      "Arity mismatch")


#define X(v,str)  v,
  enum class Error: U32
  {
    DSL_EXCEPTIONS
  };
  extern std::map<Error, const char*> ErrorNameMap;
  extern std::map<Error, const char*> ErrorStringMap;
#undef X

  CX_DECLARE_EXCEPTION_CLASS(Exception, Error, CX::Exception);

} // namespace DSL

#endif  // DSLANG_EXCEPTION_HPP


