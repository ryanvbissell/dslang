// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "sexpr-literal-number.hpp"

#include "dslang-exception.hpp"
#include "dslang-dialect.hpp"
#include "dslang-lexer.hpp"
#include "dslang-parser.hpp"
using DSL::SexprNUMBER;
using namespace DSL::detail;

#include <cmath>
#include <tuple>
#include <string>
using std::string;
#include <sstream>
#include <iomanip>


// ---------------------------------------------------------------------


#define cmp(a,b) ((((uint32_t)(a)) << 16) | ((uint32_t)(b)))

static
CX_FUNCTION(int decode_number_prefix, DSL::Dialect const& dialect,
                                      NumType* type,
                                      char const** input)

  *type = NumType::UNKNOWN;
  int radix = 10;

  if (**input == '#')
  {
    ++(*input);
    switch (tolower(**input))
    {
      case 'e':  *type = NumType::FIXNUM; break;
      case 'i':  *type = NumType::FLONUM; break;

      case 'd':  break;
      case 'b':  radix = 2; break;
      case 'o':  radix = 8; break;
      case 'x':  radix = 16; break;

      default:
        CX_ASSERT(false);  // TODO, need exception
    }

    ++(*input);
  }

  CX_RETURN(radix);

CX_ENDFUNCTION


static
CX_FUNCTION(U64 radix_to_fraction,  char const** input,
                                    int radix,
                                    S64* numer)

  U64 denom=1;

  char cdigit;
  if (input && *input)
    while (    (cdigit = tolower(*(++(*input))))
            && !DSL::lexer::is_delim(cdigit)
            && (cdigit != '+')
            && (cdigit != 'i') )
    {
      int digit = 0;
      switch (radix)
      {
        case 2:
        case 8:
        case 10:
          if ((cdigit < '0') || (cdigit > '0'+radix-1))
            CX_ASSERT(false); // TODO, throw
          digit = cdigit - '0';
          break;

        case 16:
          if ((cdigit >= '0') || (cdigit <= '9'))
            digit = cdigit - '0';
          else if ((cdigit >= 'a') || (cdigit <= 'f'))
            digit = cdigit - 'a' + 10;
          else
            CX_ASSERT(false); // TODO, throw
          break;

        default:
          CX_ASSERT(false);  // TODO, invalid base, throw?
      }

      denom *= 10;
      *numer *= 10;
      *numer += digit;
    }

  CX_RETURN(denom);

CX_ENDFUNCTION


static
CX_FUNCTION(bool is_integer_delim, char const* text)

  switch (*text)
  {
    case '.':
    case '+':
    case '-':
    case '/':
      CX_RETURN(true);

    case 'i':
      CX_RETURN(DSL::lexer::is_delim(*(text+1)));
  }

  CX_RETURN(DSL::lexer::is_delim(*text));

CX_ENDFUNCTION


// this will get used by 'inexact->exact'
#if 0
static
CX_FUNCTION(std::tuple<S64,U64> double_to_rational, double dvalue)

  double remainder;
  U64 den = 1;
  S64 num = modf(dvalue, &remainder);

  while (remainder)
  {
    remainder *= 10;
    num *= 10;
    num += modf(remainder, &remainder);
    den *= 10;
  }

  CX_RETURN(std::make_tuple(num,den));

CX_ENDMETHOD
#endif


CX_CONSTRUCTOR3(DSL::detail::NumValue::NumValue, :type(NumType::FIXNUM),
                                                  fix{0,1})

CX_ENDMETHOD



CX_CONSTRUCTOR(DSL::detail::NumValue::NumValue,
                                      : type(NumType::FIXNUM),
                                                  S64 numer, U64 denom)

  fix.num = numer;
  fix.den = denom;

CX_ENDMETHOD



CX_CONSTRUCTOR2(DSL::detail::NumValue::NumValue, :type(NumType::FLONUM),
                                                  flo(flonum),
                                                         double flonum)

CX_ENDMETHOD



