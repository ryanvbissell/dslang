// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2015-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "dslang-keywords.hpp"

#include "dslang-exception.hpp"
#include "dslang-dialect.hpp"
#include "sexpr-void.hpp"
#include "sexpr-literal-bool.hpp"
#include "sexpr-callable-closure.hpp"
#include "sexpr-literal-number.hpp"
#include "sexpr-callable-keyword.hpp"
#include "sexpr-env.hpp"
#include "sexpr-cons.hpp"
using namespace DSL::detail;

#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;


// ---------------------------------------------------------------------


CX_FUNCTION(Sexpr const* DSL::detail::keyword__load,  SexprEnv* env,
                                               SexprCons const* args)

  CX_RETURN(SexprVoid::VOID);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__void,  SexprEnv* env,
                                               SexprCons const* args)

  // (void) just ignores its arguments.
  // it seems to not be a special form in Racket, so
  // I didn't make it be one here.
  CX_RETURN(SexprVoid::VOID);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__not,  SexprEnv* env,
                                              SexprCons const* args)

  DSL_ARITY("not", 1, args);

  Sexpr const* arg1 = car(args);
  CX_RETURN(is_false(arg1)  ? SexprBool::TRUE
                            : SexprBool::FALSE);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__booleanp, SexprEnv* env,
                                                  SexprCons const* args)

  DSL_ARITY("boolean?", 1, args);

  Sexpr const* arg1 = car(args);
  CX_RETURN(arg1->IsBOOL()  ? SexprBool::TRUE
                            : SexprBool::FALSE);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__car, SexprEnv* env,
                                             SexprCons const* args)

  DSL_ARITY("car", 1, args);

  Sexpr const* pair = car(args);
  DSL_CONTRACT("car", 1, CONS, pair);

  CX_RETURN(car(as<SexprCons const*>(pair)));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__cdr, SexprEnv* env,
                                             SexprCons const* args)

  DSL_ARITY("cdr", 1, args);

  Sexpr const* pair = car(args);
  DSL_CONTRACT("cdr", 1, CONS, pair);

  CX_RETURN(cdr(as<SexprCons const*>(pair)));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__caar,  SexprEnv* env,
                                               SexprCons const* args)

  DSL_ARITY("caar", 1, args);

  Sexpr const* pair = car(args);
  DSL_CONTRACT("caar", 1, CONS, pair);

  CX_RETURN(caar(as<SexprCons const*>(pair)));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__cadr,  SexprEnv* env,
                                               SexprCons const* args)

  DSL_ARITY("cadr", 1, args);

  Sexpr const* pair = car(args);
  DSL_CONTRACT("cadr", 1, CONS, pair);

  CX_RETURN(cadr(as<SexprCons const*>(pair)));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__cdar,  SexprEnv* env,
                                               SexprCons const* args)

  DSL_ARITY("cdar", 1, args);

  Sexpr const* pair = car(args);
  DSL_CONTRACT("cdar", 1, CONS, pair);

  CX_RETURN(cdar(as<SexprCons const*>(pair)));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__cddr,  SexprEnv* env,
                                               SexprCons const* args)

  DSL_ARITY("cddr", 1, args);

  Sexpr const* pair = car(args);
  DSL_CONTRACT("cddr", 1, CONS, pair);

  CX_RETURN(cddr(as<SexprCons const*>(pair)));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__cons,  SexprEnv* env,
                                               SexprCons const* args)

  DSL_ARITY("cons", 2, args);

  Sexpr const* arg1 = car(args);
  Sexpr const* arg2 = cadr(args);

  CX_RETURN(new SexprCons(env->context(), arg1, arg2));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__eval,  SexprEnv* env,
                                               SexprCons const* args)

  DSL_ARITY("eval", 1, args);

  Sexpr const* arg = car(args);
  Sexpr const* result = arg->eval(env);

  CX_RETURN(result);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__list,  SexprEnv* env,
                                               SexprCons const* args)

  // 'args' is already the list we're supposed to be making.
  CX_RETURN(args);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__eq,  SexprEnv* env,
                                             SexprCons const* args)

  DSL_ARITY("eq?", 2, args);

  Sexpr const* arg1 = car(args);
  Sexpr const* arg2 = cadr(args);

  // two objects are eq? IFF they refer to the same object
  if (arg1 == arg2)
    CX_RETURN(SexprBool::TRUE);

  CX_RETURN(SexprBool::FALSE);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__eqv, SexprEnv* env,
                                             SexprCons const* args)

  // TODO
  // except for numbers and characters, two values are eqv IFF
  // they are eq.
  //    * numbers are only eqv if they have the same exactness,
  //      precision, and value
  //    * characters are only eqv if their char->integer results
  //      are equal

  DSL_ARITY("eqv?", 2, args);

  Sexpr const* arg1 = car(args);
  Sexpr const* arg2 = cadr(args);

  if (arg1->IsLiteralType() && arg2->IsLiteralType())
  {
    if (is_canonical_true(arg1) && is_canonical_true(arg2))
      CX_RETURN(SexprBool::TRUE);

    if (is_false(arg1) && is_false(arg2))
      CX_RETURN(SexprBool::TRUE);

    if (arg1->IsNUMBER() && arg2->IsNUMBER())
    {
      auto narg1 = as<SexprNumber const*>(arg1);
      auto narg2 = as<SexprNumber const*>(arg2);
      if (*narg1 == *narg2)
        CX_RETURN(SexprBool::TRUE);

      CX_RETURN(SexprBool::FALSE);
    }
  }

  if (arg1->IsIDENT() && arg2->IsIDENT())
  {
    if (arg1 == arg2)  // compare addresses
      CX_RETURN(SexprBool::TRUE);

    CX_RETURN(SexprBool::FALSE);
  }

  if (is_nil(arg1) && is_nil(arg2))
    CX_RETURN(SexprBool::TRUE);

  // TODO: characters, pairs, strings, closures
  // a lot more left to do here!

  CX_RETURN(SexprBool::FALSE);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__equalp, SexprEnv* env,
                                                SexprCons const* args)

  // TODO
  // two values are equal? IFF they are eqv?, except:
  //  * strings, pairs, vectors, etc, which must be iteratively
  //    or recursively inspected

  CX_RETURN(SexprBool::FALSE);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__quote, SexprEnv* env,
                                               SexprCons const* args)

  DSL_ARITY("quote", 1, args);

  CX_RETURN(car(args));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__define, SexprEnv* env,
                                                SexprCons const* args)

  DSL_ARITY_MIN("define", 2, args);

  Context* sc = env->context();
  Sexpr const* arg1 = car(args);

  if (arg1->IsCONS())
  {
    // we are defining a function
    auto cons1 = as<SexprCons const*>(arg1);

    Sexpr const* car1 = car(cons1);
    DSL_CONTRACT("define", 1, IDENT, car1);
    std::string const& name = as<SexprIdent const*>(car1)->Write();

    auto formals = as<SexprCons const*>(cdr(cons1));
    auto ast = as<SexprCons const*>(cdr(args));

    Sexpr const* closure(new SexprClosure(sc, name, env, formals, ast));
    env->SetVariable(name, closure);
  }
  else
  {
    // we are defining a variable
    DSL_ARITY("define", 2, args);
    DSL_CONTRACT("define", 1, IDENT, arg1);

    auto name = as<SexprIdent const*>(arg1);
    Sexpr const* arg2 = cadr(args)->eval(env);
    env->SetVariable(name->Write(), arg2);
  }

  CX_RETURN(SexprVoid::VOID);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__lambda,  SexprEnv* env,
                                                 SexprCons const* args)

  Sexpr const* arglist = car(args);
  Sexpr const* body = cdr(args);

  DSL_CONTRACT("lambda", 1, CONS, arglist);
  auto formals = as<SexprCons const*>(arglist);

  DSL_CONTRACT("lambda", 2, CONS, body);
  auto ast = as<SexprCons const*>(body);

  Context* sc = env->context();
  CX_RETURN(new SexprClosure(sc, "<lambda>", env, formals, ast));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::keyword__print, SexprEnv* env,
                                               SexprCons const* args)

  Sexpr const* arg1 = car(args);

  cout << arg1->Print();  // TODO: need something akin to env->cout
  CX_RETURN(SexprVoid::VOID);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__newline, SexprEnv* env,
                                                 SexprCons const* args)

  cout << endl;  // TODO: need something akin to env->cout
  CX_RETURN(SexprVoid::VOID);

