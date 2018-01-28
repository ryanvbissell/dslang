// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_CUSTOM_HPP
#define DSLANG_SEXPR_CUSTOM_HPP

#include "sexpr.hpp"

#include <string>

namespace DSL
{

namespace detail
{

  class SexprCustom : public Sexpr
  {
  public:
    SexprCustom(Context* sc, std::string const& lexeme);

    std::string Write() const override;

  protected:
    Type type() const override { return Sexpr::Type::CUSTOM; }

  private:
    std::string lexeme_;
  };

} // namespace detail

using SexprCUSTOM = CX::IntrusivePtr<detail::SexprCustom>;

} // namespace DSL


#endif // DSLANG_SEXPR_CUSTOM_HPP

