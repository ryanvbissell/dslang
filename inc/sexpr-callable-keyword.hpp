// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_KEYWORD_HPP
#define DSLANG_SEXPR_KEYWORD_HPP

#include "sexpr-callable.hpp"

#include <string>

namespace DSL
{

namespace detail
{

  class SexprEnv;
  class SexprCons;

  // note that the env argument is not const; this is because some
  // keywords can modify the environment
  typedef Sexpr const* Keyword(SexprEnv*, SexprCons const*);

  class SexprKeyword CX_FINAL : public SexprCallable
  {
  public:
    SexprKeyword(Context* sc, std::string const& name,
                              Keyword* handler,
                              CallType type=CallType::REGULAR);

    std::string Write() const override;
    Sexpr const* Call(SexprEnv const* env, SexprCons const* args) const override;

  protected:
    Type type() const override { return Sexpr::Type::KEYWORD; }

  private:
    Keyword* handler_;
  };

} // namespace detail

using SexprKEYWORD = CX::IntrusivePtr<detail::SexprKeyword>;

} // namespace DSL

#endif // DSLANG_SEXPR_KEYWORD_HPP

