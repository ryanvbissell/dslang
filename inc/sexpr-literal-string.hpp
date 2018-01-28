// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_STRING_HPP
#define DSLANG_SEXPR_STRING_HPP

#include "sexpr-literal.hpp"

#include <string>

namespace DSL
{

namespace detail
{

  class SexprString CX_FINAL : public SexprLiteral
  {
  public:
    SexprString(Context* sc, char const** input=nullptr);
    SexprString(Context* sc, std::string const& text);
    ~SexprString();

    std::string Write() const;
    std::string Display() const;

  protected:
    Type type() const override { return Sexpr::Type::STRING; }

  private:
    std::string text_;
  };

  bool SexprString__Match(char const* text);
  Sexpr::Type SexprString__Skip(char const** input);
  Sexpr const* SexprString__Parse(Context* sc, char const** input);

} // namespace detail

using SexprSTRING = CX::IntrusivePtr<detail::SexprString>;

} // namespace DSL


#endif // DSLANG_SEXPR_STRING_HPP

