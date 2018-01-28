// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_CLOSURE_HPP
#define DSLANG_SEXPR_CLOSURE_HPP

#include "sexpr-callable.hpp"
#include "sexpr-cons.hpp"
#include "sexpr-env.hpp"

#include <string>

namespace DSL
{

namespace detail
{

  class SexprClosure CX_FINAL : public SexprCallable
  {
  public:
    SexprClosure(Context* sc, std::string const& name,
                              SexprEnv const* env,
                              SexprCons const* formals,
                              SexprCons const* ast);

    void Mark() const override  { ast_->Mark(); formals_->Mark(); Sexpr::Mark(); }


    std::string Write() const override;
    Sexpr const* Call(SexprEnv const* env, SexprCons const* args) const override;

  protected:
    Type type() const override { return Sexpr::Type::CLOSURE; }

  private:
    SexprENV env_;
    SexprCONS ast_;
    SexprCONS formals_;

  public:
    virtual ~SexprClosure();
  };

} // namespace detail

using SexprCLOSURE = CX::IntrusivePtr<detail::SexprClosure>;

} // namespace DSL


#endif // DSLANG_SEXPR_CLOSURE_HPP

