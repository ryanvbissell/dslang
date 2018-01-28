// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2014-2018, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "bench"

#include "dslang.hpp"

#include <stdlib.h>

// ---------------------------------------------------------------------

const char* fib = R"(
(define (fib n)
    (cond
        ((= n 0) 0)
        ((= n 1) 1)
        (else
            (+ (fib (- n 1))
               (fib (- n 2)) ) )))
)";

const char* _fibseq = R"(
(define (_fibseq n max)
    (print (fib (- max n)))
    (if (> n 0)
        (_fibseq (- n 1) max) ) )
)";

const char* fibseq = "(define (fibseq n) (_fibseq n n) )\n";

CX_FUNCTION(void benchmark)

  DSL::Scheme* scheme = DSL::SchemeFactory<DSL::Dialect>();

  char buf[1024];
  const char* ptr;

  scheme->ReadEvalPrint(&fib);
  scheme->ReadEvalPrint(&_fibseq);
  scheme->ReadEvalPrint(&fibseq);

  const char* fib30 = "(fibseq 30)";
  scheme->ReadEvalPrint(&fib30);

  delete scheme;

CX_ENDMETHOD



CX_FUNCTION(int main, int argc, char** const argv)

  CX_TRY
  {
    benchmark();
  }
  CX_CATCH(CX::Exception& e)
  {
    e.StdError();
    CX_RETURN(EXIT_FAILURE);
  }
  CX_ENDTRY

  CX_RETURN(EXIT_SUCCESS);

CX_ENDFUNCTION



