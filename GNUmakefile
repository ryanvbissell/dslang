# vim: set ft=make:
#
# Copyright (c) 2017, Ryan V. Bissell
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
# See the enclosed "LICENSE" file for exact license terms.
#

.DEFAULT_GOAL:=repl

define MF_HELPDOC
USAGE:  make <target> [flags]
SUPPORTED TARGETS:
*   help                -- this message
*   clean               -- erase dslang-specific contents of out dir
*   repl   (default)    -- build dslang executable
*   static              -- build dslang.a
SUPPORTED FLAGS:
All of the usual (CPPFLAGS, CXXFLAGS, etc.) plus:
*   CROSS_COMPILE=...   -- target triplet of toolchain to use
*   DEBUG=1             -- build debug target
*   PROFILE=1           -- build profile-able target
*   VERBOSE=1           -- verbose GNUMakefile output
*   TEST=1              -- dry-run of recipes for <target>
*   PLAID=1             -- dslang goes into ludicrous speed

CX-RELATED FLAGS:
*   CXDIR=<path>        -- path to alternate CX src tree
*   CXDEBUG=1           -- build a CX-enabled binary (topics)
*   CXTRACE=1           -- build a CX-enabled binary (tracing)
*   CXALL=1             -- same as CXDEBUG=1 CXTRACE=1

After building with CXDEBUG, the following environment variables
are honored at runtime:
*   CX_TOPICS="foo bar" -- show all topical debug output in
.                          topics 'foo' and 'bar'
After building with CXTRACE, the following environment
variables are honored at runtime:
*   CX_TRACE="*"        -- show all tracing sections
*   CX_TRACE="foo bar"  -- show 'foo' and 'bar' tracing sections
*   CX_TRACEFILE=...    -- Path of file to dump tracing log to (instead
.                          of stdout)
endef


# initialize the build system
MFOUT:=$(CURDIR)/out
MF_PROJECT_DIR:=$(CURDIR)
include $(MF_PROJECT_DIR)/_forge/make-forge

# import rules for building libcx.a
CXOUT:=$(MFOUT)
CXDIR:=$(CURDIR)/sub/cx
WITH:=intrusive
$(call mf-include-as,cxlib,$(CXDIR)/GNUmakefile.MF)


ifdef PLAID
override CF_CPPFLAGS+=-DDSL_OPT_FASTCAST=1
#override CF_CPPFLAGS+=-DDSL_OPT_MEMOCONS=1
endif


# import rules for building dslang.a
$(call mf-include-as,dslang,./GNUmakefile.MF)

static: static@dslang

# describe how to build repl
$(call mf-declare-target,repl)
    $(call mf-static-dependencies,static@cxlib static@dslang)
    #override LDFLAGS+=-lpanel -lcurses
    override CPPFLAGS+=-I$(CXDIR)/inc -I$(CURDIR)/inc
    $(call mf-add-sources,C++,$(CURDIR)/src/apps,repl.cpp)
    $(call mf-build-executable)


# describe how to build bench
$(call mf-declare-target,bench)
    $(call mf-set-object-suffix,BENCH)
    $(call mf-static-dependencies,static@cxlib static@dslang)
    #override LDFLAGS+=-lpanel -lcurses
    override CPPFLAGS+=-I$(CXDIR)/inc -I$(CURDIR)/inc -DDSL_OPT_BENCHMARK
    $(call mf-add-sources,C++,$(CURDIR)/src/apps,bench.cpp)
    $(call mf-build-executable)

