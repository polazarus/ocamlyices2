include Makefile.config

compile_byte      = $(OCAMLFIND) c -package zarith -I src -c $(ANNOTFLAG)
compile_interface = $(compile_byte)
compile_native    = $(OCAMLFIND) opt -package zarith -I src -c $(ANNOTFLAG)
compile_stubs     = $(CC) $(CFLAGS) -I$(shell $(OCAMLFIND) query zarith) \
                    -std=c11 -c

link_byte   = $(OCAMLFIND) c -a -package zarith -linkpkg \
              -dllib -l$(PACKAGE_NAME) -cclib '$(LIBS)'
link_native = $(OCAMLFIND) opt -a -package zarith \
              -cclib -l$(PACKAGE_NAME) -cclib '$(LIBS)'
link_dll    = gcc -shared

compile_test_native = $(OCAMLFIND) opt -package zarith -linkpkg -I src \
                      src/$(PACKAGE_NAME).cmxa
compile_test_byte   = $(OCAMLFIND) c -package zarith -linkpkg -I src \
                      src/$(PACKAGE_NAME).cma


# Source files
ML_SOURCE  = src/yices2.ml
MLI_SOURCE = src/yices2.mli
C_SOURCE   = src/contexts.c src/models.c src/terms.c src/types.c src/misc.c

TESTS = $(wildcard tests/*.ml)


# Compiled files
OBJECTS    = $(C_SOURCE:%.c=%.o)
CMO_FILES   = $(ML_SOURCE:%.ml=%.cmo)
CMX_FILES = $(ML_SOURCE:%.ml=%.cmx)
CMI_FILES = $(MLI_SOURCE:%.mli=%.cmi)

ANNOT_FILES = $(ML_SOURCE:%.ml=%.annot)
ifneq ($(filter -bin-annot, $(ANNOTFLAG)),)
ANNOT_FILES += $(ML_SOURCE:%.ml=%.cmt) $(MLI_SOURCE:%.mli=%.cmti)
endif

TESTS_BYTE = $(TESTS:%.ml=%.byte)
TESTS_OPT = $(TESTS:%.ml=%.opt)


# Build and install files
BUILD_FILES = src/$(PACKAGE_NAME).cma src/dll$(PACKAGE_NAME).so $(CMI_FILES)
INSTALL_FILES =  src/$(PACKAGE_NAME).cma $(MLI_SOURCE) $(CMI_FILES) $(ANNOT_FILES)
DLL_FILE = src/dll$(PACKAGE_NAME).so

ifneq ($(OCAMLOPT),)
BUILD_FILES += src/$(PACKAGE_NAME).cmxa src/lib$(PACKAGE_NAME).a
INSTALL_FILES += src/$(PACKAGE_NAME).cmxa src/$(PACKAGE_NAME).a src/lib$(PACKAGE_NAME).a
endif


################################################################################

all: build

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

src/dll$(PACKAGE_NAME).so: $(OBJECTS)
	$(link_dll) -o $@ $^ $(LIBS)
src/lib$(PACKAGE_NAME).a: $(OBJECTS)
	$(AR) rcs $@ $^
src/$(PACKAGE_NAME).cma: src/dll$(PACKAGE_NAME).so $(CMO_FILES)
	$(link_byte) -o $@ $(CMO_FILES)
src/$(PACKAGE_NAME).cmxa: src/lib$(PACKAGE_NAME).a $(CMX_FILES)
	$(link_native) -o $@ $(CMX_FILES)


# (Un)Install ##################################################################

install: build
	$(OF_INSTALL) $(PACKAGE_NAME) $(INSTALL_FILES) META -dll $(DLL_FILE)
uninstall:
	$(OF_REMOVE) $(PACKAGE_NAME)


# Clean up #####################################################################

clean:
	$(RM) */*.o */*.[aos] */*.cm[aoxit] */*.cmti */*.cmxa */*.annot */*.so \
 */a.out .depend


# Testing ######################################################################

$(TESTS_OPT): %.opt: %.ml build
	$(compile_test_native) $< -o $@

$(TESTS_BYTE): %.byte: %.ml build
	$(compile_test_byte) $< -o $@

test: test.byte

ifneq ($(OCAMLOPT),)
test: test.opt
endif

test.byte: $(TESTS_BYTE)
	@cd tests; for testfile in *.byte; do\
	  if ./$$testfile ; then\
	    echo "test '$${testfile}' passed";\
	  else\
	    echo "test '$${testfile}' failed";\
	  fi;\
	done

test.opt: $(TESTS_OPT)
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

.PHONY: all build install uninstall clean test debug test.opt test.byte