CX_CONSTRUCTOR0(DSL::detail::NumValue::NumValue,
                                            DSL::Dialect const& dialect,
                                            int radix,
                                            NumType* reqtype,
                                            char const** input)

  // caller has passed in the hinted 'reqtype', but that...
  //     * declares requested internal storage type
  //     * could be 'UNKNOWN'
  //     * could be at odds with 'input' string representation
  // so we'll begin parsing as though input is a FIXNUM,
  // and adjust accordingly
  type = NumType::FIXNUM;

  char const* begin = *input;
  char* end;
  errno = 0;
  fix.den = 1;
  fix.num = strtol(*input, &end, radix);
  if (errno)
    CX_ASSERT(false); // TODO, need exception
  if (*input == end)
  {
    lexer::skip_to_delim(input);
    CX_THROW( DSL::Exception,
              DSL::Error::SYNTAX,
              "Numerical digit (base %lu) expected",
              radix );
  }
  if (!is_integer_delim(end))
  {
    lexer::skip_to_delim(input);
    string bad = lexer::duplicate_text(begin, *input);
    CX_THROW( DSL::Exception,
              DSL::Error::SYNTAX,
              "Invalid base %lu number encountered: '%s'",
              radix, bad.c_str() );
  }

  *input = end;
  switch (**input)
  {
    case '/': // indicates fractional notation
      ++(*input);
      errno = 0;
      fix.den = strtoul(*input, &end, radix);
      if (errno)
        CX_ASSERT(false); // TODO, need exception
      *input = end;
      if (*reqtype == NumType::FLONUM)
        MakeInexact();
      break;

    case '.': // indicates radix-point notation
      fix.den = radix_to_fraction(input, radix, &fix.num);
      if (*reqtype != NumType::FIXNUM)
        MakeInexact();
      break;
  }

  if (*reqtype == NumType::UNKNOWN)
    *reqtype = type;

  if ((*reqtype == NumType::FLONUM) && (type == NumType::FIXNUM))
    MakeInexact();

  if (*reqtype == NumType::FIXNUM)
    reduce();

CX_ENDMETHOD



CX_METHOD(void DSL::detail::NumValue::MakeExact)

  CX_ASSERT(false);

CX_ENDMETHOD



CX_METHOD(void DSL::detail::NumValue::MakeInexact)

  if (type == NumType::FIXNUM)
  {
    flo = 1.0*fix.num / fix.den;
    type = NumType::FLONUM;
  }

CX_ENDMETHOD



// TODO, remove me
static
U64 dsl_gcd(U64 a, U64 b)
{
      return b == 0 ? a : dsl_gcd(b, a % b);
}



CX_CONSTMETHOD(NumValue DSL::detail::NumValue::sqrt)

  if (type == NumType::FIXNUM)
  {
    if (fix.den == 1)
      CX_RETURN(std::sqrt(fix.num));

    double val = 1.0*fix.num / fix.den;
    CX_RETURN(std::sqrt(val));
  }

  CX_RETURN(std::sqrt(flo));

CX_ENDMETHOD



CX_METHOD(void DSL::detail::NumValue::reduce)

  if (type == NumType::FIXNUM)
  {
    // TODO, std::gcd() not available before C++17?
    U64 gcd = dsl_gcd((U64)std::llabs(fix.num), fix.den);
    if (gcd && (gcd > 1))
    {
      fix.num /= gcd;
      fix.den /= gcd;
    }
  }

CX_ENDMETHOD



CX_CONSTMETHOD(int DSL::detail::NumValue::compare, NumValue const& rhs)

  // compares this value with 'rhs' returning...
  //  * negative value if 'this' is smaller
  //  * zero if 'this' is equal
  //  * positive value if 'this' is larger

  // easy cases
  switch (cmp(type,rhs.type))
  {
    case cmp(NumType::FLONUM,NumType::FLONUM):
      CX_RETURN((int)(flo - rhs.flo));

    case cmp(NumType::FLONUM,NumType::FIXNUM):
      CX_RETURN((int)(flo - (1.0*rhs.fix.num / rhs.fix.den)));

    case cmp(NumType::FIXNUM,NumType::FLONUM):
      CX_RETURN((int)((1.0*fix.num / fix.den) - rhs.flo));
  }

  // FIXNUM vs FIXNUM is a bit more complicated

  // if just one number is negative, it's the smallest
  if ((fix.num < 0) != (rhs.fix.num < 0))
    CX_RETURN(fix.num - rhs.fix.num);

    // if denominators are the same, smallest numerator is smallest #
  if (fix.den == rhs.fix.den)
    CX_RETURN(fix.num - rhs.fix.num);

  // and finally, we have "same sign, different nums and dens"
  // TODO for now we'll use cross-multiply, which may risk errors
  // due to imprecision (if intermediate values exceed 2^53, IIRC)
  double expect = 1.0*(fix.num * rhs.fix.den) / fix.den;
  CX_RETURN((int)(expect - rhs.fix.num));

