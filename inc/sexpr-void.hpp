// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_VOID_HPP
#define DSLANG_SEXPR_VOID_HPP

#include "sexpr.hpp"

#include <string>

namespace DSL
{

namespace detail
{

  // void is the invisible value that won't print (but will display)
  class SexprVoid CX_FINAL : public Sexpr
  {
  public:
    static SexprVoid const* VOID;
    SexprVoid(Context *sc);
    virtual ~SexprVoid();

    std::string Print() const override;
    std::string Display() const override;
    std::string Write() const override;

  protected:
    Type type() const override { return Sexpr::Type::VOID; }
    Sexpr const* eval(SexprEnv const* env) const override;
  };

  bool is_void(Sexpr const* sexpr);

} // namespace detail

bool is_void(SEXPR const& sexpr);

using SexprVOID = CX::IntrusivePtr<DSL::detail::SexprVoid>;

} // namespace DSL


#endif // DSLANG_SEXPR_VOID_HPP

