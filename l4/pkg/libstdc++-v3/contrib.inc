# vi:ft=make

__NUMBERS := x x x x x x x x x x x x x x x x x x x x

n = $(wordlist 1, $1, $(__NUMBERS))
max = $(subst xx,x,$(join $1,$2))
min = $(subst xx,x,$(filter xx,$(join $1,$2)))
gt = $(filter-out $(words $2),$(words $(call max,$1,$2)))
eq = $(filter $(words $1),$(words $2))
gte = $(call gt,$1,$2)$(call eq,$1,$2)
mul = $(foreach a,$1,$2)


__GCCMAJOR = $(wordlist 1, $(GCCMAJORVERSION), $(__NUMBERS))
__GCCMINOR = $(wordlist 1, $(GCCMINORVERSION), $(__NUMBERS))
__GCCSUBVER= $(wordlist 1, $(GCCSUBVERSION), $(__NUMBERS))

SDTCXX_0x = n

STDCXX_AVAILABLE_VERSIONS = 4.1.0 4.3.3 4.5 4.6

ifeq ("$(GCCMAJORVERSION)", "4")
  ifeq ("$(GCCMINORVERSION)", "3")
    STDCXX_CONTRIB_VERSION := 4.3.3
  endif
  ifeq ("$(GCCMINORVERSION)", "4")
    STDCXX_CONTRIB_VERSION := 4.3.3
  endif
  ifeq ("$(GCCMINORVERSION)", "5")
    STDCXX_CONTRIB_VERSION := 4.5
    STDCXX_0x = y
  endif
  ifeq ("$(GCCMINORVERSION)", "6")
    STDCXX_CONTRIB_VERSION := 4.6
    STDCXX_CONTRIB_VERSION_4.6_PLUS := y
    STDCXX_0x = y
  endif
  ifeq ("$(GCCMINORVERSION)", "7")
    STDCXX_CONTRIB_VERSION := 4.6
    STDCXX_CONTRIB_VERSION_4.6_PLUS := y
    STDCXX_0x = y
  endif
  ifneq ("$(call gte, $(__GCCMINOR), $(call n, 1))", "")
    STDCXX_CONTRIB_VERSION ?= 4.1.0
  endif
endif

STDCXX_CONTRIB_DIR = $(STDCXX_PKG_DIR)/contrib/libstdc++-v3-$(STDCXX_CONTRIB_VERSION)
DEFINES += -Dlinux

CPU_H_x86   := i386
CPU_H_ppc32 := powerpc
CPU_H_arm   := arm
CPU_H_amd64 := i386
CPU_H_sparc := sparc

CPU_H_generic := config/cpu/generic
CPU_H_arch     = config/cpu/$(CPU_H_$(BUILD_ARCH))

CPU_file = $(word 1, $(wildcard $(STDCXX_CONTRIB_DIR)/$(CPU_H_arch)/$(strip $(1)) $(STDCXX_CONTRIB_DIR)/$(CPU_H_generic)/$(strip $(1))))

#$(info GCC: $(STDCXX_CONTRIB_DIR))