CX_ENDMETHOD



CX_METHOD(NumValue& DSL::detail::NumValue::operator +=,
                                                    NumValue const& rhs)

  // easy cases
  switch (cmp(type,rhs.type))
  {
    case cmp(NumType::FLONUM,NumType::FLONUM):
      flo += rhs.flo;
      CX_RETURNREF(*this);

    case cmp(NumType::FLONUM,NumType::FIXNUM):
      flo += (1.0*rhs.fix.num / rhs.fix.den);
      CX_RETURNREF(*this);

    case cmp(NumType::FIXNUM,NumType::FLONUM):
      type = NumType::FLONUM;
      flo = (1.0*fix.num / fix.den) + rhs.flo;
      CX_RETURNREF(*this);
  }

  // FIXNUM += FIXNUM
  if (!fix.num)
    *this = rhs;
  else if (rhs.fix.num)
  {
    S64 rhsnum = rhs.fix.num;
    if (fix.den != rhs.fix.den)
    {
      // make denominators equal
      rhsnum *= fix.den;
      fix.den *= rhs.fix.den;
      fix.num *= rhs.fix.den;
    }

    fix.num += rhsnum;

    reduce();
  }

CX_ENDMETHOD



CX_METHOD(NumValue& DSL::detail::NumValue::operator -=,
                                                    NumValue const& rhs)

  // easy cases
  switch (cmp(type,rhs.type))
  {
    case cmp(NumType::FLONUM,NumType::FLONUM):
      flo -= rhs.flo;
      CX_RETURNREF(*this);

    case cmp(NumType::FLONUM,NumType::FIXNUM):
      flo -= (1.0*rhs.fix.num / rhs.fix.den);
      CX_RETURNREF(*this);

    case cmp(NumType::FIXNUM,NumType::FLONUM):
      type = NumType::FLONUM;
      flo = (1.0*fix.num / fix.den) - rhs.flo;
      CX_RETURNREF(*this);
  }

  // FIXNUM -= FIXNUM
  S64 rhsnum = rhs.fix.num;
  if (rhsnum)
  {
    if (fix.den != rhs.fix.den)
    {
      // make denominators equal
      rhsnum *= fix.den;
      fix.den *= rhs.fix.den;
      fix.num *= rhs.fix.den;
    }

    fix.num -= rhsnum;

    reduce();
  }

CX_ENDMETHOD



CX_METHOD(NumValue& DSL::detail::NumValue::operator *=,
                                                    NumValue const& rhs)

  if (IsZero())
    CX_RETURNREF(*this);

  if (rhs.IsZero())
  {
    type = NumType::FIXNUM;
    fix.num = 0;
    CX_RETURNREF(*this);
  }

  switch (cmp(type,rhs.type))
  {
    case cmp(NumType::FLONUM,NumType::FLONUM):
      flo *= rhs.flo;
      break;

    case cmp(NumType::FLONUM,NumType::FIXNUM):
      flo *= (1.0*rhs.fix.num / rhs.fix.den);
      break;

    case cmp(NumType::FIXNUM,NumType::FLONUM):
      type = NumType::FLONUM;
      flo = (1.0*fix.num / fix.den) * rhs.flo;
      break;

    case cmp(NumType::FIXNUM,NumType::FIXNUM):
      fix.num *= rhs.fix.num;
      fix.den *= rhs.fix.den;
      break;

    default:
      CX_ASSERT(false);
      break;
  }

  CX_RETURNREF(*this);

