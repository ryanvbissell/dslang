// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SCHEME_HPP
#define DSLANG_SCHEME_HPP

#include "sexpr.hpp"

#include <string>
#include <typeinfo>

namespace DSL
{
  class Scheme
  {
  public:
    Scheme();
    virtual ~Scheme();

    SEXPR Read(char const** input);
    SEXPR Eval(SEXPR const& sexpr) const;
    std::string Print(SEXPR const& sexpr) const;
    std::string ReadEvalPrint(char const** input);

  protected:
    virtual DSL::detail::SexprEnv const* env() const = 0;

  private:
    virtual DSL::detail::Sexpr const* read(char const** input) = 0;
    DSL::detail::Sexpr const* eval(DSL::detail::Sexpr const* sexpr) const;
    std::string print(DSL::detail::Sexpr const* sexpr) const;
  };

} // namespace DSL


#endif  // DSLANG_SCHEME_HPP


