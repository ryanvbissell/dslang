// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-env.hpp"

#include "dslang-exception.hpp"
#include "dslang-scheme-context.hpp"
using DSL::SexprENV;
using namespace DSL::detail;

#include <string>
using std::string;


// ---------------------------------------------------------------------


CX_CONSTRUCTOR2(DSL::detail::SexprEnv::SexprEnv,
                                      : Sexpr(sc),
                                        prev_(sc->env()),
                                                    Context* sc)

CX_ENDMETHOD


CX_CONSTRUCTOR2(DSL::detail::SexprEnv::SexprEnv,
                                      : Sexpr(sc),
                                        prev_(env),
                                                  Context* sc,
                                                  SexprEnv const* env)

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::detail::SexprEnv::~SexprEnv)

  CX_TOPICOUT(dslang:env, "Deconstructing ENV (serial = %lu)\n",
                          serial_);

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprEnv::Write)

  CX_RETURN("#<environment>");

CX_ENDMETHOD


CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprEnv::GetInternal,
                                                  string const& name)

  // TODO: check for constants like 'else'
  Sexpr const* that = sc_->GetKeyword(name);

  if (!that)
    that = GetForeign(name);

  CX_RETURN(that);

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprEnv::GetVariable,
                                                    string const& ident)

  if (bindings_.count(ident) == 1)
    CX_RETURN(bindings_.at(ident).get());

  if (prev_)
    CX_RETURN(prev_->GetVariable(ident));

  CX_RETURN(nullptr);

CX_ENDMETHOD



CX_METHOD(void DSL::detail::SexprEnv::SetVariable,  const string& ident,
                                                    Sexpr const* value)

  bindings_[ident] = value;

CX_ENDMETHOD



CX_CONSTMETHOD(SexprForeign const* DSL::detail::SexprEnv::GetForeign,
                                                    const string& fname)

  if (ffmap_.count(fname) == 1) // TODO, not performant
    CX_RETURN(ffmap_.at(fname).get());

  if (prev_)
    CX_RETURN(prev_->GetForeign(fname));

  CX_RETURN(nullptr);

CX_ENDMETHOD



CX_METHOD(void DSL::detail::SexprEnv::AddForeign, const string& fname,
                                                    ForeignFunc ffunc)

  if (ffmap_.count(fname)) // TODO, not performant
    CX_ASSERT(false); // TODO, throw exception

  SexprForeign const* foreign = new SexprForeign(sc_, fname, ffunc);
  ffmap_[fname] = foreign;

CX_ENDMETHOD

