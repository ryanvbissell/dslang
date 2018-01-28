// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-callable.hpp"
using DSL::detail::Sexpr;


#include <string>
using std::string;


// ---------------------------------------------------------------------


CX_CONSTRUCTOR3(DSL::detail::SexprCallable::SexprCallable,
                                : Sexpr(sc), name_(name), type_(type),
                                                    Context* sc,
                                                    string const& name,
                                                    CallType type)

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprCallable::eval, SexprEnv const* env)

  // all callables evaluate to themselves
  CX_RETURN(this);

CX_ENDMETHOD

