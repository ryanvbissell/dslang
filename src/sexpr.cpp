// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr.hpp"
#include "dslang-exception.hpp"
#include "dslang-scheme-context.hpp"
using DSL::detail::Sexpr;

#include <string>
using std::string;


// ---------------------------------------------------------------------


U32 Sexpr::extant_=0;
U32 Sexpr::numsexprs_=0;


// ---------------------------------------------------------------------


CX_CONSTRUCTOR4(DSL::detail::Sexpr::Sexpr,
                                      : CX::IntrusiveBase(),
                                        sc_(sc),
                                        marked_(false),
                                        serial_(++numsexprs_),
                                                          Context* sc)

  ++extant_;

  // note that 'sc' is allowed to be <nullptr>.  This is because things
  // like SexprKeyword are really only Sexprs for convenience, and we
  // don't want to register them w/ the GC
  if (sc_)
    sc_->Register(this);

  CX_TOPICOUT(dslang:refcount, "Hello world, I am sexpr #%d\n", serial_);

CX_ENDMETHOD



CX_DESTRUCTOR(DSL::detail::Sexpr::~Sexpr)

  CX_TOPICOUT(dslang:refcount, "Sexpr #%d: refcount @ destruction = %d\n",
                            serial_, (size_t)refcount_);

  if (sc_)
    sc_->Deregister(this);

  // failing assertion indicates that either:
  // - some code called 'delete' directly instead of ->Disowned()
  // - a bug in refcounting
  CX_ASSERT(refcount_ == 0,
            "refcounting error detected @ Sexpr destruction");

  CX_TOPICOUT(dslang:refcount, "sexprs remaining: %lu\n", --extant_);

CX_ENDMETHOD



CX_CONSTMETHOD(void DSL::detail::Sexpr::Adopted)

  CX::IntrusiveBase::Adopted();
  CX_TOPICOUT(dslang:refcount,
              "ADOPTED: Sexpr #%d (%p)'s refcount is now %d\n",
              serial_, this, (size_t)refcount_);

CX_ENDMETHOD



CX_CONSTMETHOD(bool DSL::detail::Sexpr::Disowned)

  if (nascent_)
  {
    CX_ASSERT(refcount_ == 0);
    CX_TOPICOUT(dslang:refcount,
                "DISOWNED: Sexpr #%d (%p) was nascent, now moribund\n",
                serial_, this);
    CX_RETURN(true);
  }

  bool moribund = CX::IntrusiveBase::Disowned();
  CX_TOPICOUT(dslang:refcount,
              "DISOWNED: Sexpr #%d (%p)'s refcount is now %d\n",
              serial_, this, (size_t)refcount_);

  CX_RETURN(moribund);

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::Sexpr::transmute,
                                                    char const** input)

  // most DSL::SexprTYPES do all of their reading during construction.
  // but since there are some exceptions, this method cannot be const.
  CX_RETURN(this);

CX_ENDMETHOD



CX_CONSTMETHOD(Sexpr const* DSL::detail::Sexpr::eval, SexprEnv const* env)

  CX_RETURN(this);

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::Sexpr::Print)

  // for most types, Print() is an alias of Display() + "\n"
  CX_RETURN(Display() + "\n");

CX_ENDMETHOD



CX_CONSTMETHOD(string DSL::detail::Sexpr::Display)

  // by default, Display() is a synonym of Write()
  CX_RETURN(Write());

CX_ENDMETHOD

