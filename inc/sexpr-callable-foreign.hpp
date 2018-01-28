// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_FOREIGN_HPP
#define DSLANG_SEXPR_FOREIGN_HPP

#include "sexpr-callable.hpp"

#include <string>
#include <functional>

namespace DSL
{

namespace detail
{
  class Sexpr;
  class SexprEnv;
  class SexprCons;
}

typedef std::function<DSL::detail::Sexpr const* ( DSL::detail::SexprEnv const*,
                                                  std::string const&,
                                                  DSL::detail::SexprCons const*)> ForeignFunc;

namespace detail
{

  class SexprForeign CX_FINAL : public SexprCallable
  {
  public:
    SexprForeign(Context* sc, std::string const& name,
                              ForeignFunc const& ffunc);

    std::string Write() const override;
    Sexpr const* Call(SexprEnv const* env, SexprCons const* args) const override;

  protected:
    Type type() const override { return Sexpr::Type::FOREIGN; }

  private:
    ForeignFunc const ffunc_;

  public:
    virtual ~SexprForeign();
  };

} // namespace detail

using SexprFOREIGN = CX::IntrusivePtr<detail::SexprForeign>;

} // namespace DSL


#endif // DSLANG_SEXPR_FOREIGN_HPP