CX_ENDMETHOD


CX_FUNCTION(Sexpr const* DSL::detail::keyword__write, SexprEnv* env,
                                                  SexprCons const* args)

  Sexpr const* arg1 = car(args);

  // TODO: Note there is a bug here, which is actually within
  // the parser: escapes such as '\n' need to be converted to
  // the appropriate bytes so that they have effect when we
  // do the 'cout' below
  cout << arg1->Write();  // TODO: need something akin to env->cout
  CX_RETURN(SexprVoid::VOID);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__display, SexprEnv* env,
                                                 SexprCons const* args)

  Sexpr const* arg1 = car(args);

  // TODO: Note there is a bug here, which is actually within
  // the parser: escapes such as '\n' need to be converted to
  // the appropriate bytes so that they have effect when we
  // do the 'cout' below
  cout << arg1->Display();  // TODO: need something akin to env->cout
  CX_RETURN(SexprVoid::VOID);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__if,  SexprEnv* env,
                                             SexprCons const* args)

  Sexpr const* test = car(args);

  // 'if' is a special-form, so must eval 'test' ourselves herein.
  if (!is_false(test->eval(env)))
  {
    // then
    Sexpr const* consequent = cadr(args);
    CX_RETURN(consequent->eval(env));
  }

  if (is_nilp(cddr(args))) // no 'else' sub-expression
    CX_RETURN(SexprVoid::VOID);

  // else
  Sexpr const* alternate = caddr(args);
  CX_RETURN(alternate->eval(env));

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__cond,  SexprEnv* env,
                                               SexprCons const* args)

  // TODO: add support for '=>' syntactic keyword

  // We use 'Sexpr*' below where possible, to minimize
  // CX::IntrusivePtr copying (reference to const is not
  // possible for many of them, since they get updated
  // within a loop)
  Sexpr const* last;
  SexprCons const* clauses = as<SexprCons const*>(args);
  // TODO, is clause guaranteed to be a CONS, here?
  SexprCons const* clause = as<SexprCons const*>(clauses->car());
  while (clause && !is_nilp(clause)) // TODO: SexprCons::iterator ?
  {
    // cond is a special-form, so must eval
    auto test = clause->car();
    if (!is_false(test->eval(env)))
    {
      SexprCons const* exprs = as<SexprCons const*>(clause->cdr());
      Sexpr const* expr = exprs->car();
      while (expr && !is_nilp(expr))
      {
        // cond is a special-form, so must eval each clause
        last = expr->eval(env);  // TODO, make sure move-assignment happens here
        exprs = as<SexprCons const*>(exprs->cdr());
        expr = exprs->car();
      }
      CX_RETURN(last);
    }
    clauses = as<SexprCons const*>(clauses->cdr());
    clause = as<SexprCons const*>(clauses->car());
  }

  // TODO, Racket returns #<void> if there were zero cond-clauses

  // undefined, but we'll return #f when no tests were true
  CX_RETURN(SexprBool::FALSE);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__and, SexprEnv* env,
                                             SexprCons const* args)

  // We use 'Sexpr*' below where possible, to minimize
  // shared_ptr copying (reference to const is not
  // possible for many of them, since they get updated
  // within a loop)
  Sexpr const* eval = SexprBool::TRUE;

  SexprCons const* exprs = as<SexprCons const*>(args);
  Sexpr const* expr = exprs->car();
  while (expr && !is_nil(expr))
  {
    eval = expr->eval(env);
    if (is_false(eval))
      CX_RETURN(SexprBool::FALSE);

    exprs = as<SexprCons const*>(exprs->cdr());
    expr = exprs->car();
  }

  CX_RETURN(eval);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__or,  SexprEnv* env,
                                             SexprCons const* args)

  // We use 'Sexpr*' below where possible, to minimize
  // shared_ptr copying (reference to const is not
  // possible for many of them, since they get updated
  // within a loop)
  Sexpr const* eval = SexprBool::FALSE;

  SexprCons const* exprs = as<SexprCons const*>(args);
  Sexpr const* expr = exprs->car();
  while (expr && !is_nilp(expr))
  {
    eval = expr->eval(env);
    if (is_true(eval))
      CX_RETURN(eval);

    exprs = as<SexprCons const*>(exprs->cdr());
    expr = exprs->car();
  }

  CX_RETURN(eval);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__set, SexprEnv* env,
                                             SexprCons const* args)

  DSL_ARITY("set!", 2, args);

  Sexpr const* arg1 = car(args);
  DSL_CONTRACT("set!", 1, IDENT, arg1);
  SexprIdent const* ident = as<SexprIdent const*>(arg1);
  string const& name = ident->Write();

  SEXPR const& oldvalue = env->GetVariable(name);
  if (!oldvalue)
  {
    CX_THROW( DSL::Exception,
              DSL::Error::UNDEFINED,
              "Cannot set unbound symbol '%s'.",
              name.c_str());
  }

  Sexpr const* newvalue = cadr(args);
  env->SetVariable(name, newvalue);
  // TODO: verify that oldvalue gets GC'd

  CX_RETURN(SexprVoid::VOID);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__number_eq, SexprEnv* env,
                                                   SexprCons const* args)

  Sexpr const* arg1 = car(args);
  Sexpr const* arg2 = cadr(args);

  if (!(arg1->IsNUMBER() && arg2->IsNUMBER()))
    CX_ASSERT(false); // TODO, throw contract exception

  auto narg1 = as<SexprNumber const*>(arg1);
  auto narg2 = as<SexprNumber const*>(arg2);

  if (*narg1 == *narg2)
    CX_RETURN(SexprBool::TRUE);

  CX_RETURN(SexprBool::FALSE);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__add, SexprEnv* env,
                                             SexprCons const* args)

  SEXPR const& car = DSL::car(args);
  SEXPR const& cadr = DSL::cadr(args);

