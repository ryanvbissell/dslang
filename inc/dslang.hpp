// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2018, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_HPP
#define DSLANG_HPP

#include "dslang-scheme.hpp"
#include "dslang-dialect.hpp"

#include "dslang-exception.hpp"

namespace DSL
{

  template <class D = Dialect> Scheme* SchemeFactory();

} // namespace DSL

#include "dslang-scheme-impl.hpp"

template <class D>
DSL::Scheme* DSL::SchemeFactory()
{
  return new DSL::detail::SchemeImpl<D>;
}

#endif  // DSLANG_HPP