CX_ENDMETHOD



CX_METHOD(NumValue& DSL::detail::NumValue::operator /=,
                                                    NumValue const& rhs)

  if (IsZero())
    CX_RETURNREF(*this);

  if (rhs.IsZero())
    CX_ASSERT(false); // TODO, throw exception

  // easy cases
  switch (cmp(type,rhs.type))
  {
    case cmp(NumType::FLONUM,NumType::FLONUM):
      flo /= rhs.flo;
      break;

    case cmp(NumType::FLONUM,NumType::FIXNUM):
      flo /= (1.0*rhs.fix.num / rhs.fix.den);
      break;

    case cmp(NumType::FIXNUM,NumType::FLONUM):
      type = NumType::FLONUM;
      flo = (1.0*fix.num / fix.den) / rhs.flo;
      break;

    case cmp(NumType::FIXNUM,NumType::FIXNUM):
      // multiplying by reciprocal
      fix.num *= rhs.fix.den;
      fix.den *= rhs.fix.num;
      break;

    default:
      CX_ASSERT(false);
      break;
  }

  CX_RETURNREF(*this);

CX_ENDMETHOD


#if 0
CX_METHOD(NumValue& DSL::detail::NumValue::operator %=,
                                                    NumValue const& rhs)

CX_ENDMETHOD
#endif


CX_FUNCTION(bool DSL::detail::operator <,  NumValue const& lhs,
                                              NumValue const& rhs)

  CX_RETURN(lhs.compare(rhs) < 0);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::detail::operator <=, NumValue const& lhs,
                                              NumValue const& rhs)

  CX_RETURN(lhs.compare(rhs) <= 0);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::detail::operator ==, NumValue const& lhs,
                                              NumValue const& rhs)

  CX_RETURN(lhs.compare(rhs) == 0);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::detail::operator !=, NumValue const& lhs,
                                              NumValue const& rhs)

  CX_RETURN(lhs.compare(rhs) != 0);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::detail::operator >=, NumValue const& lhs,
                                              NumValue const& rhs)

  CX_RETURN(lhs.compare(rhs) >= 0);

CX_ENDFUNCTION



CX_FUNCTION(bool DSL::detail::operator >,  NumValue const& lhs,
                                              NumValue const& rhs)

  CX_RETURN(lhs.compare(rhs) > 0);

CX_ENDFUNCTION



CX_FUNCTION(NumValue const DSL::detail::operator+,  NumValue const& lhs,
                                                    NumValue const& rhs)

  NumValue temp(lhs);
  temp += rhs;
  CX_RETURN(temp);

CX_ENDFUNCTION



CX_FUNCTION(NumValue const DSL::detail::operator-,  NumValue const& lhs,
                                                    NumValue const& rhs)

  NumValue temp(lhs);
  temp -= rhs;
  CX_RETURN(temp);

CX_ENDFUNCTION



CX_FUNCTION(NumValue const DSL::detail::operator*,  NumValue const& lhs,
                                                    NumValue const& rhs)

  NumValue temp(lhs);
  temp *= rhs;
  CX_RETURN(temp);

CX_ENDFUNCTION



CX_FUNCTION(NumValue const DSL::detail::operator/,  NumValue const& lhs,
                                                    NumValue const& rhs)

  NumValue temp(lhs);
  temp /= rhs;
  CX_RETURN(temp);

CX_ENDFUNCTION


// --------------------------------------------------------------------


CX_FUNCTION(bool DSL::detail::SexprNumber__Match, char const* text)

  // the point is to confidently match all numbers
  // as quickly as possible, without full parsing.
  // Basically, return true as soon as it is unambiguous.

  // decimal digit is a solid indicator (and is most likely case)
  if (CX_LIKELY(std::isdigit(*text)))
    CX_RETURN(true);

  // a number-specific prefix is proof enough
  if (CX_UNLIKELY(*text == '#'))
  {
    switch (tolower(text[1]))
    {
      case 'e':
      case 'i':
      case 'b':
      case 'o':
      case 'd':
      case 'x':
        CX_RETURN(true);
        break;
    }
  }

  CX_RETURN(false);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr::Type DSL::detail::SexprNumber__Skip, char const** input)

  if (!SexprNumber__Match(*input))
    CX_ASSERT(false);  // TODO, need exception?

  lexer::skip_to_delim(input);
  CX_RETURN(Sexpr::Type::NUMBER);

