// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-literal-string.hpp"

#include "dslang-exception.hpp"
#include "dslang-dialect.hpp"
#include "dslang-lexer.hpp"
#include "dslang-parser.hpp"
using namespace DSL::detail;

#include <string>
using std::string;


// ---------------------------------------------------------------------


CX_FUNCTION(bool DSL::detail::SexprString__Match, char const* text)

  CX_RETURN(('\"' == *text));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr::Type DSL::detail::SexprString__Skip,
                                                    char const** input)

  if (!SexprString__Match(*input))
    CX_ASSERT(false);  // TODO, need exception?

    U32 bslash=0;  // count of contiguous backslashes

  do
  {
    ++(*input);

    if (**input == '\\')
      ++bslash;
    else if (**input != '"')
      bslash = 0;

    // a '"' only marks the end of the string if it is
    // immediately preceded by an even number of backslashes
  } while (**input && (**input != '"' || (bslash % 2)));

  if (**input != '"')
    CX_THROW( DSL::Exception,
              DSL::Error::SYNTAX,
              "A closing '\"' was expected" );

  ++(*input);

  CX_RETURN(Sexpr::Type::STRING);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::SexprString__Parse, Context* sc,
                                                    char const** input)

  CX_RETURN(DSL::detail::parse<SexprString>(sc, input));

CX_ENDFUNCTION


// ---------------------------------------------------------------------


CX_CONSTRUCTOR(DSL::detail::SexprString::SexprString,
                                        : SexprLiteral(sc),
                                                    Context* sc,
                                                    char const** input)

  CX_ASSERT(**input == '"');

  // we don't want our duplicated text to include the terminal
  // double-quotation mark.
  const char* begin = (*input)+1;

  SexprString__Skip(input);

  // we don't want our duplicated text to include the terminal
  // double-quotation mark.
  const char* end = (*input)-1;

  // TODO, also need to de-escape control characters
  // so, replace 'DuplicateText' with some new code,
  // and maybe move the former back to DSL::Impl
  text_ = lexer::duplicate_text(begin, end);

CX_ENDMETHOD



CX_CONSTRUCTOR(DSL::detail::SexprString::SexprString,
                                        : SexprLiteral(sc),
                                                    Context* sc,
                                                    string const& text)

  text_ = text;

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::detail::SexprString::~SexprString)

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprString::Write)

  // TODO: strings that contain escape characters (such as '\n')
  // need to actually show show them in the output
  // TODO: and, to that end, this method should likely be doing
  // the actual output itself, and then returning #<void>.
  CX_RETURN("\"" + text_ + "\"");

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprString::Display)

  // TODO: strings that contain escape characters (such as '\n')
  // need to interpret them correctly in the output
  // TODO: and, to that end, this method shoudl likely be doing
  // the actual output itself, and then returning #<void>.

  // for strings, Display prints without surrounding with quotes
  CX_RETURN(text_);

CX_ENDMETHOD

