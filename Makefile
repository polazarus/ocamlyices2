all: ext build
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
CMXS_FILE      = src/$(LIB_NAME).cmxs
LIB_FILE       = src/lib$(LIB_NAME).a
A_FILE         = src/$(LIB_NAME).a                # Generated with the .cmxa
DLL_FILE       = src/dll$(LIB_NAME).so

ANNOT_FILES    = $(ML_SOURCE:%.ml=%.annot)
ifdef BIN_ANNOT
  ANNOT_FILES += $(ML_SOURCE:%.ml=%.cmt) $(MLI_SOURCE:%.mli=%.cmti)
endif

TESTS_BYTE   = $(TESTS:%.ml=%.byte)
TESTS_OPT    = $(TESTS:%.ml=%.opt)


# Build and install files

BUILD_FILES         = $(CMA_FILE) $(DLL_FILE)
INSTALL_FILES       = META \
                      $(CMA_FILE) $(DLL_FILE) \
                      $(MLI_SOURCE) $(CMI_FILES) \
                      $(ANNOT_FILES) \
                      ext/libyices.a

ifdef HAVE_OCAMLOPT
  BUILD_FILES      += $(CMXA_FILE) $(LIB_FILE) $(CMXS_FILE)
  INSTALL_FILES    += $(CMXA_FILE) $(A_FILE) $(LIB_FILE) $(CMXS_FILE)
endif

###############################################################################

ZARITH_LIBDIR       = $(shell $(OCAMLFIND) query zarith)

# Commands

compile_byte        = $(OCAMLFIND) ocamlc -package zarith -I src \
                      -annot $(BIN_ANNOT) -c -o

compile_interface   = $(compile_byte)

compile_native      = $(OCAMLFIND) opt -package zarith -I src -c -o

compile_stub        = $(CC) $(CFLAGS) -fPIC -I$(ZARITH_LIBDIR) -Iext -Isrc -std=c99 -c -o

compile_stub_static = $(CC) $(CFLAGS) -I$(ZARITH_LIBDIR) -Iext -Isrc -std=c99 -c -o

link_byte           = $(OCAMLFIND) c -a -package zarith \
                      -dllib -l$(LIB_NAME) -o

link_native         = $(OCAMLFIND) opt -a -package zarith \
                      -cclib -l$(LIB_NAME) -o

link_native_shared  = $(OCAMLFIND) opt -shared -package zarith \
                      -ccopt -Lext -o

link_stubs_shared   = gcc -shared -Lext -o

link_stubs_static   = $(AR) rcs

compile_test_native = $(OCAMLFIND) opt -package zarith -linkpkg \
                      -I src -ccopt -Lext \
                      $(CMXA_FILE)

compile_test_byte   = $(OCAMLFIND) c -package zarith -linkpkg \
                      -I src -ccopt -Lext \
                      $(CMA_FILE)

ifdef HAVE_OCAMLDOC
  gen_doc           = $(OCAMLFIND) ocamldoc -package zarith -I src -html \
                      -charset utf-8 -d
else
  gen_doc           = @echo "Error: Cannot generate API doc. \
                      Please install 'ocamldoc'."
endif

################################################################################

ext: ext/libyices.a

ext/libyices.a:
	$(MAKE) -C ext

debug: CFLAGS += -DDEBUG
debug: build

build: $(BUILD_FILES)

# Dependencies #################################################################

src/yices2.cmo src/yices2.cmx: src/yices2.cmi

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

%.o: %.c
	$(compile_stub) $@ $<
%.lo: %.c
	$(compile_stub_static) $@ $<
%.cmi: %.mli
	$(compile_interface) $@ $<
%.cmo: %.ml
	$(compile_byte) $@ $<
%.cmx: %.ml
	$(compile_native) $@ $<


# Library compilation ##########################################################

$(DLL_FILE): $(OBJECTS)
	$(link_stubs_shared) $@ $^ $(LIBS)
# $(LIBS)'s position matters here

$(LIB_FILE): $(OBJECTS_STATIC)
	$(link_stubs_static) $@ $^

$(CMA_FILE): $(DLL_FILE) $(CMO_FILES)
	$(link_byte) $@ $(CMO_FILES)

$(CMXA_FILE): $(LIB_FILE) $(CMX_FILES)
	$(link_native) $@ $(CMX_FILES) -cclib '$(LIBS)'

$(CMXS_FILE): $(OBJECTS) $(CMX_FILES)
	$(link_native_shared) $@ $^ -cclib '$(LIBS)'
# $(LIBS)'s position matters here


# Documentation ################################################################

doc:
	$(gen_doc) doc $(MLI_SOURCE)


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


# Testing ######################################################################

$(TESTS_OPT): %.opt: %.ml
	$(compile_test_native) $< -o $@

$(TESTS_BYTE): %.byte: %.ml
	$(compile_test_byte) $< -o $@

test: test.byte

ifdef HAVE_OCAMLOPT
test: test.opt
endif

test.byte: build $(TESTS_BYTE)
	@cd tests; for testfile in *.byte; do\
	  if ./$$testfile ; then\
	    echo "test '$${testfile}' passed";\
	  else\
	    echo "test '$${testfile}' failed";\
	  fi;\
	done

test.opt: build $(TESTS_OPT)
	@cd tests; for testfile in *.opt; do\
	  if ./$$testfile ; then\
	    echo "test '$${testfile}' passed";\
	  else\
	    echo "test '$${testfile}' failed";\
	  fi;\
	done

cleantest:
	$(RM) tests/*.byte tests/*.opt tests/*.cm[ox]

################################################################################

.PHONY: all build install uninstall clean test debug test.opt test.byte ext doc
