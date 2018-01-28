// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-ident.hpp"

#include "dslang-exception.hpp"
#include "dslang-scheme-context.hpp"
#include "dslang-lexer.hpp"
#include "dslang-parser.hpp"
using namespace DSL::detail;

#include <string>
using std::string;


// ---------------------------------------------------------------------


// TODO, why is this in here?
namespace DSL {
namespace detail {

  bool SexprIdent__Match(char const* text);
  Sexpr::Type SexprIdent__Skip(char const** input);
  Sexpr const* SexprIdent__Parse(Context*, char const** input);

}
}

// ---------------------------------------------------------------------


CX_FUNCTION(bool DSL::detail::SexprIdent__Match, char const* text)

  if (lexer::is_delim(*text))
    CX_RETURN(false);

  CX_RETURN(true);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr::Type DSL::detail::SexprIdent__Skip, char const** input)

  lexer::skip_to_delim(input);
  CX_RETURN(Sexpr::Type::IDENT);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::SexprIdent__Parse, Context* sc,
                                                  char const** input)

  CX_RETURN(DSL::detail::parse<SexprIdent>(sc, input));

CX_ENDFUNCTION


// ---------------------------------------------------------------------



CX_CONSTRUCTOR(DSL::detail::SexprIdent::SexprIdent, : Sexpr(sc),
                                                    Context* sc,
                                                    char const** input)

  CX_ASSERT(SexprIdent__Match(*input));

  char const* begin = *input;

  CX_ASSERT(Sexpr::Type::IDENT == SexprIdent__Skip(input));
  name_ = lexer::duplicate_text(begin, *input);

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::detail::SexprIdent::~SexprIdent)

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprIdent::transmute,
                                                    char const** input)

  // transmute into pre-existing ident-symbol
  SexprIdent const* that = sc_->GetSymbol(name_);
  if (that)
    CX_RETURN(that);

  // TODO, GC: in cases where the user mispells an identifier name,
  // we will end up recording a symbol here that is only referenced
  // by the symbol table.  We'll need to periodically scan the symbol
  // table and cull items with refcount_ == 1
  sc_->AddSymbol(this);
  CX_RETURN(this);

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprIdent::eval, SexprEnv const* env)


  Sexpr const* that;

  if (that = env->GetInternal(name_)) // keywords & foreign-funcs
    CX_RETURN(that);

  if (that = env->GetVariable(name_))
    CX_RETURN(that);

  CX_THROW( DSL::Exception,
            DSL::Error::UNDEFINED,
            "Attempted to evaluate unbound name '%s'",
            name_.c_str() );

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprIdent::Write)

  CX_RETURN(name_);

CX_ENDMETHOD