CX_ENDFUNCTION



CX_FUNCTION(Sexpr const* DSL::detail::SexprNumber__Parse,
                                          Context* sc,
                                          char const** input)

  CX_RETURN(DSL::detail::parse<SexprNumber>(sc, input));

CX_ENDFUNCTION




CX_CONSTRUCTOR(DSL::detail::SexprNumber::SexprNumber,
                                        : SexprLiteral(sc),
                                                    Context* sc,
                                                    char const** input)

  if (input)
  {
    NumType itype, rtype=NumType::UNKNOWN;
    int radix = decode_number_prefix(sc_->dialect(), &rtype, input);

    r_ = NumValue(sc_->dialect(), radix, &rtype, input);
    if ((**input == '+') || (**input == '-'))
    {
      // i_ must be inexact if r_ is inexact
      itype = (rtype == NumType::FLONUM) ? rtype : NumType::UNKNOWN;
      i_ = NumValue(sc_->dialect(), radix, &itype, input);
      if (tolower(**input) != 'i')
        CX_ASSERT(false); // TODO, need exception
      ++(*input);

      // r_ must be inexact if i_ is inexact
      if ((i_.type == NumType::FLONUM) && (r_.type == NumType::FIXNUM))
        r_.MakeInexact();

      CX_ASSERT(r_.type == i_.type);
    }

    CX_ASSERT(r_.type != NumType::UNKNOWN);

    exact_ = (r_.type == NumType::FIXNUM);
  }

CX_ENDMETHOD



CX_CONSTRUCTOR4(DSL::detail::SexprNumber::SexprNumber,
                                      : SexprLiteral(that.sc_),
                                        exact_(that.exact_),
                                        r_(that.r_), i_(that.i_),
                                               SexprNumber const& that)

CX_ENDMETHOD



CX_CONSTRUCTOR3(DSL::detail::SexprNumber::SexprNumber,
                                        : SexprLiteral(sc),
                                              r_(real), i_(0,1),
                                                  Context* sc,
                                                  NumValue const& real)

CX_ENDMETHOD



CX_CONSTRUCTOR3(DSL::detail::SexprNumber::SexprNumber,
                                        : SexprLiteral(sc),
                                              r_(real), i_(imag),
                                                  Context* sc,
                                                  NumValue const& real,
                                                  NumValue const& imag)

CX_ENDMETHOD



CX_CONSTMETHOD(bool DSL::detail::SexprNumber::IsExact)

  // TODO:  "#e4.1" should be considered exact
  // but that should happen during parsing, as 4.1 will get
  // transmuted to 41/10

  if (r_.type == NumType::FLONUM)
    CX_RETURN(false);

  if (i_.type == NumType::FLONUM)
    CX_RETURN(false);

  CX_RETURN(true);

CX_ENDMETHOD



CX_CONSTMETHOD(SexprNumber const DSL::detail::SexprNumber::Real)

  CX_RETURN(SexprNumber(sc_, r_));

CX_ENDMETHOD



CX_CONSTMETHOD(SexprNumber const DSL::detail::SexprNumber::Imag)

  CX_RETURN(SexprNumber(sc_, i_));

CX_ENDMETHOD



CX_CONSTMETHOD(SexprNumber const DSL::detail::SexprNumber::Conjugate)

  if (IsReal());
    CX_RETURN(*this);

  SexprNumber conjugate = *this;

  switch (i_.type)
  {
    case NumType::FIXNUM:
      conjugate.i_.fix.num *= -1;
      break;

    case NumType::FLONUM:
      conjugate.i_.flo *= -1;
      break;

    default:
      CX_ASSERT(false);
      break;
  }

  CX_RETURN(conjugate);

CX_ENDMETHOD



