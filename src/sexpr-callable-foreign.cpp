// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-callable-foreign.hpp"

#include "dslang-exception.hpp"
#include "dslang-dialect.hpp"
using namespace DSL::detail;

#include <string>
using std::string;


// ---------------------------------------------------------------------


CX_CONSTRUCTOR2(DSL::detail::SexprForeign::SexprForeign,
                    : SexprCallable(sc, name, CallType::SPECIAL),
                      ffunc_(ffunc),
                                            Context* sc,
                                            string const& name,
                                            ForeignFunc const& ffunc)

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::detail::SexprForeign::~SexprForeign)

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprForeign::Call,
                                              SexprEnv const* env,
                                              SexprCons const* args)

  SEXPR const& result = ffunc_(env, name_, args);

  result.get()->Adopted(); // take ownership
  CX_RETURN(result.get());

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprForeign::Write)

  string const& temp = "#<foreign:" + name_ + ">";
  CX_RETURN(temp);

CX_ENDMETHOD



