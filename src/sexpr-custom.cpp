// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-custom.hpp"

using namespace DSL::detail;

#include <string>
using std::string;


// ---------------------------------------------------------------------


CX_CONSTRUCTOR2(DSL::detail::SexprCustom::SexprCustom,
                                      : Sexpr(sc),
                                        lexeme_(lexeme),
                                                  Context* sc,
                                                  string const& lexeme)

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprCustom::Write)

  string const& temp = "#<custom:" + lexeme_ + ">";
  CX_RETURN(temp);

CX_ENDMETHOD