CX_CONSTMETHOD(bool DSL::detail::SexprNumber::operator <,
                                              SexprNumber const& other)

  // pathological cases: we need to throw a contract violation
  // if either number has a nonzero imaginary component
  if (!IsReal() || !other.IsReal())
    CX_ASSERT(false);  // TODO, throw exception

  CX_RETURN(r_ < other.r_);

CX_ENDMETHOD



CX_CONSTMETHOD(bool DSL::detail::SexprNumber::operator <=,
                                              SexprNumber const& other)

  CX_RETURN(!operator>(other));

CX_ENDMETHOD



CX_CONSTMETHOD(bool DSL::detail::SexprNumber::operator ==,
                                              SexprNumber const& other)

  if (i_ != other.i_)
    CX_RETURN(false); // different imaginary components

  CX_RETURN(r_ == other.r_);

CX_ENDMETHOD



CX_CONSTMETHOD(bool DSL::detail::SexprNumber::operator !=,
                                              SexprNumber const& other)

  CX_RETURN(!operator==(other));

CX_ENDMETHOD



CX_CONSTMETHOD(bool DSL::detail::SexprNumber::operator >=,
                                              SexprNumber const& other)

  CX_RETURN(!operator<(other));

CX_ENDMETHOD



CX_CONSTMETHOD(bool DSL::detail::SexprNumber::operator >,
                                              SexprNumber const& other)

  // pathological cases: we need to throw a contract violation
  // if either number has a nonzero imaginary component
  if (!IsReal() || !other.IsReal())
    CX_ASSERT(false);  // TODO, throw exception

  CX_RETURN(r_ > other.r_);

CX_ENDMETHOD



static
CX_FUNCTION(bool reconcile_exactness, NumValue* r, NumValue* i)

  if ((r->type == NumType::FIXNUM) && (i->type == NumType::FLONUM))
    r->MakeInexact();

  if ((r->type == NumType::FLONUM) && (i->type == NumType::FIXNUM))
    i->MakeInexact();

  CX_RETURN((i->type == NumType::FIXNUM) && (r->type == NumType::FIXNUM));

CX_ENDFUNCTION



CX_METHOD(SexprNumber& DSL::detail::SexprNumber::operator +=,
                                              SexprNumber const& other)

  r_ += other.r_;
  i_ += other.i_;
  exact_ = reconcile_exactness(&r_, &i_);

  CX_RETURNREF(*this);

CX_ENDMETHOD




CX_METHOD(SexprNumber& DSL::detail::SexprNumber::operator -=,
                                              SexprNumber const& other)

  i_ -= other.i_;
  r_ -= other.r_;
  exact_ = reconcile_exactness(&r_, &i_);

  CX_RETURNREF(*this);

CX_ENDMETHOD



CX_METHOD(SexprNumber& DSL::detail::SexprNumber::operator *=,
                                              SexprNumber const& other)

  NumValue first(r_ * other.r_);
  NumValue outer(r_ * other.i_);
  NumValue inner(i_ * other.r_);
  NumValue last(i_ * other.i_);

  r_ = first - last; // aka, + -1*last
  i_ = inner + outer;

  exact_ = reconcile_exactness(&r_, &i_);

  CX_RETURNREF(*this);

CX_ENDMETHOD



CX_METHOD(SexprNumber& DSL::detail::SexprNumber::operator /=,
                                              SexprNumber const& other)

  CX_RETURNREF(operator*=(other.Conjugate()));

CX_ENDMETHOD



#if 0
CX_METHOD(SexprNumber& DSL::detail::SexprNumber::operator %=,
                                              SexprNumber const& other)

CX_ENDMETHOD
#endif



CX_CONSTMETHOD(SexprNumber const DSL::detail::SexprNumber::abs)

  if (IsReal())
    CX_RETURN(SexprNumber(sc_, (r_ >= 0) ? r_ : r_ * -1));

  if (r_ == 0)
    CX_RETURN(SexprNumber(sc_, (i_ >= 0) ? i_ : i_ * -1));

  NumValue leg1 = r_ * r_;
  NumValue leg2 = i_ * i_;
  CX_RETURN(SexprNumber(sc_, (leg1 + leg2).sqrt()));

CX_ENDMETHOD



