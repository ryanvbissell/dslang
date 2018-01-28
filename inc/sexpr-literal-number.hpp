// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_NUMBER_HPP
#define DSLANG_SEXPR_NUMBER_HPP

#include "sexpr-literal.hpp"

#include <string>

namespace DSL
{

  class Dialect;

namespace detail
{

  enum class NumType { UNKNOWN=0, FIXNUM, FLONUM };
  struct Fixnum { S64 num; U64 den; };
  struct NumValue
  {
    NumType type;
    union
    {
      Fixnum fix;
      double flo;
    };
    NumValue();
    NumValue(DSL::Dialect const& dialect, int radix, NumType* reqtype, char const** input);
    NumValue(S64 numer, U64 denom);
    NumValue(double flonum);
    bool operator()() const { return !IsZero(); }
    NumValue& operator += (NumValue const& rhs);
    NumValue& operator -= (NumValue const& rhs);
    NumValue& operator *= (NumValue const& rhs);
    NumValue& operator /= (NumValue const& rhs);
    //NumValue& operator %= (NumValue const& rhs);
    bool IsZero() const { return (fix.num == 0); }
    bool IsNonZero() const { return (fix.num != 0); }

    void MakeExact();
    void MakeInexact();

    NumValue sqrt() const;

    void reduce();
    int compare(NumValue const& rhs) const;
  };

  bool operator <  (NumValue const& lhs, NumValue const& rhs);
  bool operator <= (NumValue const& lhs, NumValue const& rhs);
  bool operator == (NumValue const& lhs, NumValue const& rhs);
  bool operator != (NumValue const& lhs, NumValue const& rhs);
  bool operator >= (NumValue const& lhs, NumValue const& rhs);
  bool operator >  (NumValue const& lhs, NumValue const& rhs);

  NumValue const operator+(NumValue const& lhs, NumValue const& rhs);
  NumValue const operator-(NumValue const& lhs, NumValue const& rhs);
  NumValue const operator*(NumValue const& lhs, NumValue const& rhs);
  NumValue const operator/(NumValue const& lhs, NumValue const& rhs);
  //NumValue const operator%(NumValue const& lhs, NumValue const& rhs);

  class SexprNumber : public SexprLiteral
  {
  public:
    // doesn't take Context*, because of standalone operators
    SexprNumber(Context* sc, char const** input);
    SexprNumber(SexprNumber const& that);
    // fixnum & flonum constructors?

    // TODO: these need to defer to 'custom_' later
    virtual bool IsReal()     const { return i_.IsZero(); }
    virtual bool IsComplex()  const { return true; } // entire tower is complex
    virtual bool IsRational() const { return true; }
    // TODO: IsRational() needs work (+/- NAN, +/i INF)

    // TODO: needs to defer to 'custom_' later
    // TODO: #e4.0 is exact, 4.0 is inexact.  Need 'exact_' flag.
    virtual bool IsExact() const;

    // machine representation for this SexprNumber (independent of complex/real etc)
    // TODO: needs to defer to 'custom_' later
    virtual bool IsFixnum() const { return exact_; }
    virtual bool IsFlonum() const { return !exact_; }

    // these are members becuase they need access to private stuff
    // but e.g. operator < (double, SexprNumber) could be a non-member
    virtual bool operator <  (SexprNumber const& rhs) const;
    virtual bool operator <= (SexprNumber const& rhs) const;
    virtual bool operator == (SexprNumber const& rhs) const;
    virtual bool operator != (SexprNumber const& rhs) const;
    virtual bool operator >= (SexprNumber const& rhs) const;
    virtual bool operator >  (SexprNumber const& rhs) const;

    virtual SexprNumber const Real() const;
    virtual SexprNumber const Imag() const;
    virtual SexprNumber const Conjugate() const;

    virtual SexprNumber& operator += (SexprNumber const& other);
    virtual SexprNumber& operator -= (SexprNumber const& other);
    virtual SexprNumber& operator *= (SexprNumber const& other);
    virtual SexprNumber& operator /= (SexprNumber const& other);
    //virtual SexprNumber& operator %= (SexprNumber const& other);

    std::string Write() const override;

    virtual SexprNumber const abs() const;
  protected:
    Sexpr::Type type() const override { return Sexpr::Type::NUMBER; }
    int realDifference(SexprNumber const& other) const;

  private:
    bool exact_;
    NumValue r_;
    NumValue i_;
    SexprNumber(Context* sc, NumValue const& real);
    SexprNumber(Context* sc, NumValue const& real, NumValue const& imag);
  };

  SexprNumber const min(SexprNumber const& lhs, SexprNumber const& rhs);
  SexprNumber const max(SexprNumber const& lhs, SexprNumber const& rhs);

  SexprNumber const operator+(SexprNumber const& lhs, SexprNumber const& rhs);
  SexprNumber const operator-(SexprNumber const& lhs, SexprNumber const& rhs);
  SexprNumber const operator*(SexprNumber const& lhs, SexprNumber const& rhs);
  SexprNumber const operator/(SexprNumber const& lhs, SexprNumber const& rhs);
  //SexprNumber const operator%(SexprNumber const& lhs, SexprNumber const& rhs);


  bool SexprNumber__Match(char const* text);
  Sexpr::Type SexprNumber__Skip(char const** input);
  Sexpr const* SexprNumber__Parse(Context* sc, char const** input);

} // namespace detail

using SexprNUMBER = CX::IntrusivePtr<detail::SexprNumber>;

#if 0
SexprNUMBER const min(SexprNUMBER const& lhs, SexprNUMBER const& rhs);
SexprNUMBER const max(SexprNUMBER const& lhs, SexprNUMBER const& rhs);
SexprNUMBER const operator+(SexprNUMBER const& lhs, SexprNUMBER const& rhs);
SexprNUMBER const operator-(SexprNUMBER const& lhs, SexprNUMBER const& rhs);
SexprNUMBER const operator*(SexprNUMBER const& lhs, SexprNUMBER const& rhs);
SexprNUMBER const operator/(SexprNUMBER const& lhs, SexprNUMBER const& rhs);
//SexprNUMBER const operator%(SexprNUMBER const& lhs, SexprNUMBER const& rhs);
#endif

} // namespace DSL


#endif // DSLANG_SEXPR_NUMBER_HPP

