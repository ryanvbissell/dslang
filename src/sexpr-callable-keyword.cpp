// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-callable-keyword.hpp"
#include "dslang-exception.hpp"
using namespace DSL::detail;

#include <string>
using std::string;


// ---------------------------------------------------------------------


CX_CONSTRUCTOR2(DSL::detail::SexprKeyword::SexprKeyword,
                              : SexprCallable(nullptr, name, type),
                                handler_(handler),
                                            Context* sc,
                                            string const& name,
                                            Keyword* handler,
                                            CallType type)

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprKeyword::Call,
                                                  SexprEnv const* env,
                                                  SexprCons const* args)

  // some keywords modify the environment, so need non-const
  CX_RETURN(handler_(const_cast<SexprEnv*>(env), args));

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprKeyword::Write)

  string const& temp = "#<keyword:" + name_ + ">";
  CX_RETURN(temp);

CX_ENDMETHOD



