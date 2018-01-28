// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "dslang-exception.hpp"


// ---------------------------------------------------------------------


#define X(v,str) { DSL::Error::v, "DSL::Error::"#v },
std::map<DSL::Error, const char*> DSL::ErrorNameMap =
{
  DSL_EXCEPTIONS
};
#undef X

#define X(v,str) { DSL::Error::v, str },
std::map<DSL::Error, const char*> DSL::ErrorStringMap =
{
  DSL_EXCEPTIONS
};
#undef X

