// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "dslang"

#include "dslang-scheme-context.hpp"

#include "dslang-exception.hpp"
#include "dslang-keywords.hpp"
#include "sexpr-void.hpp"
#include "sexpr-literal-bool.hpp"
#include "sexpr-cons.hpp"
#include "sexpr-ident.hpp"
#include "dslang-dialect.hpp"
#include "dslang-lexer.hpp"
using DSL::SexprIDENT;
using namespace DSL::detail;

#include <string>
using std::string;


// ---------------------------------------------------------------------


CX_CONSTRUCTOR6(DSL::detail::Context::Context,
                                    : Scheme(),
                                      nil_(new SexprCons(this)),
                                      void_(new SexprVoid(this)),
                                      true_(new SexprBool(this)),
                                      false_(new SexprBool(this)),
                                      env_(new SexprEnv(this,nullptr)))

  // NIL & VOID registrations
  SexprCons::NIL = nil_.get();
  SexprVoid::VOID = void_.get();

  // bool registration
  SexprBool::TRUE = true_.get();
  SexprBool::FALSE = false_.get();

  // TODO HACK: bind 'else' to TRUE
  env_->SetVariable("else", SexprBool::TRUE);


  // keyword registration
#define SPECIAL(name) SPECIAL2(name, #name)
#define SPECIAL2(name, str) KEYWORD(name, str, CallType::SPECIAL)
#define REGULAR(name) REGULAR2(name, #name)
#define REGULAR2(name, str) KEYWORD(name, str, CallType::REGULAR)
#define KEYWORD(name, str, special) \
                { str, special, DSL::detail::keyword__##name },
  struct KeywordDef
  {
    const char* name;
    CallType type;
    Keyword* func;
  };
  KeywordDef temp[] = { DSL_KEYWORDS };
#undef KEYWORD
#undef REGULAR2
#undef REGULAR
#undef SPECIAL2
#undef SPECIAL

  for (const auto k: temp)
    addKeyword(k.name, k.type, k.func);

CX_ENDMETHOD



CX_METHOD(void DSL::detail::Context::Register, Sexpr const* that)

  registry_.insert(that);

CX_ENDMETHOD



CX_METHOD(void DSL::detail::Context::Deregister, Sexpr const* that)

  registry_.erase(that);

CX_ENDMETHOD


// TODO: this is just test code
void DSL::detail::Context::report()
{
  CX_TOPICOUT(dslang,
              " === Here is the registry of %lu sexprs still remaining:\n",
              registry_.size());

  for (Sexpr const* item : registry_)
  {
    CX_TOPICOUT(dslang, "serial = %lu, refcount = %lu, value = '%s'\n",
                        item->serial_, (size_t)item->refcount_,
                        item->Write().c_str());
  }

  CX_TOPICOUT(dslang,
              " --- There are still %lu sexprs remaining.\n",
              Sexpr::extant_);
}


CX_DESTRUCTOR(DSL::detail::Context::~Context)

  CX_TOPICOUT(dslang, "Destructing context: total sexpr count = %lu, "
                      "remaining = %lu\n",
                      Sexpr::numsexprs_, Sexpr::extant_);

  /////keywords_.clear();
  report();

  gc();
  report();

  symbols_.clear();
  report();

  env_->clear();
  report();

CX_ENDMETHOD



CX_METHOD(void DSL::detail::Context::gc_symbols)

  std::lock_guard<std::mutex> lock(symlock_);

  for (auto item = symbols_.cbegin(); item != symbols_.cend();)
  {
    bool moribund = item->second->Disowned();
    item->second->Adopted();  // yes, really.

    if (moribund)
    {
      auto next = symbols_.erase(item);
      item = next;
    }
    else
      ++item;
  }

CX_ENDMETHOD



CX_METHOD(void DSL::detail::Context::gc)

  SexprCons::NIL->Mark();
  SexprVoid::VOID->Mark();
  SexprBool::TRUE->Mark();
  SexprBool::FALSE->Mark();

  // the root environment itself is in the object registry,
  // and may not have any other references.  We certainly don't
  // want to GC it!
  env_->Mark();

  // protect our keywords
  for (auto item : keywords_)
    item.second->Mark();

  gc_symbols(); // see NOTE below

  for (auto item = registry_.cbegin(); item != registry_.cend();)
  {
    if ((*item)->IsMarked())
    {
      (*item)->ClearMark();
      ++item;
    }
    else
    {
      CX_TOPICOUT(dslang,
                  "GC: serial = %lu, refcount = %lu, value = '%s'\n",
                  (*item)->serial_, (size_t)(*item)->refcount_, (*item)->Write().c_str());
      auto next = registry_.erase(item);
      delete *item;
      item = next;
      // NOTE that some of the things we might have just deleted
      // are things in the symbol_ map, and consequently gc_symbols()
      // would crash if we called it after this.
    }
  }

  gc_symbols(); // see NOTE below

CX_ENDMETHOD



CX_METHOD(Sexpr const* DSL::detail::Context::read, char const** input)

  lexer::skip_any_whitespace(input);
  if (!**input)
    CX_RETURN(nullptr); // #<void>

  TokenHandler const* handler = dialect().GetTokenHandler(*input);
  if (!handler)
  {
    char const* begin = *input;
    lexer::skip_non_whitespace(input);
    string const& bad = lexer::duplicate_text(begin, *input);
    CX_THROW( DSL::Exception,
              DSL::Error::UNDEFINED,
              "Unknown handler encountered: '%s'",
              bad.c_str() );
  }

  CX_RETURN(handler->parse(this, input));

CX_ENDMETHOD



CX_CONSTMETHOD(U32 DSL::detail::Context::hashSymbolName,
                                                    string const& name)

  U32 result;
  result = std::hash<std::string>{}(name);

  CX_RETURN(result);

CX_ENDMETHOD



CX_CONSTMETHOD(SexprIdent const* DSL::detail::Context::getSymbol,
                                              string const& name,
                                              U32* hash)

  // NOTE this routine requires that its caller has already
  // taken the symlock_ mutex

  *hash = hashSymbolName(name);
  U32 count = symbols_.count(*hash);

  if (count)
  {
    auto const& range = symbols_.equal_range(*hash);
    for (auto it=range.first; it != range.second; ++it)
    {
      SexprIdent const* that = it->second.get();
      if (name == that->Write())
        CX_RETURN(that);
    }
  }

  CX_RETURN(nullptr);

CX_ENDMETHOD



CX_METHOD(void DSL::detail::Context::AddSymbol,
                                              SexprIdent const* ident)


  std::lock_guard<std::mutex> lock(symlock_);

  string const& name = ident->Write();

  U32 hash;
  SexprIdent const* exists = getSymbol(name, &hash);

  if (exists)
    CX_ASSERT(false); // TODO, exception

  symbols_.insert(std::pair<U32,SexprIDENT const>(hash, ident));

CX_ENDMETHOD



CX_CONSTMETHOD(SexprIdent const* DSL::detail::Context::GetSymbol,
                                                    string const& ident)

  std::lock_guard<std::mutex> lock(symlock_);

  U32 hash;
  CX_RETURN(getSymbol(ident, &hash));


CX_ENDMETHOD



CX_METHOD(void DSL::detail::Context::addKeyword,  string const& name,
                                                  CallType type,
                                                  Keyword handler)

  // no check for existing keyword; this permits overloading /
  // overriding of keywords by derived implementations
  SexprKeyword const* keyword = new SexprKeyword(this, name, handler, type);
  keywords_[name] = keyword;

CX_ENDMETHOD



CX_CONSTMETHOD(SexprKeyword const* DSL::detail::Context::GetKeyword,
                                                    string const& name)

  auto item = keywords_.find(name);

  if (item != keywords_.end())
    CX_RETURN(item->second.get());

  CX_RETURN(nullptr);

CX_ENDMETHOD


