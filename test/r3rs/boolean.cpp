// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#include "dslrep.hpp"
#include <string>

using DSL::SEXPR;
using std::string;


char const * tests[][2] =
{
  { "#f",               "#f"  },
  { "#t",               "#t"  },
  { "'#f",              "#f"  },
  { "'#t",              "#t"  },

  { "(not #t)",         "#f"  },
  { "(not 3)",          "#f"  },
  { "(not (list 3))",   "#f"  },
  { "(not #f)",         "#t"  },
  { "(not '())",        "#f"  },
  { "(not (list))",     "#f"  },

  { "(boolean? #f)",    "#t"  },
  { "(boolean? #t)",    "#t"  },
  { "(boolean? 0)",     "#f"  },

  { "(and #f #f #f)",   "#f"  },
  { "(and #f #t #f)",   "#f"  },
  { "(and #t #f #t)",   "#f"  },
  { "(and #t #t #t)",   "#t"  },

  { "(or #f #f #f)",    "#f"  },
  { "(or #f #t #f)",    "#t"  },
  { "(or #t #f #t)",    "#t"  },
  { "(or #t #t #t)",    "#t"  },
};
#define NUM_TESTS ((sizeof(tests)/sizeof(const char*))/2)





int main(int argc, const char** argv)
{
  DSL::Scheme* scheme = DSL::SchemeFactory();

  for (int i=0; i<NUM_TESTS; ++i)
    DSL_ASSERT(scheme,tests[i][0], tests[i][1]);
}

