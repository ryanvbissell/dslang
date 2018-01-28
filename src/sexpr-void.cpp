// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-void.hpp"
#include "dslang-exception.hpp"
using DSL::SEXPR;
using namespace DSL::detail;

#include <string>
using std::string;


// ---------------------------------------------------------------------


SexprVoid const* DSL::detail::SexprVoid::VOID(nullptr);


// ---------------------------------------------------------------------


CX_CONSTRUCTOR(DSL::detail::SexprVoid::SexprVoid, :Sexpr(sc),
                                                          Context* sc)

  CX_ASSERT(!SexprVoid::VOID);
  CX_TOPICOUT(dslang:sexpr, "I am the One True VOID\n");

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::detail::SexprVoid::~SexprVoid)

  VOID = nullptr;
  CX_TOPICOUT(dslang:sexpr, "Destructing canonical VOID\n");

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprVoid::eval, SexprEnv const* env)

  CX_RETURN(SexprVoid::VOID);

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprVoid::Print)

  CX_RETURN("");

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprVoid::Display)

  CX_RETURN("");

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprVoid::Write)

  CX_RETURN("#<void>");

CX_ENDMETHOD



CX_FUNCTION(bool DSL::detail::is_void, Sexpr const* sexpr)

  CX_RETURN(sexpr == SexprVoid::VOID);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::is_void, SEXPR const& sexpr)

  CX_RETURN(sexpr.get() == SexprVoid::VOID);

CX_ENDFUNCTION

