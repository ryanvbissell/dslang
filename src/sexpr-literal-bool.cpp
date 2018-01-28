// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-literal-bool.hpp"

#include "dslang-scheme.hpp"
#include "dslang-lexer.hpp"
#include "dslang-parser.hpp"
using DSL::SEXPR;
using namespace DSL::detail;

#include <string>
using std::string;


// ---------------------------------------------------------------------


SexprBool const* DSL::detail::SexprBool::TRUE(nullptr);
SexprBool const* DSL::detail::SexprBool::FALSE(nullptr);


// ---------------------------------------------------------------------


CX_FUNCTION(bool DSL::detail::SexprBool__Match, char const* text)

  CX_RETURN((text[0] == '#') && ((text[1] == 't') || (text[1] == 'f')));

CX_ENDFUNCTION


static
CX_FUNCTION(bool SexprBool__skip, char const** input)

  if (**input != '#')
    CX_ASSERT(false);  // TODO, need exception

  const char* begin = ++(*input);

  DSL::lexer::skip_to_delim(input);
  if (*input - begin > 1)
  {
    *input = begin;
    CX_ASSERT(false);  // TODO, need exception
  }

  switch(*begin)
  {
    case 't':
      CX_RETURN(true);
      break;

    case 'f':
      CX_RETURN(false);
      break;
  }

  CX_ASSERT(false);  // TODO, need exception

CX_ENDFUNCTION



CX_FUNCTION(Sexpr::Type DSL::detail::SexprBool__Skip, char const** input)

  SexprBool__skip(input);
  CX_RETURN(Sexpr::Type::BOOL);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::SexprBool__Parse,  Context* sc,
                                                  char const** input)

  bool me  = SexprBool__skip(input);

  if (me && SexprBool::TRUE)
    CX_RETURN(SexprBool::TRUE);

  if (!me && SexprBool::FALSE)
    CX_RETURN(SexprBool::FALSE);

  // ostensibly we should never get here, because DSL::detail::Context
  // should have already instantiated the canonical values above
  CX_ASSERT(false, "How did we get here?")
  CX_RETURN(nullptr);

CX_ENDFUNCTION


// ---------------------------------------------------------------------


CX_CONSTRUCTOR(DSL::detail::SexprBool::SexprBool, :SexprLiteral(sc),
                                                           Context* sc)

  // I am a canonical bool value

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::detail::SexprBool::~SexprBool)

  if (is_canonical_true(this))
  {
    CX_TOPICOUT(dslang:bool, "Destructing canonical TRUE\n");
    TRUE = nullptr;
  }
  else if (is_false(this))
  {
    CX_TOPICOUT(dslang:bool, "Destructing canonical FALSE\n");
    FALSE = nullptr;
  }
  else
  {
    CX_ASSERT(false, "Unexpected SexprBool doppelganger");
  }

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprBool::transmute,
                                                    char const** input)

  // I don't actually think this should happen for bools;
  // our present implementation is smart enough to avoid it
  CX_ASSERT(false, "Unexpected SexprBool doppelganger");

  // transmute doppelganger to the corresponding canonical object
  CX_RETURN(this);

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprBool::Write)

  if (is_false(this))
    CX_RETURN("#f");

  CX_RETURN("#t");

CX_ENDMETHOD



CX_FUNCTION(bool DSL::detail::is_false, Sexpr const* sexpr)

  // equivalency to canonical false
  CX_RETURN(sexpr == DSL::detail::SexprBool::FALSE);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::is_false, SEXPR const& sexpr)

  // equivalency to canonical false
  CX_RETURN(sexpr.get() == DSL::detail::SexprBool::FALSE);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::detail::is_true, Sexpr const* sexpr)

  CX_RETURN(!is_false(sexpr));

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::is_true, SEXPR const& sexpr)

  CX_RETURN(!is_false(sexpr));

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::detail::is_canonical_true, Sexpr const* sexpr)

  // equivalency to canonical true
  CX_RETURN(sexpr == DSL::detail::SexprBool::TRUE);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::is_canonical_true, SEXPR const& sexpr)

  // equivalency to canonical true
  CX_RETURN(sexpr.get() == DSL::detail::SexprBool::TRUE);

CX_ENDFUNCTION

