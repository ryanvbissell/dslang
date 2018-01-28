// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-quote.hpp"

#include "dslang-exception.hpp"
#include "dslang-dialect.hpp"
#include "dslang-lexer.hpp"
#include "dslang-parser.hpp"
#include "sexpr-literal-bool.hpp"
#include "sexpr-cons.hpp"
#include "sexpr-literal-number.hpp"
#include "sexpr-literal-string.hpp"
using namespace DSL::detail;

#include <string>
using std::string;


// ---------------------------------------------------------------------


static
CX_FUNCTION(Sexpr::Type _get_quoted_expression_type, char const* text)

  // TODO, this needs to be genericized
  if (SexprQuote__Match(text))
    CX_RETURN(Sexpr::Type::QUOTE);

  if (SexprBool__Match(text))
    CX_RETURN(Sexpr::Type::BOOL);

  if (SexprNumber__Match(text))
    CX_RETURN(Sexpr::Type::NUMBER);

  if (SexprString__Match(text))
    CX_RETURN(Sexpr::Type::STRING);

  if (SexprCons__Match(text))
    CX_RETURN(Sexpr::Type::CONS);

  // TODO
  /*
  if (dialect.IsCustom(text)
    CX_RETURN(Sexpr::Type::CUSTOM);
  */

  // anything else should be initially identified as an ident.
  CX_RETURN(Sexpr::Type::IDENT);

CX_ENDFUNCTION


static
CX_FUNCTION(Sexpr::Type _skip_past_quoted_expression,
                                                char const** input)

  CX_ASSERT(input);
  CX_ASSERT(*input);
  CX_ASSERT(**input);

  Sexpr::Type type = _get_quoted_expression_type(*input);

  switch (type)
  {
  case Sexpr::Type::QUOTE:
    // skip_past_quoted_expression() expressly (heh) assumes that the
    // expression is not quoted. (That's what skip_past_quote() is for)
    CX_THROW( DSL::Exception,
        DSL::Error::INTERNAL,
        "Specifically, an unexpected quote token (\') was "
        "encountered." );
    break;

  // TODO. this is broken, should be genericized for all literals not just numbers
  case Sexpr::Type::NUMBER:
    // quoted literals decay to just literals
    type = SexprNumber__Skip(input);
    break;

  case Sexpr::Type::BOOL:
    type = SexprBool__Skip(input);
    break;

  case Sexpr::Type::STRING:
    type = SexprString__Skip(input);
    break;

  case Sexpr::Type::CONS:
    type = SexprCons__Skip(input);
    break;

  case Sexpr::Type::IDENT:
    type = SexprIdent__Skip(input);
    break;

  default:
    CX_THROW( DSL::Exception,
        DSL::Error::INTERNAL,
        "Specifically, an unknown s-expr type was "
        "encountered." );
  }

  CX_RETURN(type);

CX_ENDFUNCTION


static
CX_FUNCTION(void _skip_any_quotemarks, char const** input)

  if (**input != '\'')
    CX_RETURNVOID;

  bool badquote = false;

  do
  {
    ++(*input);
    while (**input && DSL::lexer::is_whitespace(**input))
    {
      // real scheme seems to support this,
      // but I think it's an abomination
      badquote = true;
      ++(*input);
    }
  } while (**input && (**input == '\''));


  if (badquote)
  {
    CX_THROW( DSL::Exception,
        DSL::Error::SYNTAX,
        "Whitespace disallowed between quote token (\') and "
        "the to-be-quoted expression." );
  }

CX_ENDFUNCTION

// ---------------------------------------------------------------------


CX_FUNCTION(bool DSL::detail::SexprQuote__Match, char const* text)

  CX_RETURN(*text == '\'');

CX_ENDFUNCTION



CX_FUNCTION(Sexpr::Type DSL::detail::SexprQuote__Skip,
                                                    char const** input)

  _skip_any_quotemarks(input);
  _skip_past_quoted_expression(input);

  CX_RETURN(Sexpr::Type::QUOTE);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::SexprQuote__Parse, Context* sc,
                                                  char const** input)

  CX_RETURN(DSL::detail::parse<SexprQuote>(sc, input));

CX_ENDFUNCTION


// ---------------------------------------------------------------------



CX_CONSTRUCTOR(DSL::detail::SexprQuote::SexprQuote,
                                      : Sexpr(sc),
                                            Context* sc,
                                            char const** input)

  CX_ASSERT(SexprQuote__Match(*input));

  char const* begin = *input;

  // learn the final, underlying type that is quoted
  // we use this to short-circuit the quoting of literals.
  _skip_any_quotemarks(input);
  type_ = _skip_past_quoted_expression(input);

  text_ = lexer::duplicate_text(begin, *input);

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::detail::SexprQuote::~SexprQuote)

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprQuote::transmute,
                                                    char const** input)

  // we will special-case quoted literals and '()
  // because they, when quoted, should always evaluate
  // to their unquoted selves.  There seems to be no good
  // reason for delaying that transmutation.

  char const* begin = text_.c_str();
  while (*begin && (*begin == '\'')) { ++begin; }
  switch (type_)
  {
    // TODO this is broken, should be all literals not just numbers
    case Sexpr::Type::NUMBER:
      {
        // transmute quoted numbers into numbers
        CX_RETURN(DSL::detail::parse<SexprNumber>(sc_, &begin));
      }
      break;

    case Sexpr::Type::CONS:
      if(DSL::lexer::is_empty_list(begin))
      {
        // transmute '() into nil
        CX_RETURN(SexprCons::NIL);
      }
      break;
  }

  CX_RETURN(this);

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprQuote::eval, SexprEnv const* env)

  char const* text = text_.c_str();
  ++text;

  Sexpr const* sexpr = sc_->read(&text);
  if (!sexpr) // if (nil)
  {
    CX_ASSERT(false);  // TODO, need exception (end of cons?)
  }

  CX_RETURN(sexpr);

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprQuote::Write)

  CX_RETURN(text_);

CX_ENDMETHOD



