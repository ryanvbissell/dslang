// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2018, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SCHEME_IMPL_HPP
#define DSLANG_SCHEME_IMPL_HPP

#include "dslang-scheme-context.hpp"
#include "dslang-dialect.hpp"

namespace DSL {
namespace detail {

  template <class D>
  class SchemeImpl: public Context
  {
  public:
    SchemeImpl() { dialect_.SetContext(this); }
    virtual ~SchemeImpl() {};

    Dialect const& dialect() const { return dialect_; }

  private:
    D dialect_;
  };

} // namespace detail
} // namespace DSL

#endif  // DSLANG_SCHEME_IMPL_HPP


