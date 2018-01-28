// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_CONS_HPP
#define DSLANG_SEXPR_CONS_HPP

#include "sexpr.hpp"

#include <string>

namespace DSL
{

namespace detail
{

  class SexprCons CX_FINAL : public Sexpr
  {
  template <class U> friend class CX::IntrusivePtr; // for destructor access
  friend class DSL::detail::SexprClosure; // for destructor access
  public:
    static SexprCons const* NIL;

    SexprCons(Context* sc);
    SexprCons(Context* sc, const char** input);
    SexprCons(Context* sc, Sexpr const* car, Sexpr const* cdr);

    void Mark() const override { if (cdr_) cdr_->Mark(); if (car_) car_->Mark(); Sexpr::Mark(); }

/*
    static bool Match(Dialect const& dialect, char const* text);
    static Sexpr::Type Skip(Dialect const& dialect, char const** input);
    static SEXPR Parse(Dialect const& dialect, char const** input);
*/
    std::string Write() const override;

    Sexpr const* car() const { return car_; }
    Sexpr const* cdr() const { return cdr_; }

    U32 Length() const;

    static SexprCons const* EvalArgs(SexprEnv const* env, SexprCons const* args);

  protected:
    Type type() const { return Sexpr::Type::CONS; }
    Sexpr const* transmute(const char** input) const override;
    Sexpr const* eval(SexprEnv const* env) const override;

  private:
    SEXPR memo_;
    Sexpr const* car_;
    Sexpr const* cdr_;
    virtual ~SexprCons();
  };

  bool SexprCons__Match(char const* text);
  Sexpr::Type SexprCons__Skip(char const** input);
  Sexpr const* SexprCons__Parse(Context* sc, char const** input);

  Sexpr const* car(SexprCons const* cons);
  Sexpr const* cdr(SexprCons const* cons);

  Sexpr const* caar(SexprCons const* cons);
  Sexpr const* cadr(SexprCons const* cons);
  Sexpr const* cdar(SexprCons const* cons);
  Sexpr const* cddr(SexprCons const* cons);

  Sexpr const* caddr(SexprCons const* cons);

  bool is_nilp(Sexpr const* sexpr);

} // namespace detail

using SexprCONS = CX::IntrusivePtr<detail::SexprCons>;

SEXPR car(SexprCONS const& cons);
SEXPR cdr(SexprCONS const& cons);

SEXPR caar(SexprCONS const& cons);
SEXPR cadr(SexprCONS const& cons);
SEXPR cdar(SexprCONS const& cons);
SEXPR cddr(SexprCONS const& cons);

SEXPR caddr(SexprCONS const& cons);

bool is_nil(SEXPR const& sexpr);
bool is_nil(SexprCONS const& sexpr); // TODO, why?

} // namespace DSL

#endif // DSLANG_SEXPR_CONS_HPP

