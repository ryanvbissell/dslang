# vim: set ft=make:
#
# Copyright (c) 2017, Ryan V. Bissell
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
# See the enclosed "LICENSE" file for exact license terms.
#


$(call tf-export-makeforge-as,common,$(TF_TESTROOT)/_common/dslrep.MF)

$(call tf-declare-target,BOOL)
    override CPPFLAGS:=-I$(TF_TESTROOT) -I${TF_TESTROOT}/../inc
    override CPPFLAGS+=-I$(CXDIR)/inc -I$(TF_TESTROOT)/_common
    override CPPFLAGS+=-DCX_OPSYS=$(HOSTOS)
    override CPPFLAGS+=-DCX_OPSYS_HEADER=os/$(HOSTOS)
    $(call tf-static-dependencies,dslrep@common static@cxlib@common static@dslang@common)
    $(call tf-add-sources,C++,${TF_TESTDIR},boolean.cpp)
    $(call tf-build-executable)

$(call tf-test-exitstatus,boolean)


$(call tf-declare-target,LISTS)
    override CPPFLAGS:=-I$(TF_TESTROOT) -I${TF_TESTROOT}/../inc
    override CPPFLAGS+=-I$(CXDIR)/inc -I$(TF_TESTROOT)/_common
    override CPPFLAGS+=-DCX_OPSYS=$(HOSTOS)
    override CPPFLAGS+=-DCX_OPSYS_HEADER=os/$(HOSTOS)
    $(call tf-static-dependencies,dslrep@common static@cxlib@common static@dslang@common)
    $(call tf-add-sources,C++,${TF_TESTDIR},lists.cpp)
    $(call tf-build-executable)

$(call tf-test-exitstatus,lists)