static
CX_FUNCTION(string write_numvalue, bool exact, NumValue const& num,
                                               bool showpos=false)

  std::stringstream s;

  if (showpos && (num >= 0))
    s << "+";

  if (!exact)
  {
    if (floor(num.flo) == num.flo)  // print at least 1 decimal digit
      s << std::setprecision(1) << std::fixed << num.flo;
    else
      s << std::setprecision(10) << num.flo;
  }
  else
  {
    s << num.fix.num;
    if (num.fix.den != 1)
      s << "/" << num.fix.den;
  }

  CX_RETURN(s.str());

CX_ENDFUNCTION



CX_CONSTMETHOD(string DSL::detail::SexprNumber::Write)

  std::stringstream s;
  s << write_numvalue(exact_, r_);
  if (i_.fix.num)
  {
    char sign = (i_.fix.num < 0) ? '-' : '+';
    s << write_numvalue(exact_, i_, true) << "i";
  }

  CX_RETURN(s.str());

CX_ENDMETHOD


// --------------------------------------------------------------------


CX_FUNCTION(SexprNumber const DSL::detail::min, SexprNumber const& lhs,
                                                SexprNumber const& rhs)

  CX_RETURN(lhs < rhs ? lhs : rhs);

CX_ENDFUNCTION



CX_FUNCTION(SexprNumber const DSL::detail::max, SexprNumber const& lhs,
                                                SexprNumber const& rhs)

  CX_RETURN(lhs > rhs ? lhs : rhs);

CX_ENDFUNCTION



CX_FUNCTION(SexprNumber const DSL::detail::operator +,
                                                SexprNumber const& lhs,
                                                SexprNumber const& rhs)

  SexprNumber temp(lhs);

  temp += rhs;
  CX_RETURN(temp);

CX_ENDFUNCTION



CX_FUNCTION(SexprNumber const DSL::detail::operator -,
                                                SexprNumber const& lhs,
                                                SexprNumber const& rhs)

  SexprNumber temp(lhs);

  temp -= rhs;
  CX_RETURN(temp);

CX_ENDFUNCTION




CX_FUNCTION(SexprNumber const DSL::detail::operator *,
                                                SexprNumber const& lhs,
                                                SexprNumber const& rhs)

  SexprNumber temp(lhs);

  temp *= rhs;
  CX_RETURN(temp);

CX_ENDFUNCTION



CX_FUNCTION(SexprNumber const DSL::detail::operator /,
                                                SexprNumber const& lhs,
                                                SexprNumber const& rhs)

  SexprNumber temp(lhs);

  temp /= rhs;
  CX_RETURN(temp);

CX_ENDFUNCTION


// --------------------------------------------------------------------


#if 0
CX_FUNCTION(SexprNUMBER const DSL::min, SexprNUMBER const& lhs,
                                        SexprNUMBER const& rhs)

  CX_RETURN(*lhs < *rhs ? lhs : rhs);

CX_ENDFUNCTION



CX_FUNCTION(SexprNUMBER const DSL::max, SexprNUMBER const& lhs,
                                        SexprNUMBER const& rhs)

  CX_RETURN(*lhs > *rhs ? lhs : rhs);

CX_ENDFUNCTION



CX_FUNCTION(SexprNUMBER const DSL::operator +,  SexprNUMBER const& lhs,
                                                SexprNUMBER const& rhs)

  SexprNUMBER sum(lhs);
  sum += rhs;
  CX_RETURN(sum);

CX_ENDFUNCTION



CX_FUNCTION(SexprNUMBER const DSL::operator -,  SexprNUMBER const& lhs,
                                                SexprNUMBER const& rhs)

  CX_RETURN(*lhs - *rhs);

CX_ENDFUNCTION




CX_FUNCTION(SexprNUMBER const DSL::operator *,  SexprNUMBER const& lhs,
                                                SexprNUMBER const& rhs)

  CX_RETURN(*lhs * *rhs);

CX_ENDFUNCTION



CX_FUNCTION(SexprNUMBER const DSL::operator /,  SexprNUMBER const& lhs,
                                                SexprNUMBER const& rhs)

  CX_RETURN(*lhs / *rhs);

CX_ENDFUNCTION
#endif


