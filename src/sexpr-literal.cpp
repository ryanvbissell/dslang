// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-literal.hpp"
#include "dslang-exception.hpp"


// ---------------------------------------------------------------------


CX_CONSTRUCTOR(DSL::detail::SexprLiteral::SexprLiteral, :Sexpr(sc),
                                                           Context* sc)

  // TODO
  // this is where we might deal with dialect literals.  we'd construct
  // this placeholder, and then transmute during SexprLiteral::transmute()

CX_ENDMETHOD


