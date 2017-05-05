all: ext/lib/libyices.a build
# Be helpful if not configured
Makefile.config: configure
	@echo Please run ./configure first; exit 1
include Makefile.config

################################################################################

# Source files

ML_SOURCE    = src/yices2.ml
MLI_SOURCE   = src/yices2.mli
C_SOURCE     = src/contexts.c src/models.c src/terms.c src/types.c src/misc.c
TESTS        = $(wildcard tests/*.ml)


# Compiled files

OBJECTS        = $(C_SOURCE:%.c=%.o)
OBJECTS_STATIC = $(C_SOURCE:%.c=%.lo)
CMO_FILES      = $(ML_SOURCE:%.ml=%.cmo)
CMX_FILES      = $(ML_SOURCE:%.ml=%.cmx)
CMI_FILES      = $(MLI_SOURCE:%.mli=%.cmi)
CMA_FILE       = src/$(LIB_NAME).cma
CMXA_FILE      = src/$(LIB_NAME).cmxa
LIB_FILE       = src/lib$(LIB_NAME)_stubs.a
A_FILE         = src/$(LIB_NAME).a                # Generated with the .cmxa
DLL_FILE       = src/dll$(LIB_NAME)_stubs.$(SO)

ANNOT_FILES    = $(ML_SOURCE:%.ml=%.annot)
ifdef BIN_ANNOT
  ANNOT_FILES += $(ML_SOURCE:%.ml=%.cmt) $(MLI_SOURCE:%.mli=%.cmti)
endif

TESTS_BYTE   = $(TESTS:%.ml=%.byte)
TESTS_OPT    = $(TESTS:%.ml=%.opt)


# Build and install files
# For now, DLL_FILE is not built because it needs either a PIC libyices.a
# that we could build with --with-pic, or the shared library libyices.so.
# For now, only the static stub is built and linked statically against
# non-PIC libyices.a.
# We also put libgmp.a (if a static libgmp has been found/given) to be sure
# that a static version of libgmp is picked in priority when linking against
# Zarith. To avoid this behaviour, just pass --with-shared-gmp.
BUILD_FILES         = $(CMA_FILE) $(LIB_FILE)
INSTALL_FILES       = META \
                      $(CMA_FILE) $(LIB_FILE) \
                      $(MLI_SOURCE) $(CMI_FILES) \
                      $(ANNOT_FILES) \
					  ext/lib/libyices.a \
					  $(if $(STATIC_GMP),src/libgmp.a)

ifdef HAVE_OCAMLOPT
  BUILD_FILES      += $(CMXA_FILE) $(CMXS_FILE) $(A_FILE)
  INSTALL_FILES    += $(CMXA_FILE) $(CMXS_FILE) $(A_FILE)
endif

###############################################################################

OCAML_LIBDIR        = $(shell $(OCAMLFIND) printconf stdlib)
ZARITH_LIBDIR       = $(shell $(OCAMLFIND) query zarith)

# This is given to any ocamlfind command
PACKAGES            = -package zarith
ifdef HAVE_OUNIT
    PACKAGES       += -package oUnit
endif

################################################################################
src/libyices.a: ext/lib/libyices.a
	cp $^ $@

MAKE_VARIABLES = \
	$(if $(STATIC_GMP),STATIC_GMP=$(STATIC_GMP)) \
	$(if $(FORCE_SHARED_GMP),FORCE_SHARED_GMP=$(FORCE_SHARED_GMP)) \
	HOST=$(HOST)
ext/lib/libyices.a:
	$(MAKE) -C ext $(MAKE_VARIABLES)

debug: CFLAGS += -DDEBUG
debug: build

build: .depend $(BUILD_FILES)

# Dependencies #################################################################
# Only the C dependencies are listed here.
# The ocaml dependencies are created using the target '.depend'. The file
# .depend contains all the dependencies and is included at the end of this
# makefile.
src/contexts.o: src/contexts.c src/config.h src/contexts.h src/misc.h \
  src/terms.h
src/misc.o: src/misc.c src/config.h src/misc.h src/terms.h src/types.h \
  src/models.h src/errors.h
src/models.o: src/models.c src/config.h src/models.h src/misc.h src/terms.h \
  src/contexts.h
src/terms.o: src/terms.c src/config.h src/terms.h src/misc.h src/types.h \
  src/terms_macros.h
src/types.o: src/types.c src/config.h src/types.h src/terms.h src/misc.h


# Generic compilation ##########################################################

# For compiling .c to .o that will serve for creating libyices2_stubs.a.
# -custom will simply integrate the ocamlrun.a in the library. With -custom,
# the resulting library will be forced to be a static library (.a).
%.o: %.c
	$(OCAMLFIND) ocamlc $(PACKAGES) -g -custom -c $< -ccopt '-std=c99 $(CFLAGS) -Iext/include -Isrc $(CPPFLAGS)'
	mv $(notdir $@) $@

ifneq ($(STATIC_GMP),)
src/libgmp.a: $(STATIC_GMP)
	cp $^ $@
$(LIB_FILE) $(DLL_FILE): | src/libgmp.a
endif

# Compile libyices2_stubs.a and dllyices2_stubs.so.
# NOTE: If I use the flag -Lext for finding the library -lyices, the resulting
# library will complain about -Lext being useless.
# To avoid that, we ocamlmklib in the same dir as libyices.a and use -L. (which
# should not output any warning when building with the yices2 package).
#
# The preprocessor variable -DNOYICES_DLL allows to disable the linking to a dll
# and thus you can link to a static libyices.a instead of libyices.dll.a/libyices.a.
# -custom = only produce static libyices_stubs.a, not the shared dllyices_stubs.so.
$(LIB_FILE) $(DLL_FILE): $(OBJECTS) | src/libyices.a
	cd src && $(OCAMLFIND) ocamlmklib -o $(LIB_NAME)_stubs $(notdir $^) $(LDFLAGS) $(LIBS) -custom -lyices -L. -ccopt -DNOYICES_DLL

src/%.cmi: src/%.mli
	$(OCAMLFIND) ocamlc $(PACKAGES) -I src -annot $(BIN_ANNOT) -c -o $@ $<
src/%.cmo: src/%.ml
	$(OCAMLFIND) ocamlc $(PACKAGES) -I src -annot $(BIN_ANNOT) -c -o $@ $<
src/%.cmx: src/%.ml
	$(OCAMLFIND) opt $(PACKAGES) -I src -c -o $@ $<

# Library compilation ##########################################################

%$(LIB_NAME).cma: %$(LIB_NAME).cmo | %lib$(LIB_NAME)_stubs.a %dll$(LIB_NAME)_stubs.$(SO)
	$(OCAMLFIND) ocamlc -a \
	-cclib '-l$(LIB_NAME)_stubs -lyices $(LDFLAGS) $(LIBS)' \
	-custom $^ -o $@

%$(LIB_NAME).cmxa %$(LIB_NAME).a: %$(LIB_NAME).cmx | %lib$(LIB_NAME)_stubs.a
	$(OCAMLFIND) ocamlopt -a \
	-cclib '-l$(LIB_NAME)_stubs -lyices $(LDFLAGS) $(LIBS)' \
	$^ -o $@

%$(LIB_NAME).cmxs %$(LIB_NAME).$(SO): %$(LIB_NAME).cmxa %$(LIB_NAME).cmx | %dll$(LIB_NAME)_stubs.$(SO)
	$(OCAMLFIND) ocamlopt -shared -I $(dir $@) $^ -o $@

# Documentation ################################################################

ifndef HAVE_OCAMLDOC
  @echo "Error: Cannot generate API doc. Please install 'ocamldoc'."
endif
doc:
	$(OCAMLFIND) ocamldoc $(PACKAGES) -I src -html -charset utf-8\
		-d doc $(MLI_SOURCE)


# (Un)Install ##################################################################

install: build
	$(OF_INSTALL) $(LIB_NAME) $(INSTALL_FILES)
uninstall:
	$(OF_REMOVE) $(LIB_NAME)


# Clean up #####################################################################

CLEAN_EXTS = */*.[aos] */*.cm[aoxit] */*.cmti */*.cmx[as] */*.annot \
             */*.so */a.out .depend

clean:
	$(RM) $(CLEAN_EXTS)
	make -C ext clean

cleansrc:
	$(RM) src/*.[aos] src/*.cm[aoxit] src/*.cmti src/*.cmx[as] src/*.annot \
             src/*.so src/a.out .depend


# Testing ######################################################################

.depend: $(wildcard */*.mli) $(wildcard */*.ml)
	$(OCAMLFIND) ocamldep -I src -I tests $(PACKAGES) $^ > .depend

tests/%.cmo: tests/%.ml
	$(OCAMLFIND) ocamlc -c -g -annot $(BIN_ANNOT) -I src -I tests $(PACKAGES) $< -o $@

tests/%.cmx: tests/%.ml
	$(OCAMLFIND) ocamlopt -c -g -annot $(BIN_ANNOT) -I src -I tests $(PACKAGES) $< -o $@

# Compile tests (native)
# The filter-out is here because I need tests/test.ml to be the last one in
# the list, because the order of .cmo matters with ocamlc.
tests/test.byte: $(filter-out tests/test.cmo, $(TESTS:%.ml=%.cmo)) tests/test.cmo
	$(OCAMLFIND) ocamlc -g -linkpkg $(PACKAGES) \
	-I src -I tests $(CMA_FILE) $^ -o $@

tests/test.opt: $(filter-out tests/test.cmx, $(TESTS:%.ml=%.cmx)) tests/test.cmx
	$(OCAMLFIND) ocamlopt -g -linkpkg $(PACKAGES) \
	-I src -I tests $(CMXA_FILE) $^ -o $@

test: test.byte

ifdef HAVE_OCAMLOPT
test: test.opt
endif

ifdef HAVE_OUNIT
test.byte: build tests/test.byte
	@./tests/test.byte
test.opt: build tests/test.opt
	@./tests/test.opt
else
test.byte test.opt test:
	@echo The ocaml package 'ounit' is not installed. Install it with 'opam install ounit'.
	@exit 1
endif

cleantest:
	$(RM) tests/*.byte tests/*.opt tests/*.cm[ox]

################################################################################

.PHONY: all build install uninstall clean test debug test.opt test.byte ext doc cleansrc

# The .SECONDARY is necessary because if I don't put it, make will remove
# src/libyices_stubs.a because for some reason it considers it as an
# intermediate file. I have no idea why!
.SECONDARY: $(DLL_FILE) $(LIB_FILE)

-include .depend