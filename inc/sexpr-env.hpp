// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef DSLANG_SEXPR_ENV_HPP
#define DSLANG_SEXPR_ENV_HPP

#include "sexpr.hpp"
#include "sexpr-callable-foreign.hpp"

#include <string>
#include <unordered_map>


namespace DSL
{

  namespace detail
  {
    class SexprEnv;
  }

  using SexprENV = CX::IntrusivePtr<detail::SexprEnv>;

  namespace detail
  {

  class SexprEnv CX_FINAL : public Sexpr
  {
  public:
    SexprEnv(Context* sc);
    SexprEnv(Context* sc, SexprEnv const* prev);
    virtual ~SexprEnv();  // TODO, why can't be private?

    void Mark() const override
    {
      Sexpr::Mark();
      for (auto& item : bindings_)
        item.second->Mark();
    }


    typedef std::unordered_map<std::string, SEXPR> BindingsMap;
    typedef BindingsMap::const_iterator const_iterator;
    typedef BindingsMap::iterator iterator;
    iterator begin()                { return bindings_.begin();   }
    iterator end()                  { return bindings_.end();     }
    const_iterator cbegin() const   { return bindings_.cbegin();  }
    const_iterator cend() const     { return bindings_.cend();    }
    size_t size() const             { return bindings_.size();    }
    bool empty() const              { return bindings_.empty();   }
    void erase(iterator pos)        { bindings_.erase(pos);       }
    void clear()
    {
      if (prev_)
        prev_->clear();

      bindings_.clear();
    }

    std::string Write() const;
    Context* context() const { return sc_; };

  public:
    Sexpr const* GetInternal(std::string const& name) const;

    SexprForeign const*  GetForeign(const std::string& fname) const;
    void AddForeign(const std::string& fname, ForeignFunc ffunc);

    Sexpr const* GetVariable(std::string const& ident) const;
    void SetVariable(std::string const& ident, Sexpr const* value);

  protected:
    Type type() const override { return Sexpr::Type::ENV; }

  private:
    SexprENV prev_;
    BindingsMap bindings_;
    typedef std::unordered_map<std::string, SexprFOREIGN> FFuncMap;
    FFuncMap ffmap_;
  };

  } // detail

} // DSL


#endif // DSLANG_SEXPR_ENV_HPP

