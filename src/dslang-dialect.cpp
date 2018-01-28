// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "dslang-dialect.hpp"

#include "dslang-lexer.hpp"
#include "sexpr-literal-bool.hpp"
#include "sexpr-literal-number.hpp"
#include "sexpr-literal-string.hpp"
#include "sexpr-ident.hpp"
#include "sexpr-cons.hpp"
#include "sexpr-quote.hpp"
using namespace DSL::detail;
using DSL::TokenHandler;


// ---------------------------------------------------------------------


CX_CONSTRUCTOR(DSL::Dialect::Dialect, :sc_(nullptr))

  // this should be first, so it gets matched last
  registerTokenizer(SexprIdent__Match, SexprIdent__Skip, SexprIdent__Parse);

  registerTokenizer(SexprQuote__Match, SexprQuote__Skip, SexprQuote__Parse);
  registerTokenizer(SexprCons__Match, SexprCons__Skip, SexprCons__Parse);
  registerTokenizer(SexprString__Match, SexprString__Skip, SexprString__Parse);
  registerTokenizer(SexprNumber__Match, SexprNumber__Skip, SexprNumber__Parse);
  registerTokenizer(SexprBool__Match, SexprBool__Skip, SexprBool__Parse);

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::Dialect::~Dialect)

  for (auto&& pair : tokenizers_)
    delete pair.second;

CX_ENDMETHOD



// TODO, if made private, use DSL::detail::Context* for 'sc'
CX_METHOD(void DSL::Dialect::SetContext, DSL::Scheme* sc)

  CX_ASSERT(!sc_); // this shouldn't get called multiple times

  // TODO, can remove typecast if above TODO is completed
  sc_ = dynamic_cast<Context*>(sc);
  CX_ASSERT(sc_);  // dynamic cast failed?

CX_ENDMETHOD



CX_CONSTMETHOD(TokenHandler const* DSL::Dialect::GetTokenHandler,
                                                      char const* text)

#if 1
  // C++17 version:
  for (auto&& [match,handler] : tokenizers_)
    if (match(text))
      CX_RETURN(handler);
#else
  // C++11 version:
  for (auto&& pair : tokenizers_)
    if (pair.first(text))
      CX_RETURN(pair.second);
#endif

  CX_RETURN(nullptr);

CX_ENDMETHOD



CX_METHOD(void DSL::Dialect::registerTokenizer, TokenMatcher match,
                                                TokenSkipper skip,
                                                TokenParser parse)

  TokenHandler const* phandler = new TokenHandler{skip, parse};
  // we use a deque so that custom dialects can override
  // built-in token handlers, when we iterate for a match
  tokenizers_.push_front(std::make_pair(match,phandler));

CX_ENDMETHOD


#if 0
CX_CONSTMETHOD(bool DSL::Dialect::IsCustomToken, char const * const text)

  CX_RETURN(false);  // class must be overridden for this behavior

CX_ENDMETHOD



CX_CONSTMETHOD(const SexprCUSTOM* DSL::Dialect::ParseCustom, const string& lexeme)

  CX_ASSERT(false); // TODO, needs to be fatal, internal error

  // This is supposed to be overridden, and never called
  // It is not pure-virtual because the class overall is
  // designed to be used as the default behavior.
  CX_RETURN(nullptr);

CX_ENDMETHOD
#endif


