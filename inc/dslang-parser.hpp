// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_PARSER_HPP
#define DSLANG_PARSER_HPP

#include "sexpr.hpp"

namespace DSL {

namespace detail {

template <class T>
CX_FUNCTION(Sexpr const* parse, Context* sc,
                                char const** input)

  CX_TOPICOUT(dslang:parse, "input is '%s'\n", *input);
  Sexpr const* sexpr = new T(sc, input);

  if (input)
  {
    Sexpr const* trans = sexpr->transmute(input);

    // some Ts are factories, and their T::transmute() methods will
    // return eomething other than themselves.  If this happens, we
    // need to destroy the original.
    if (trans != sexpr)
    {
      CX_ASSERT(sexpr->Nascent());
      delete sexpr;
      sexpr = trans;
    }
  }

  CX_RETURN(sexpr);

CX_ENDFUNCTION

} // namespace detail

} // namespace DSL


#endif // DSLANG_PARSER_HPP

