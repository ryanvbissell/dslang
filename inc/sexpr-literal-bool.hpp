// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_BOOL_HPP
#define DSLANG_SEXPR_BOOL_HPP

#include "sexpr-literal.hpp"

#include <string>

namespace DSL
{

namespace detail
{

  class SexprBool CX_FINAL : public SexprLiteral
  {
    friend class CX::IntrusivePtr<SexprBool>; // TODO this is getting silly (destructor)
  public:
    static SexprBool const* TRUE;
    static SexprBool const* FALSE;
    SexprBool(Context* sc);  // used to construct canonicals
    ~SexprBool();

/*
    static bool Match(Dialect const& dialect, char const* text);
    static Sexpr::Type Skip(Dialect const& dialect, char const** input);
    static SEXPR Parse(Dialect const& dialect, char const** input);
*/
    std::string Write() const override;

  protected:
    Sexpr const* transmute(char const** input) const override;
    Type type() const override { return Sexpr::Type::BOOL; }
  };

 bool SexprBool__Match(char const* text);
 Sexpr::Type SexprBool__Skip(char const** input);
 Sexpr const* SexprBool__Parse(Context* sc, char const** input);

  // canonical #t and #f objects
  bool is_false(Sexpr const* sexpr);
  bool is_true(Sexpr const* sexpr);
  bool is_canonical_true(Sexpr const* sexpr);

} // namespace detail

bool is_false(SEXPR const& sexpr);  // canonical-false value
bool is_true(SEXPR const& sexpr);   // any non-false value

bool is_canonical_true(SEXPR const& sexpr);

using SexprBOOL = CX::IntrusivePtr<detail::SexprBool>;

} // namespace DSL


#endif // DSLANG_SEXPR_BOOL_HPP

