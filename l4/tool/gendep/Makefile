L4DIR ?= ../..
include $(L4DIR)/mk/Makeconf

VERSION	= 0.2
CSRC	= deptrack.c syscall.c
MAN	= man/man7/libgendep.so.7
MAN_SRC	= manpage.dox Doxyfile
DDIR	= gendep-$(VERSION)

CC	= $(HOST_CC)
CFLAGS	= -fPIC -Wall -pedantic -g

checkbuild = $(shell if echo 'int main(){return 0;}' | \
                     $(CC) -m$(1) $(CFLAGS) -o /dev/null  \
                     -x c - > /dev/null 2>&1; then echo 1; fi)

OBJS32	= $(addprefix $(OBJ_DIR)/,$(CSRC:.c=.32.o))
OBJS64  = $(addprefix $(OBJ_DIR)/,$(CSRC:.c=.64.o))
LIB32   := $(if $(call checkbuild,32),$(OBJ_DIR)/32/libgendep.so)
LIB64   := $(if $(call checkbuild,64),$(OBJ_DIR)/64/libgendep.so)


ifneq ($(filter linux freebsd,$(HOST_SYSTEM)),)
LIB     := $(LIB32) $(LIB64)
LIBDL-linux := -ldl
cmd_link = $(HOST_CC) -m$(2) -shared -Wl,--no-as-needed,-soname,$(1) $(LIBDL-$(HOST_SYSTEM)) -o $(1)
else
ifeq ($(HOST_SYSTEM),darwin)
LIB     := $(OBJ_DIR)/libgendep.so
cmd_link = $(HOST_CC) -m$(2) -dynamiclib -o $(1)
$(LIB): $(LIB32) $(LIB64)
	lipo -create $^ -output $@
else
$(error Your host system type "$(HOST_SYSTEM)" is not supported here. Fix me.)
endif
endif

ifeq ($(LIB),)
$(error Neither 32 nor 64 bit version are built?!)
endif

all:: $(LIB)

$(OBJ_DIR)/%.32.o: $(SRC_DIR)/%.c $(SRC_DIR)/Makefile
	$(CC) $(CFLAGS) -m32 -c $< -o $@

$(OBJ_DIR)/%.64.o: $(SRC_DIR)/%.c $(SRC_DIR)/Makefile
	$(CC) $(CFLAGS) -m64 -c $< -o $@

$(LIB32): $(OBJS32) $(SRC_DIR)/Makefile
	$(MKDIR) $(OBJ_DIR)/32
	$(call cmd_link,$@,32) $(OBJS32)

$(LIB64): $(OBJS64) $(SRC_DIR)/Makefile
	$(MKDIR) $(OBJ_DIR)/64
	$(call cmd_link,$@,64) $(OBJS64)

doc: $(MAN)

$(MAN): $(MAN_SRC)
	doxygen

install:: $(LIB) $(MAN)
	$(if $(DROPS_STDDIR),,$(error DROPS_STDDIR is not set. Do a 'make config' in $(L4DIR)))
	$(VERBOSE)install -d $(DROPS_STDDIR)/tool/lib
	$(VERBOSE)install -c $(LIB) $(DROPS_STDDIR)/tool/lib
	$(VERBOSE)install -d $(DROPS_STDDIR)/tool/man/man7
	$(VERBOSE)install -c $(MAN) $(DROPS_STDDIR)/tool/man/man7

test:
	GENDEP_TARGET='simple-cat' \
		GENDEP_BINARY=cpp\
		GENDEP_cpp='+\.h$$ -^/usr' \
		$(LD_PRELOAD)=$(OBJ_DIR)/libgendep.so\
		$(HOST_CC) -o $(OBJ_DIR)/simple-cat simple-cat.c
	GENDEP_TARGET='blabla' \
		GENDEP_BINARY=cpp\
		$(LD_PRELOAD)=$(OBJ_DIR)/libgendep.so\
		$(HOST_CC) -o $(OBJ_DIR)/simple-cat simple-cat.c
	GENDEP_TARGET='badexp' \
		GENDEP_BINARY=cpp\
		GENDEP_cpp='\)foo'\
		$(LD_PRELOAD)=$(OBJ_DIR)/libgendep.so\
		$(HOST_CC) -o $(OBJ_DIR)/simple-cat simple-cat.c
	@echo ==========
	cat simple-cat.dep
	cat blabla.dep

clean cleanall::
	rm -f $(OBJ_DIR)/.*.d $(OBJ_DIR)/*.o $(OBJ_DIR)/simple-cat
	rm -f $(OBJ_DIR)/32/*.so $(OBJ_DIR)/64/*.so
	rm -rf $(OBJ_DIR)/32 $(OBJ_DIR)/64

cleanall::
	rm -f $(LIB)
	rm -fr man/ html/

dist:
	rm -rf $(DDIR)
	mkdir $(DDIR)
	ln $(CSRC) COPYING Doxyfile manpage.dox gendep.h Makefile \
		   WhatIsThis simple-cat.c $(DDIR)
	tar cfz $(DDIR).tar.gz $(DDIR)
	rm -rf $(DDIR)
