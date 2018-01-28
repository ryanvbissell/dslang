// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#include "cx-exceptions.hpp"
#include "dslrep.hpp"

#include <string>

using DSL::SEXPR;
using std::string;

string dslrep(DSL::Scheme* scheme, const char* expr)
{
  string result;

  try
  {
    while (expr && *expr)
    {
      const SEXPR& read = scheme->Read(&expr);
      const SEXPR& eval = scheme->Eval(read);

      // The idiomatic think to do would be 'scheme->Print(eval)',
      // but that adds a newline that breaks our string comparisons
      result += eval->Write();
    }
  }
  catch (...)
  {
    // TODO, need to exit/assert here
    return "BAD";
  }
  return result;
}

