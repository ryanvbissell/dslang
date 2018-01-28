// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_DIALECT_HPP
#define DSLANG_DIALECT_HPP

#include "dslang-scheme-context.hpp"

#include <deque>
#include <utility>

namespace DSL
{
  typedef bool (*TokenMatcher)(char const*);
  typedef DSL::detail::Sexpr::Type (*TokenSkipper)(char const**);
  typedef DSL::detail::Sexpr const* (*TokenParser)(DSL::detail::Context*, char const**);

  struct TokenHandler
  {
    TokenSkipper skip;
    TokenParser parse;
  };

  class Dialect
  {

  public:
    Dialect();
    virtual ~Dialect();

    Scheme* scheme() const { return sc_; };
    DSL::detail::Context* context() const { return sc_; }
    void SetContext(Scheme* sc);  // TODO, private w/ friend access?

    TokenHandler const* GetTokenHandler(char const* text) const;

#if 0
    virtual bool IsCustomToken(char const * const text) const;
    virtual const SexprCUSTOM* ParseCustom(const std::string& lexeme) const;
#endif

  protected:
    DSL::detail::Context* sc_;

    std::deque<std::pair<TokenMatcher,TokenHandler const*> > tokenizers_;
    void registerTokenizer(TokenMatcher,TokenSkipper,TokenParser);
};


} // namespace DSL


#endif // DSLANG_DIALECT_HPP

