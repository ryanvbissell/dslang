// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-callable-closure.hpp"

#include "dslang-exception.hpp"
#include "dslang-dialect.hpp"
using namespace DSL::detail;

#include <string>
using std::string;


// ---------------------------------------------------------------------


CX_CONSTRUCTOR4(DSL::detail::SexprClosure::SexprClosure,
                    : SexprCallable(sc, name, CallType::REGULAR),
                      env_(env), formals_(formals), ast_(ast),
                                            Context* sc,
                                            string const& name,
                                            SexprEnv const* env,
                                            SexprCons const* formals,
                                            SexprCons const* ast)

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::detail::SexprClosure::~SexprClosure)

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprClosure::Call,
                                              SexprEnv const* env,
                                              SexprCons const* callargs)

  // We are ignoring the 'env' passed in here.  Since this is a
  // closure, it has already captured the env_-ironment it is
  // supposed to evaluate in
  SexprEnv* workenv(new SexprEnv(sc_, env_.get()));

  Sexpr const* result(nullptr);

  SexprCons const* exprs = ast_.get();
  SexprCons const* formals = formals_.get();

  SexprCONS args(callargs);
  tailcall: do
  {
    CX_ASSERT(formals);
    CX_ASSERT(args);
    // bind each formal to the provided value
    SexprCons const* iter = args.get();
    while (!is_nilp(formals) && !is_nilp(iter))
    {
      Sexpr const* formal = car(formals);
      Sexpr const* value = car(iter);
      workenv->SetVariable(formal->Write(), value);
      iter = as<SexprCons const*>(cdr(iter));
      formals = as<SexprCons const*>(cdr(formals));
    }
    if (is_nilp(formals) != is_nilp(iter))
      CX_ASSERT(false); // TODO, need exception for arity mismatch

    Sexpr const* expr = car(exprs);
    while (expr && !is_nilp(expr))
    {
      // if 'expr' is the last expression and is a cons...
      if (is_nilp(cdr(exprs)) && expr->IsCONS())
      {
        auto cons = as<SexprCons const*>(expr);
        // if the call-type is a closure...
        Sexpr const* item = car(cons)->eval(workenv);
        if (item->IsCLOSURE())
        {
          // tail-call elimination
          auto tail = as<SexprClosure const*>(item);
          exprs = tail->ast_.get();
          formals = tail->formals_.get();
          SexprCONS temp(SexprCons::EvalArgs(workenv, as<SexprCons const*>(cdr(cons))));
          args = std::move(temp);
          goto tailcall;
        }
      }
      // either a non-closure, or a non-tail position
      if (result && result->Nascent())
        delete result;

      result = expr->eval(workenv);

      exprs = as<SexprCons const*>(cdr(exprs));
      expr = car(exprs);
    }
  } while (false);

  delete workenv;

  CX_ASSERT(result);
  CX_RETURN(result);

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprClosure::Write)

  string const& temp = "#<closure:" + name_ + ">";
  CX_RETURN(temp);

CX_ENDMETHOD



