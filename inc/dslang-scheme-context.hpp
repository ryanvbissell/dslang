// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SCHEME_CONTEXT_HPP
#define DSLANG_SCHEME_CONTEXT_HPP

#include "dslang-scheme.hpp"
#include "sexpr-env.hpp"
#include "sexpr-void.hpp"
#include "sexpr-literal-bool.hpp"
#include "sexpr-cons.hpp"
#include "sexpr-ident.hpp"
#include "sexpr-callable-keyword.hpp"

#include <string>
#include <mutex>
#include <unordered_set>
#include <unordered_map>

namespace DSL
{

#define DSL_FOREIGN(thys,func)                                      \
  [=](SexprENV const* e, std::string const& c, SexprCONS const* a)  \
  {                                                                 \
    return thys->func(e,c,a);                                       \
  }

class Dialect;

namespace detail {

  class Context: public Scheme
  {
  public:
    Context();
    virtual ~Context();

    void Register(Sexpr const* that);
    void Deregister(Sexpr const* that);

    Sexpr const* read(char const** input) final;
    virtual Dialect const& dialect() const = 0;

    void gc();
    SexprEnv const* env() const override { return env_.get(); }

    void AddSymbol(SexprIdent const* ident);
    SexprIdent const* GetSymbol(std::string const& ident) const;

    SexprKeyword const* GetKeyword(std::string const& ident) const;

    void report();  // TODO, this is temporary test code

  protected:
    typedef std::unordered_multimap<U32, SexprIDENT const> SymbolMMap;
    typedef std::unordered_map<std::string, SexprKEYWORD> KeywordMap;

  protected:
    mutable std::unordered_set<Sexpr const*> registry_;
    SexprCONS nil_;
    SexprVOID void_;
    SexprBOOL true_, false_;
    KeywordMap keywords_;
    mutable SexprENV env_;
    mutable SymbolMMap symbols_;
    void addKeyword(std::string const& name, CallType type, Keyword handler);
    U32 hashSymbolName(std::string const& name) const;

  private:
    mutable std::mutex symlock_;
    void gc_symbols();
    SexprIdent const* getSymbol(std::string const& name, U32* hash) const;
  };

} // namespace detail

} // namespace DSL

#endif  // DSLANG_SCHEME_CONTEXT_HPP


