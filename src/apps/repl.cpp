// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "repl"

#include "dslang.hpp"

#include <stdlib.h>

// ---------------------------------------------------------------------

CX_FUNCTION(int32_t count_closures, bool* empty, const char* text)

  int32_t count=0;

  // In this narrow context, a 'closure' is just a set of
  // balanced parentheses.  This function will return 0
  // for balanced strings, positive for more o-parens than
  // c-parens, and negative for mostly c-parens.
  // The idea is that the caller can pass a string on to
  // the reader anytime the net value is zero or less (although,
  // less will generally result in an error message.)
  //
  // It will also set *empty to false if any non-whitespace
  // character is seen.
  *empty = true;
  while (*text && count >= 0)
  {
    if (*empty && !isspace(*text))
      *empty = false;
    if (*text == '(')
      count++;
    else if (*text == ')')
      count--;

    ++text;
  }

  CX_RETURN(count);

CX_ENDMETHOD



CX_FUNCTION(void scheme_repl)

  DSL::Scheme* scheme = DSL::SchemeFactory<DSL::Dialect>();

  char buf[1024];
  const char* ptr;

  bool eof = false;
  while (!eof)
  {
    bool empty=true;
    int32_t parens = 0;
    std::string input = "";
    do
    {
      printf((input == "") ? "dsl> " : "   ");
      eof = !fgets(buf, 1024, stdin);
      if (!eof)
      {
        input += buf;
        parens = count_closures(&empty, input.c_str());
      }
    }
    while (!eof && (empty || (parens > 0)));

    while (input != "")
    {
      ptr = input.c_str();
      CX_TRY
      {
        std::cout << scheme->ReadEvalPrint(&ptr) << std::endl;
      }
      CX_CATCH(const DSL::Exception& e)
      {
        std::cerr << "!!! " << e.Name()   << ":" << std::endl;
        std::cerr << "    " << e.Why()    << std::endl;
        std::cerr << "    " << e.Reason() << std::endl;
      }
      CX_ENDTRY

      // it could be that the reader did not consume all of the
      // input data, so we need to recover any remainder and
      // recycle it for the next iteration through this bottom loop
      //
      // since ptr points into the c_str of 'input', it
      // is probably not safe to use it to re-assign input.
      // consequently, I use a temporary std::string during
      // the assignment
      input = std::string(ptr);
    }
  }
  std::cout << "\nGoodbye.\n" << std::endl;

  delete scheme;

  // TODO, this call is no longer possible because 'report()'
  // is no longer static
#if 0
  DSL::detail::Context::report();
#endif

CX_ENDMETHOD



CX_FUNCTION(int main, int argc, char** const argv)

  CX_TRY
  {
    scheme_repl();
  }
  CX_CATCH(CX::Exception& e)
  {
    e.StdError();
    CX_RETURN(EXIT_FAILURE);
  }
  CX_ENDTRY

  CX_DEBUGOUT("REPL has been exited.\n");
  CX_RETURN(EXIT_SUCCESS);

CX_ENDFUNCTION



