include Makefile.config

# Source files

ML_SOURCE    = src/yices2.ml
MLI_SOURCE   = src/yices2.mli
C_SOURCE     = src/contexts.c src/models.c src/terms.c src/types.c src/misc.c
TESTS        = $(wildcard tests/*.ml)


# Compiled files

OBJECTS      = $(C_SOURCE:%.c=%.o)
CMO_FILES    = $(ML_SOURCE:%.ml=%.cmo)
CMX_FILES    = $(ML_SOURCE:%.ml=%.cmx)
CMI_FILES    = $(MLI_SOURCE:%.mli=%.cmi)
CMA_FILE     = src/$(LIB_NAME).cma
CMXA_FILE    = src/$(LIB_NAME).cmxa
LIB_FILE     = src/lib$(LIB_NAME).a
A_FILE       = src/$(LIB_NAME).a                # Generated with the .cmxa
DLL_FILE     = src/dll$(LIB_NAME).so

ANNOT_FILES  = $(ML_SOURCE:%.ml=%.annot)
ifneq ($(filter -bin-annot, $(ANNOTFLAG)),)
ANNOT_FILES += $(ML_SOURCE:%.ml=%.cmt) $(MLI_SOURCE:%.mli=%.cmti)
endif

TESTS_BYTE   = $(TESTS:%.ml=%.byte)
TESTS_OPT    = $(TESTS:%.ml=%.opt)


# Build and install files

BUILD_FILES    = $(CMA_FILE) src/dll$(LIB_NAME).so $(CMI_FILES)
INSTALL_FILES  = $(CMA_FILE) $(MLI_SOURCE) $(CMI_FILES) \
                 $(ANNOT_FILES) ext/libyices.a
INSTALL_DLLS   = $(DLL_FILE)

ifneq ($(OCAMLOPT),)
BUILD_FILES   += $(CMXA_FILE) $(LIB_FILE)
INSTALL_FILES += $(CMXA_FILE) $(A_FILE) $(LIB_FILE)
endif


# Commands

compile_byte        = $(OCAMLFIND) c -package zarith -I src -c $(ANNOTFLAG)
compile_interface   = $(compile_byte)
compile_native      = $(OCAMLFIND) opt -package zarith -I src -c $(ANNOTFLAG)
compile_stubs       = $(CC) $(CFLAGS) -I$(shell $(OCAMLFIND) query zarith) \
                      -Iext -std=c1x -c

link_byte           = $(OCAMLFIND) c -a -package zarith \
                      -dllib -l$(LIB_NAME)
link_native         = $(OCAMLFIND) opt -a -package zarith \
                      -cclib -l$(LIB_NAME) -cclib '$(LIBS)'
link_dll            = gcc -shared -Lext

compile_test_native = $(OCAMLFIND) opt -package zarith -linkpkg \
                      -I src -ccopt -Lext \
                      $(CMXA_FILE)
compile_test_byte   = $(OCAMLFIND) c -package zarith -linkpkg \
                      -I src -ccopt -Lext \
                      $(CMA_FILE)

################################################################################

all: ext build

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
	$(compile_stubs) -o $@ $<
%.cmi: %.mli
	$(compile_interface) -o $@ $<
%.cmo: %.ml
	$(compile_byte) -o $@ $<
%.cmx: %.ml
	$(compile_native) -o $@ $<


# Library compilation ##########################################################

$(DLL_FILE): $(OBJECTS)
	$(link_dll) -o $@ $^ $(LIBS)
$(LIB_FILE): $(OBJECTS)
	$(AR) rcs $@ $^
$(CMA_FILE): $(DLL_FILE) $(CMO_FILES)
	$(link_byte) -o $@ $(CMO_FILES)
$(CMXA_FILE): $(LIB_FILE) $(CMX_FILES)
	$(link_native) -o $@ $(CMX_FILES)


# (Un)Install ##################################################################

install: build
	$(OF_INSTALL) $(LIB_NAME) -nodll $(INSTALL_FILES) META -dll $(INSTALL_DLLS)
uninstall:
	$(OF_REMOVE) $(LIB_NAME)


# Clean up #####################################################################

clean:
	$(RM) */*.o */*.[aos] */*.cm[aoxit] */*.cmti */*.cmxa */*.annot */*.so \
 */a.out .depend


# Testing ######################################################################

$(TESTS_OPT): %.opt: %.ml
	$(compile_test_native) $< -o $@

$(TESTS_BYTE): %.byte: %.ml
	$(compile_test_byte) $< -o $@

test: test.byte

ifneq ($(OCAMLOPT),)
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

.PHONY: all build install uninstall clean test debug test.opt test.byte ext
