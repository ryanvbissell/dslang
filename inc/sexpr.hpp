// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_HPP
#define DSLANG_SEXPR_HPP

#include "cx-intrusive.hpp"
#include "cx-hackery.hpp" // CX_FINAL

#include <string>


#define SEXPRTYPES  \
  X(VOID)           \
  X(ENV)            \
  X(CONS)           \
  X(QUOTE)          \
  X(BOOL)           \
  X(STRING)         \
  X(NUMBER)         \
  X(KEYWORD)        \
  X(IDENT)          \
  X(CLOSURE)        \
  X(FOREIGN)        \
  X(CUSTOM)

// NOTE: the following don't belong in the list above, because they
// are categories, not leaf types.  They are detected with...
//   * LITERAL  : sexpr->IsLiteralType()
//   * CALLABLE : sexpr->IsCallableType()


namespace DSL
{

namespace detail
{
  class Context;
  class Sexpr;
  class SexprEnv;
  class SexprCons;
  class SexprClosure;

  class Sexpr : public CX::IntrusiveBase
  {
    template <class U>
    friend Sexpr const* parse(Context*, char const**);

    friend class DSL::detail::SexprCons;  // for sc_

  protected:
    Sexpr(Context* sc=nullptr);  // see implementation

  public:
    virtual void Mark() const  { marked_ = true;   }
    void ClearMark()    const  { marked_ = false;  }
    bool IsMarked()     const  { return marked_;   }
    void Adopted() const override;
    bool Disowned() const override;

    virtual std::string Print() const;
    virtual std::string Display() const;
    virtual std::string Write() const = 0;

    virtual ~Sexpr();

  protected:
    virtual Sexpr const* transmute(char const** input) const;

  public:
    // TODO, not a fan of this being public, because then it can
    // be called through SEXPR::operator->()
    // but unfortunately some keyword handlers need it
    virtual Sexpr const* eval(SexprEnv const* env) const;

  protected:
    mutable Context* sc_;
    mutable bool marked_;
    U32 serial_;
    static U32 extant_;
    static U32 numsexprs_;

  public:  // TODO, should be protected?
    #define X(TYPE) TYPE,
    enum class Type { SEXPRTYPES };
    #undef X
    // these are needed because they are categories, not leaf types
    virtual bool IsLiteralType() const { return false; }
    virtual bool IsCallableType() const { return false; }

#define X(TYPE) bool Is##TYPE() const { return (type() == Type::TYPE); }
    SEXPRTYPES
#undef X
  protected:
    virtual Type type() const = 0;

  };

  template <class T>
  T as(Sexpr const* sexpr)
  {
#ifdef DSL_OPT_FASTCAST
    return static_cast<T>(sexpr);  // 'time ./out/bench' is 30% faster
#else
    return dynamic_cast<T>(sexpr);
#endif
  }

} // detail

using SEXPR = CX::IntrusivePtr<detail::Sexpr>;

template <class T>
T as(SEXPR const& sexpr)
{
  T const& temp = T(sexpr);
  return temp;
}

} // DSL


#endif // DSLANG_SEXPR_HPP

