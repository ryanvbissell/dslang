// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "dslang-lexer.hpp"
#include "dslang-exception.hpp"

#include <string>
using std::string;


// ---------------------------------------------------------------------


CX_FUNCTION(string DSL::lexer::duplicate_text,  const char* start,
                                                const char* end)

  // here, 'end' is expected to refer to the location immediately
  // after the last byte of the text to strdup.
  size_t len = end - start;
  string text(start, len);

  CX_RETURN(text);

CX_ENDFUNCTION


#define DSL_NEWLINE  "\n\r"
#define DSL_WHITESPACE  DSL_NEWLINE " \t\f\v"
#define DSL_DELIMS  DSL_WHITESPACE ";#()\""
#define DSL_EOL_COMMENT ';'


CX_FUNCTION(bool DSL::lexer::is_delim, char text)

  bool delim = !!::strchr(DSL_DELIMS, text);
  CX_RETURN(delim);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::lexer::is_whitespace, char text)

  bool white = !!::strchr(DSL_WHITESPACE, text);
  CX_RETURN(white);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::lexer::is_comment, char const* text)

  // TODO, multi-line comments
  CX_RETURN(DSL_EOL_COMMENT == *text);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::lexer::is_empty_list, char const* text)

  // TODO: support other bracket flavors
  DSL::lexer::skip_any_whitespace(&text);
  if (*text != '(')
    CX_RETURN(false);

  ++text;

  DSL::lexer::skip_any_whitespace(&text);
  if (*text != ')')
    CX_RETURN(false);

  CX_RETURN(true);

CX_ENDFUNCTION



CX_FUNCTION(void DSL::lexer::skip_past_newline, char const** input)

  CX_ASSERT(input);
  CX_ASSERT(*input);

  const char* pdelim = strpbrk(*input, DSL_NEWLINE);
  if (pdelim)
    *input = pdelim + 1;
  else
    *input += strlen(*input); // null terminator

CX_ENDFUNCTION



CX_FUNCTION(void DSL::lexer::skip_past_comment, char const** input)

  // TODO: multi-line comments
  CX_ASSERT(DSL_EOL_COMMENT == **input);
  skip_past_newline(input);

CX_ENDFUNCTION



CX_FUNCTION(void DSL::lexer::skip_any_whitespace, char const** input)

  CX_ASSERT(input);
  CX_ASSERT(*input);

  // NOTE: this also treats comments as whitespace

  bool done = false;
  size_t count;
  do
  {
    count = strspn(*input, DSL_WHITESPACE);
    *input += count;

    if (is_comment(*input))
        skip_past_comment(input);  // now check whitespace again
    else
      done = true;

  } while (!done);

CX_ENDFUNCTION



CX_FUNCTION(void DSL::lexer::skip_non_whitespace, char const** input)

  CX_ASSERT(input);
  CX_ASSERT(**input);

  size_t count = strcspn(*input, DSL_WHITESPACE);
  *input += count;

CX_ENDMETHOD



CX_FUNCTION(void DSL::lexer::skip_to_delim, char const** input)

  CX_ASSERT(input);
  CX_ASSERT(*input);

  char const* pdelim = strpbrk(*input, DSL_DELIMS);
  if (pdelim)
    *input = pdelim;
  else
    *input += strlen(*input); // null terminator

CX_ENDFUNCTION



CX_FUNCTION(void DSL::lexer::skip_past_ident, char const** input)

  char const* begin = *input;

  skip_to_delim(input);

  if (*input == begin)
  {
    // it appears we were already up against a delim, which
    // implies that there is no identifier to skip past at this
    // location.  We'll treat this as a 'bad identifier'.  So
    // that the user knows what we are talking about, we'll scan
    // ahead to the next occurrence of whitespace so that we can
    // report some context in our syntax error message.
    skip_non_whitespace(input);
    string const& bad = duplicate_text(begin, *input);
    *input = begin;
    CX_THROW( DSL::Exception,
              DSL::Error::SYNTAX,
              "Unexpected \"%c\" encountered at "
              "beginning of lexeme \"%s\"",
              *begin, bad.c_str() );
  }

CX_ENDFUNCTION


