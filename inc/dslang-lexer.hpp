// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_LEXER_HPP
#define DSLANG_LEXER_HPP

#include <string>

namespace DSL
{

namespace lexer
{
  std::string duplicate_text(char const* start, char const* end);

  bool is_delim(char text);
  bool is_whitespace(char text);
  bool is_comment(char const* text);
  bool is_empty_list(char const* text);

  void skip_past_newline(char const** input);
  void skip_past_comment(char const** input);

  void skip_any_whitespace(char const** input);
  void skip_non_whitespace(char const** input);

  void skip_to_delim(char const** input);
  void skip_past_ident(char const** input);

} // namespace lexer

} // namespace DSL


#endif // DSLANG_LEXER_HPP