// TODO, this would be a spiffy syntax
//SexprNUMBER arg1 = DSL_CONTRACT("add", 1, NUMBER, car(args));
//SexprNUMBER arg2 = DSL_CONTRACT("add", 2, NUMBER, cadr(args));

  // TODO, should support n-arity
  DSL_CONTRACT("add", 1, NUMBER, car);
  DSL_CONTRACT("add", 2, NUMBER, cadr);

  SexprNumber const* narg1 = as<SexprNumber const*>(car.get());
  SexprNumber const* narg2 = as<SexprNumber const*>(cadr.get());

  CX_RETURN(new SexprNumber(*narg1 + *narg2));

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__sub, SexprEnv* env,
                                             SexprCons const* args)

  SEXPR const& car = DSL::car(args);
  SEXPR const& cadr = DSL::cadr(args);

  // TODO, should support n-arity
  DSL_CONTRACT("add", 1, NUMBER, car);
  DSL_CONTRACT("add", 2, NUMBER, cadr);

  SexprNumber const* narg1 = as<SexprNumber const*>(car.get());
  SexprNumber const* narg2 = as<SexprNumber const*>(cadr.get());

  CX_RETURN(new SexprNumber(*narg1 - *narg2));

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__mul, SexprEnv* env,
                                             SexprCons const* args)

  SEXPR const& car = DSL::car(args);
  SEXPR const& cadr = DSL::cadr(args);

  // TODO, should support n-arity
  DSL_CONTRACT("add", 1, NUMBER, car);
  DSL_CONTRACT("add", 2, NUMBER, cadr);

  auto narg1 = as<SexprNumber const*>(car.get());
  auto narg2 = as<SexprNumber const*>(cadr.get());

  CX_RETURN(new SexprNumber(*narg1 * *narg2));

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::keyword__gt,  SexprEnv* env,
                                             SexprCons const* args)

  SEXPR const& car = DSL::car(args);
  SEXPR const& cadr = DSL::cadr(args);

  // TODO, should support n-arity (compare with Racket)
  auto arg1 = as<SexprLiteral const*>(car.get());
  auto arg2 = as<SexprLiteral const*>(cadr.get());

  if (!(arg1->IsNUMBER() && arg2->IsNUMBER()))
    CX_ASSERT(false); // TODO, throw contract exception

  auto narg1 = as<SexprNumber const*>(arg1);
  auto narg2 = as<SexprNumber const*>(arg2);

  if (*narg1 > *narg2)
    CX_RETURN(SexprBool::TRUE);

  CX_RETURN(SexprBool::FALSE);

CX_ENDMETHOD


