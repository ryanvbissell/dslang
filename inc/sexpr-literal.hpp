// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_LITERAL_HPP
#define DSLANG_SEXPR_LITERAL_HPP

#include "sexpr.hpp"

#include <string>

namespace DSL
{

namespace detail
{

  class SexprLiteral : public Sexpr
  {
  public:
    SexprLiteral(Context* sc);
    bool IsLiteralType() const override { return true; }
  };

} // namespace detail

using SexprLITERAL = CX::IntrusivePtr<detail::SexprLiteral>;

} // namespace DSL


#endif // DSLANG_SEXPR_LITERAL_HPP

