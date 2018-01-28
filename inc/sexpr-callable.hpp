// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_CALLABLE_HPP
#define DSLANG_SEXPR_CALLABLE_HPP

#include "sexpr.hpp"

#include <string>


#if CX_OPT_TRACING
#define CX_CAPTURE(...) cx_trace_methodname,__VA_ARGS__
#define CX_TRACE_METHODNAME cx_trace_methodname
#else
#define CX_CAPTURE(...) __VA_ARGS__
#define CX_TRACE_METHODNAME
#endif


#define DSL_CONTRACT(name, argN, TYPE, sexpr)                         \
  if (!sexpr->Is##TYPE())                                             \
  { CX_THROW( DSL::Exception,                                         \
              DSL::Error::CONTRACT,                                   \
              "'" name "' expects " #TYPE                             \
              " for argument " #argN );                               \
  }


#define DSL_ARITY(name, count, sexpr)                                 \
  if (sexpr->Length() != count)                                       \
  {  CX_THROW( DSL::Exception,                                        \
              DSL::Error::ARITY,                                      \
              "'" name "' requires exactly "#count" argument(s), "    \
              "but %d were given.", sexpr->Length() );                \
  }


#define DSL_ARITY_MIN(name, min, sexpr)                               \
  if (sexpr->Length() < min)                                          \
  { CX_THROW( DSL::Exception,                                         \
              DSL::Error::ARITY,                                      \
              "'" name "' requires a minimum of "#min" argument(s).");\
  }


#define DSL_ARITY_RANGE(name, lo, hi, sexpr)                          \
  if ((sexpr->Length() < lo) || (sexpr->Length() > hi))               \
  { CX_THROW( DSL::Exception,                                         \
              DSL::Error::ARITY,                                      \
              "'" name "' requires between "#lo" and "#hi" arguments.");\
  }

#if CX_OPT_TRACING
#undef CX_TRACE_METHODNAME
#undef CX_CAPTURE
#endif

namespace DSL
{

namespace detail
{
  class SexprCons;
}

using SexprCONS = CX::IntrusivePtr<detail::SexprCons>;
enum class CallType { REGULAR=0, SPECIAL=1 };

namespace detail
{


class SexprCallable : public Sexpr
{
friend class CX::IntrusivePtr<SexprCallable>; // for destructor access
public:
  SexprCallable(Context* sc,  std::string const& name,
                              CallType type=CallType::REGULAR);

  bool IsCallableType() const override { return true; }
  virtual bool IsSpecial() const { return type_ == CallType::SPECIAL; }
  virtual Sexpr const* Call(SexprEnv const* env, SexprCons const* args) const=0;

protected:
  std::string name_;
  CallType type_;
  Sexpr const* eval(SexprEnv const* env) const final;
};

} // namespace detail

using SexprCALLABLE = CX::IntrusivePtr<detail::SexprCallable>;

} // namespace DSL

#endif // DSLANG_SEXPR_CALLABLE_HPP

