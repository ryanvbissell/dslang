// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLREP_HPP
#define DSLREP_HPP

#include "dslang.hpp"

#include <string>
#include <iostream>

using DSL::SEXPR;
using namespace std;

string dslrep(DSL::Scheme* scheme, const char* expr);

#define DSL_ASSERT(scheme,input,output)                               \
{                                                                     \
  string result = dslrep(scheme,input);                               \
  cout << input << " ==> " << result << " ... ";                      \
  if (dslrep(scheme,input) != output)                                 \
  {                                                                   \
    cout << "FAIL (expected " << output << ")" << endl;               \
    exit(-1);                                                         \
  }                                                                   \
  cout << "PASS\n";                                                   \
}

#endif

