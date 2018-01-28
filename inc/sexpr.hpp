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

// NOTE: these don't belong in the list above, because they are
// categories, not leaf types.  They are detected with...
//   * LITERAL  : sexpr->IsLiteralType()
//   * CALLABLE : sexpr->IsCallableType()


namespace DSL
{

class Scheme;

namespace detail
{
  class Context;
  class Sexpr;
  class SexprVoid;
  class SexprEnv;
  class SexprCons;
  class SexprBool;
  class SexprIdent;  // TODO, this is getting silly
  class SexprLiteral;
  class SexprNumber;
  class SexprClosure;
  class SexprKeyword;
  class SexprForeign;
}

////using SexprENV = CX::IntrusivePtr<detail::SexprEnv>;

namespace detail
{
  template <class T>
  Sexpr const* parse(Context*, char const**);

  class Sexpr : public CX::IntrusiveBase
  {
    template <class U>
    friend Sexpr const* parse(Context*, char const**);

    friend class Context;
    friend class DSL::detail::SexprCons;  // for sc_ (and eval(), if not public)
    friend class DSL::detail::SexprClosure;  // for Disowned() & delete
    friend class DSL::detail::SexprForeign;  // for Disowned() & delete

    // TODO, scary?
    //template <typename> friend class CX::IntrusivePtr;  // Disowned()
    //
    friend class CX::IntrusivePtr<Sexpr>;
    friend class CX::IntrusivePtr<SexprVoid>;
    friend class CX::IntrusivePtr<SexprEnv>;  // TODO this is getting silly
    friend class CX::IntrusivePtr<SexprIdent>;  // TODO this is getting silly (Disowned)
    friend class CX::IntrusivePtr<SexprLiteral>;  // TODO this is getting silly (Adopted/Disowned)
    friend class CX::IntrusivePtr<SexprNumber>;  // TODO this is getting silly (Adopted/Disowned)
    friend class CX::IntrusivePtr<SexprClosure>;  // TODO this is getting silly (Adopted/Disowned)
    friend class CX::IntrusivePtr<SexprKeyword>;  // TODO this is getting silly (Adopted/Disowned)
    friend class CX::IntrusivePtr<SexprForeign>;  // TODO this is getting silly (Adopted/Disowned)
    friend class Context;

  protected:
    Sexpr(Context* sc=nullptr);  // see implementation

  public:
    virtual void Mark() const  { marked_ = true;   }
    void ClearMark()    const  { marked_ = false;  }
    bool IsMarked()     const  { return marked_;   }

    virtual std::string Print() const;
    virtual std::string Display() const;
    virtual std::string Write() const = 0;

  protected:
    virtual ~Sexpr();

  protected:
    void Adopted() const override;
    bool Disowned() const override;

  protected:
    virtual Sexpr const* transmute(char const** input) const;

  public:
    // needs to be public for various keywords
    virtual Sexpr const* eval(SexprEnv const* env) const;

  protected:
    mutable Context* sc_;  // TODO, not to be const because of need to call addSymbol()
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

