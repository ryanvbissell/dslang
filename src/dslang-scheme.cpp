// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "dslang-scheme.hpp"
using DSL::detail::Sexpr;
using DSL::SEXPR;

#include <string>
using std::string;


// ---------------------------------------------------------------------


CX_CONSTRUCTOR0(DSL::Scheme::Scheme)

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::Scheme::~Scheme)

  CX_TOPICOUT(dslang, "Deleting SCHEME instance.\n");

CX_ENDMETHOD


#if 0
CX_METHOD(void DSL::Scheme::AddForeign, const string& name,
                                        ForeignFunc ffunc)

  env_->AddForeign(name, ffunc);

CX_ENDMETHOD
#endif



CX_METHOD(SEXPR DSL::Scheme::Read, char const** input)

  CX_RETURN(read(input));

CX_ENDMETHOD



CX_CONSTMETHOD(SEXPR DSL::Scheme::Eval, SEXPR const& sexpr)

  CX_RETURN(eval(sexpr.get()));

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::Scheme::Print, SEXPR const& sexpr)

  CX_RETURN(print(sexpr.get()));

CX_ENDMETHOD



CX_METHOD(string DSL::Scheme::ReadEvalPrint, char const** input)

  SEXPR const& parsed = Read(input);
  SEXPR const& result = Eval(parsed);
  CX_RETURN(Print(result));

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::Scheme::eval, Sexpr const* sexpr)

  CX_RETURN(sexpr ? sexpr->eval(env()) : sexpr);

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::Scheme::print, Sexpr const* sexpr)

  CX_RETURN(sexpr ? sexpr->Print() : "");

CX_ENDMETHOD


