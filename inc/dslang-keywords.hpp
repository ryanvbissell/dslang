// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2015-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_KEYWORDS_HPP
#define DSLANG_KEYWORDS_HPP

#define DSL_KEYWORDS                \
  REGULAR (load)                    \
  REGULAR (void)                    \
  REGULAR (not)                     \
  REGULAR2(booleanp, "boolean?")    \
  REGULAR (car)                     \
  REGULAR (cdr)                     \
  REGULAR (caar)                    \
  REGULAR (cadr)                    \
  REGULAR (cdar)                    \
  REGULAR (cddr)                    \
  REGULAR (cons)                    \
  REGULAR (eval)                    \
  REGULAR (list)                    \
  REGULAR2(eq,    "eq?")            \
  REGULAR2(eqv,   "eqv?")           \
  REGULAR2(equalp, "equal?")        \
  SPECIAL (quote)                   \
  SPECIAL (define)                  \
  SPECIAL (lambda)                  \
  REGULAR (write)                   \
  REGULAR (print)                   \
  REGULAR (display)                 \
  REGULAR (newline)                 \
  SPECIAL (if)                      \
  SPECIAL (cond)                    \
  SPECIAL (and)                     \
  SPECIAL (or)                      \
  SPECIAL2(set,   "set!")           \
  REGULAR2(number_eq, "=")          \
  REGULAR2(add,       "+")          \
  REGULAR2(sub,       "-")          \
  REGULAR2(mul,       "*")          \
  REGULAR2(gt,        ">")

// pick up the 'Keyword' function typedef
#include "sexpr-callable-keyword.hpp"

namespace DSL {
namespace detail {

// prototypes for all keyword handler functions
#define SPECIAL REGULAR
#define SPECIAL2 REGULAR2
#define REGULAR(name) REGULAR2(name, #name)
#define REGULAR2(name, str) \
    Keyword keyword__##name;

  DSL_KEYWORDS

#undef REGULAR2
#undef REGULAR
#undef SPECIAL2
#undef SPECIAL

} // namespace detail
} // namespace DSL

#endif // DSLANG_KEYWORDS_HPP

