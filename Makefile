# -*- indent-tabs-mode: true, mode: Makefile -*-

include Makefile.config

compile_byte = $(OCAMLFIND) c -package zarith -I src -c -annot -bin-annot
compile_interface = $(compile_byte)
compile_native = $(OCAMLFIND) opt -package zarith -I src -c -annot -bin-annot
compile_stubs = $(CC) $(CFLAGS) -I$(shell $(OCAMLFIND) query zarith) -c -pedantic -std=c11 -Wall -Wextra -Wconversion
#-x c++ -fno-exceptions
link_byte = $(OCAMLFIND) ocamlc -a -package zarith -linkpkg -dllib -l$(PACKAGE_NAME) -cclib '$(LIBS)'
link_native = $(OCAMLFIND) ocamlopt -a -package zarith  -cclib -l$(PACKAGE_NAME) -cclib '$(LIBS)'
link_dll = gcc -shared

# Source files
ML_SOURCE = src/yices2.ml
MLI_SOURCE = src/yices2.mli
C_SOURCE = \
  src/contexts.c \
  src/terms.c \
  src/models.c \
  src/types.c \
  src/misc.c

# Compiled files
C_OBJS = $(C_SOURCE:%.c=%.o)
ML_BYTE = $(ML_SOURCE:%.ml=%.cmo)
ML_NATIVE = $(ML_SOURCE:%.ml=%.cmx)
MLI_FILES = $(MLI_SOURCE:%.mli=%.cmi)

BUILD_FILES = src/$(PACKAGE_NAME).cma src/dll$(PACKAGE_NAME).so $(MLI_FILES)
INSTALL_FILES =  src/$(PACKAGE_NAME).cma $(MLI_SOURCE) $(MLI_FILES) $(MLI_SOURCE:%.mli=%.cmti) $(ML_SOURCE:%.ml=%.annot)
DLL_FILE = src/dll$(PACKAGE_NAME).so

ifneq ($(OCAMLOPT),)
BUILD_FILES += src/$(PACKAGE_NAME).cmxa src/lib$(PACKAGE_NAME).a
INSTALL_FILES += src/$(PACKAGE_NAME).cmxa src/$(PACKAGE_NAME).a src/lib$(PACKAGE_NAME).a

endif


all: build

debug: CFLAGS += -DDEBUG
debug: build

build: $(BUILD_FILES)

# Deps
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

# Generic compilation rules ####################################################

%.o: %.c
	$(compile_stubs) -o $@ $<
%.cmi: %.mli
	$(compile_interface) -o $@ $<
%.cmo: %.ml
	$(compile_byte) -o $@ $<
%.cmx: %.ml
	$(compile_native) -o $@ $<

# Library compilation ##########################################################

src/dll$(PACKAGE_NAME).so: $(C_OBJS)
	$(link_dll) -o $@ $^ $(LIBS)
src/lib$(PACKAGE_NAME).a: $(C_OBJS)
	$(AR) rcs $@ $^
src/$(PACKAGE_NAME).cma: src/dll$(PACKAGE_NAME).so $(ML_BYTE)
	$(link_byte) -o $@ $(ML_BYTE)
src/$(PACKAGE_NAME).cmxa: src/lib$(PACKAGE_NAME).a $(ML_NATIVE)
	$(link_native) -o $@ $(ML_NATIVE) -verbose

# (Un)Install ##################################################################
install: build
	$(OF_INSTALL) $(PACKAGE_NAME) $(INSTALL_FILES) META -dll $(DLL_FILE)
uninstall:
	$(OF_REMOVE) $(PACKAGE_NAME)

# Clean up #####################################################################
clean:
	$(RM) */*.o */*.[aos] */*.cm[aoxit] */*.cmti */*.cmxa */*.annot */*.so */a.out .depend

.PHONY: all build install uninstall clean test debug

TESTS = $(wildcard tests/*.ml)

$(TESTS:%.ml=%.opt): %.opt: %.ml
	$(OCAMLFIND) opt -package zarith -linkpkg -I src src/$(PACKAGE_NAME).cmxa $< -o $@

$(TESTS:%.ml=%.byte): %.byte: %.ml
	$(OCAMLFIND) c -package zarith -linkpkg -I src src/$(PACKAGE_NAME).cma $< -o $@

test: build $(TESTS:%.ml=%.opt) $(TESTS:%.ml=%.byte)
	@cd tests; for testfile in *.opt; do\
		if ./$$testfile ; then\
			echo "test '$${testfile%.opt}' passed";\
		else\
			echo "test '$${testfile%.opt}' failed";\
		fi;\
	done

