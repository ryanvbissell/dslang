// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-cons.hpp"

#include "dslang-exception.hpp"
#include "dslang-dialect.hpp"
#include "dslang-lexer.hpp"
#include "dslang-parser.hpp"
using DSL::SEXPR;
using DSL::SexprCONS;
using namespace DSL::detail;

#include <string>
#include <sstream>
using std::string;
using std::stringstream;


// ---------------------------------------------------------------------


SexprCons const* DSL::detail::SexprCons::NIL(nullptr);


// ---------------------------------------------------------------------


CX_FUNCTION(bool DSL::detail::SexprCons__Match,
                                            char const* text)

  // note that we match both open- and close-parens here.
  // the point of this matcher is to just recognize that
  // we are dealing with a cons, be its beginning or its end.

  // TODO: support multiple paren flavors
  CX_RETURN(('(' == *text) || (')' == *text));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr::Type DSL::detail::SexprCons__Skip,
                                                char const** input)

  CX_ASSERT(input);
  CX_ASSERT(*input);
  CX_ASSERT(SexprCons__Match(*input));

  // TODO: support multiple paren flavors

  // need to find the matching ')'
  int nomatch = 0;  // we'll consume the first '(' inside the do..while
  do
  {
    switch (**input)
    {
      case '(': nomatch++; break;
      case ')': nomatch--; break;
    }

    (*input)++;

  } while (nomatch && **input);

  if (nomatch || (*((*input)-1) != ')'))
    CX_THROW( DSL::Exception,
              DSL::Error::SYNTAX,
              "Malformed s-expression (unbalanced parens" );

  CX_RETURN(Sexpr::Type::CONS);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::SexprCons__Parse, Context* sc,
                                                 char const** input)

  // if we get called to parse a cons, but we are actually at
  // the end of a cons, signal that by returning nullptr.
  if (**input == ')')
    CX_RETURN(nullptr);

  ++(*input);

  // detecting NIL early will help avoid constructing
  // some temporary memoized doppelgangers of NIL
  DSL::lexer::skip_any_whitespace(input);
  if (**input == ')')  // TODO: support multiple paren flavors
  {
    ++(*input);
    CX_RETURN(SexprCons::NIL);
  }

  CX_RETURN(DSL::detail::parse<SexprCons>(sc,input));

CX_ENDFUNCTION


// ---------------------------------------------------------------------


CX_CONSTRUCTOR(DSL::detail::SexprCons::SexprCons, :Sexpr(sc),
                                                           Context* sc)

  CX_ASSERT(!SexprCons::NIL);
  CX_TOPICOUT(dslang:sexpr, "I am the One True NIL\n");

CX_ENDMETHOD



CX_CONSTRUCTOR3(DSL::detail::SexprCons::SexprCons,
                                          : Sexpr(sc),
                                            car_(nullptr),
                                            cdr_(nullptr),
                                                Context* sc,
                                                const char** input)

  // scanner needs to have already advanced past open-paren
  car_ = sc_->read(input);
  if (!car_)
  {
    CX_ASSERT(**input == ')');
    ++(*input);
    memo_ = NIL; // doppelganger, to be replaced during transmute()
    CX_TOPICOUT(dslang:sexpr, "sexpr #%d: I am a doppelganger of NIL\n",
                              serial_);
  }
  else
  {
    bool pair = false;
    const char* probe = *input;

    if (lexer::is_whitespace(*probe))
    {
      // a dot will help us distinguish between a list and a pair
      lexer::skip_any_whitespace(&probe);
      if (*probe == '.')
      {
        ++probe;
        if (lexer::is_whitespace(*probe))
        {
          pair = true;
          *input = probe;
        }
      }
    }

    if (pair)
    {
      cdr_ = sc_->read(input);
      if (!cdr_)
      {
        // for a pair, this is always a bad thing.
        CX_ASSERT(false) // TODO, need exception
      }

      lexer::skip_any_whitespace(input);

      // we had better be at the end of a cons
      CX_ASSERT(**input == ')');  // TODO, need exception?
      ++(*input);
    }
    else
      cdr_ = DSL::detail::parse<SexprCons>(sc_, input);
  }

  if (cdr_) cdr_->Adopted();
  if (car_) car_->Adopted();

CX_ENDMETHOD



CX_CONSTRUCTOR3(DSL::detail::SexprCons::SexprCons,
                                  : Sexpr(sc),
                                    car_(car), cdr_(cdr),
                                            Context* sc,
                                            Sexpr const* car,
                                            Sexpr const* cdr)

  car_->Adopted();
  cdr_->Adopted();

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::detail::SexprCons::~SexprCons)

  if (this == NIL)
  {
    CX_TOPICOUT(dslang:cons, "Destructing canonical NIL\n");
    NIL = nullptr;
  }
  else
  {
    CX_TOPICOUT(dslang:cons,  "Destructing CONS '%s'\n",
                              is_nil(memo_)
                                  ? "doppelganger of NIL"
                                  : Write().c_str());
  }

  if (cdr_ && cdr_->Disowned())
    delete cdr_;

  if (car_ && car_->Disowned())
    delete car_;

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprCons::transmute,
                                                    const char** input)

  // special case: transmutation of memoized NIL to direct NIL
  if (is_nil(memo_))
    CX_RETURN(NIL);

  CX_RETURN(this);

CX_ENDMETHOD



