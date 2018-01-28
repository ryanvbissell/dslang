// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#include "dslrep.hpp"
#include <string>

using DSL::SEXPR;
using std::string;




int main(int argc, const char** argv)
{
  DSL::Scheme* scheme = DSL::SchemeFactory();

  DSL_ASSERT(scheme,"'( 8 13 )","(8 13)");
  DSL_ASSERT(scheme,"'(8 . (13 . ()))","(8 13)");
}

