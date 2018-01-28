// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_IDENT_HPP
#define DSLANG_SEXPR_IDENT_HPP

#include "sexpr.hpp"

#include <string>

namespace DSL
{

class Dialect;

namespace detail
{

  class SexprIdent CX_FINAL : public Sexpr
  {
  friend class CX::IntrusivePtr<SexprIdent>;
  public:
    SexprIdent(Context* sc, char const** input);

    std::string Write() const;

/*
    static bool Match(Dialect const& dialect, char const* text);
    static Sexpr::Type Skip(Dialect const& dialect, char const** input);
    static SEXPR Parse(Dialect const& dialect, char const** input);
*/

  protected:
    Type type() const override { return Sexpr::Type::IDENT; }
    Sexpr const* transmute(char const** input) const override;
    Sexpr const* eval(SexprEnv const* env) const override;

  private:
    std::string name_;
    virtual ~SexprIdent();
  };

  bool SexprIdent__Match(char const* text);
  Sexpr::Type SexprIdent__Skip(char const** input);
  Sexpr const* SexprIdent__Parse(Context* sc, char const** input);

} // namespace detail

using SexprIDENT = CX::IntrusivePtr<detail::SexprIdent>;

} // namespace DSL


#endif // DSLANG_SEXPR_IDENT_HPP