CX_METHOD(SexprCons const* DSL::detail::SexprCons::EvalArgs,
                                                  SexprEnv const* env,
                                                  SexprCons const* args)

  // TODO: move this out of SexprCons?  Possibly to Scheme?

  // recursively zip along 'args', eval'ing every item therein
  if (is_nilp(args))
    CX_RETURN(NIL);

  Sexpr const* rest = EvalArgs(env, as<SexprCons const*>(args->cdr()));
  Sexpr const* first = args->car()->eval(env);

  // XXX we use env->context() instead of sc_ because this function
  // is likely to become a static member function.
  SexprCons const* result = new SexprCons(env->sc_, first, rest);
  CX_RETURN(result);

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::SexprCons::eval, SexprEnv const* env)

  // normally a cons should never have a memoized result,
  // since it may not return the same value upon every call.
  //
  // an exception might be if we are doing lazy evaluation

  if (memo_)
    CX_RETURN(memo_.get());

  if (is_nilp(this))
    CX_RETURN(NIL);

  CX_ASSERT(car_ && cdr_);

  Sexpr const* car = car_->eval(env);
  if (!car->IsCallableType())
  {
    string const& name = car_->Write();
    CX_THROW( DSL::Exception,
              DSL::Error::SYNTAX,
              "Invalid attempt to invoke '%s' as a callable object",
              name.c_str());
  }

  auto callable = as<SexprCallable const*>(car);
  auto cdr = as<SexprCons const*>(cdr_);
  SexprCONS temp(callable->IsSpecial()
                  ? cdr
                  : EvalArgs(env, cdr));

  CX_RETURN(callable->Call(env, temp.get()));

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::SexprCons::Write)

  if (is_nilp(this))
    CX_RETURN("()");

  if (memo_)
    CX_RETURN(memo_->Write());

  stringstream ss;

  ss << "(" << car_->Write();

  // using raw pointers in this loop allows us to
  // avoid creation of temporaries
  Sexpr const* cdr = cdr_;
  while (cdr && !is_nilp(cdr))
  {
    if (cdr->IsCONS())
    {
      SexprCons const* cons = static_cast<SexprCons const*>(cdr);
      ss << " " << cons->car_->Write();
      cdr = cons->cdr_;
    }
    else
    {
      ss << " . " << cdr->Write();
      cdr = nullptr;
    }
  }

  ss << ")";

  CX_RETURN(ss.str());

CX_ENDMETHOD



CX_CONSTMETHOD(U32 DSL::detail::SexprCons::Length)

  if (memo_.get() == NIL)
    CX_RETURN(0);

  CX_ASSERT(!memo_); // TODO, ??? might not be a cons

  U32 count= car_ ? 1 : 0;
  Sexpr const* cdr = cdr_;
  while (cdr && cdr->IsCONS() && !is_nilp(cdr))
  {
    cdr = as<SexprCons const*>(cdr)->cdr();
    count += 1;  // counting the new car
  }

  CX_RETURN(count);

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::car, SexprCons const* cons)

  CX_RETURN(cons->car());

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::cdr, SexprCons const* cons)

  CX_RETURN(cons->cdr());

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::caar, SexprCons const* cons)

  CX_RETURN(car(as<SexprCons const*>(car(cons))));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::cadr, SexprCons const* cons)

  CX_RETURN(car(as<SexprCons const*>(cdr(cons))));

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::cdar, SexprCons const* cons)

  CX_RETURN(cdr(as<SexprCons const*>(car(cons))));

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::cddr, SexprCons const* cons)

  CX_RETURN(cdr(as<SexprCons const*>(cdr(cons))));

CX_ENDMETHOD



CX_FUNCTION(Sexpr const* DSL::detail::caddr, SexprCons const* cons)

  CX_RETURN(car(as<SexprCons const*>(cddr(cons))));

CX_ENDMETHOD



CX_FUNCTION(bool DSL::detail::is_nilp, Sexpr const* sexpr)

  CX_RETURN(sexpr == SexprCons::NIL);

CX_ENDFUNCTION



CX_FUNCTION(SEXPR DSL::car, SexprCONS const& cons)

  CX_RETURN(cons->car());

CX_ENDFUNCTION



CX_FUNCTION(SEXPR DSL::cdr, SexprCONS const& cons)

  CX_RETURN(cons->cdr());

CX_ENDFUNCTION



CX_FUNCTION(SEXPR DSL::caar, SexprCONS const& cons)

  CX_RETURN(as<SexprCons const*>(car(cons).get())->car());

CX_ENDFUNCTION



CX_FUNCTION(SEXPR DSL::cadr, SexprCONS const& cons)

  CX_RETURN(as<SexprCons const*>(cdr(cons).get())->car());

CX_ENDFUNCTION



CX_FUNCTION(SEXPR DSL::cdar, SexprCONS const& cons)

  CX_RETURN(as<SexprCons const*>(car(cons).get())->cdr());

CX_ENDMETHOD



CX_FUNCTION(SEXPR DSL::cddr, SexprCONS const& cons)

  CX_RETURN(as<SexprCons const*>(cdr(cons).get())->cdr());

CX_ENDMETHOD



CX_FUNCTION(SEXPR DSL::caddr, SexprCONS const& cons)

  CX_RETURN(as<SexprCons const*>(cddr(cons).get())->car());

CX_ENDMETHOD



CX_FUNCTION(bool DSL::is_nil, SEXPR const& sexpr)

  CX_RETURN(sexpr.get() == SexprCons::NIL);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::is_nil, SexprCONS const& cons)

  CX_RETURN(cons.get() == SexprCons::NIL);

CX_ENDFUNCTION


