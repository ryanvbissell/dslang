// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_QUOTE_HPP
#define DSLANG_SEXPR_QUOTE_HPP

#include "sexpr.hpp"

#include <string.h>

namespace DSL
{

namespace detail
{

  class SexprQuote CX_FINAL : public Sexpr
  {
  public:
    SexprQuote(Context* sc, char const** input);

    std::string Write() const;

/*
    static bool Match(Dialect const& dialect, char const* text);
    static Sexpr::Type Skip(Dialect const& dialect, char const** input);
    static SEXPR Parse(Dialect const& dialect, char const** input);
*/

  protected:
    Type type() const override { return Sexpr::Type::QUOTE; }
    Sexpr const* transmute(char const** input) const override;
    Sexpr const* eval(SexprEnv const* env) const override;

  private:
    Sexpr::Type type_;
    std::string text_;
    virtual ~SexprQuote();
  };

  bool SexprQuote__Match(char const* text);
  Sexpr::Type SexprQuote__Skip(char const** input);
  Sexpr const* SexprQuote__Parse(Context* sc, char const** input);

} // namespace detail

using SexprQUOTE = CX::IntrusivePtr<detail::SexprQuote>;

} // namespace DSL


#endif // DSLANG_SEXPR_QUOTE_HPP

