# -*- indent-tabs-mode: true -*-

PACKAGE_NAME=ocamlyices2

OCAMLC=ocamlfind c
OCAMLOPT=ocamlfind opt
OCAMLMKLIB=ocamlmklib
OF_INSTALL=ocamlfind install
OF_REMOVE=ocamlfind remove
CFLAGS=-Wall -O2

C_STUBS= ocamlyices2_all.c

#	ocamlyices2_contexts.c \
#	ocamlyices2_models.c \
#	ocamlyices2_types.c \
#	ocamlyices2_terms.c \
#	ocamlyices2_utils.c \
#	ocamlyices2_misc.c

all: build

build: $(PACKAGE_NAME).cma $(PACKAGE_NAME).cmxa

# Generic compilation rules ####################################################

%.o: %.c
	$(OCAMLC) $< $(CFLAGS:%=-ccopt %)
	objcopy -w -L ocamlyices_internal* -X $@
%.cmi: %.mli
	$(OCAMLC) -c $<
%.cmo: %.ml
	$(OCAMLC) -c $<
%.cmx: %.ml
	$(OCAMLOPT) -c $<

# Dependencies #################################################################
.depend:
	ocamlfind dep *.ml *.mli > $@

# Library compilation ##########################################################

$(PACKAGE_NAME).cma: $(C_STUBS:.c=.o) yices2.cmo
	$(OCAMLMKLIB) -o $(PACKAGE_NAME) $^ -lyices
$(PACKAGE_NAME).cmxa: $(C_STUBS:.c=.o) yices2.cmx
	$(OCAMLMKLIB) -o $(PACKAGE_NAME) $^ -lyices

# (Un)Install ##################################################################
install: build
	$(OF_INSTALL) $(PACKAGE_NAME) $(PACKAGE_NAME).cma $(PACKAGE_NAME).cmxa \
		lib$(PACKAGE_NAME).a $(PACKAGE_NAME).a \
		yices2.cmi META -dll dll$(PACKAGE_NAME).so
uninstall:
	$(OF_REMOVE) $(PACKAGE_NAME)

# Clean up #####################################################################
clean:
	rm -f *.[aos] *.cm[aoxi] *.so a.out .depend

.PHONY: all build install uninstall clean test

test: 
	@cd tests; for testfile in *.ml; do\
		if ocaml -I .. ocamlyices2.cma $$testfile ; then\
			echo "test '$${testfile%.ml}' passed";\
		else\
			echo "test '$${testfile%.ml}' failed";\
		fi;\
	done


-include .depend

ocamlyices2_all.o: ocamlyices2_contexts.c ocamlyices2_terms.c ocamlyices2_models.c ocamlyices2_types.c ocamlyices2_misc.c
ocamlyices2_all.o ocamlyices2_contexts.o ocamlyices2_terms.o ocamlyices2_models.o ocamlyices2_types.o ocamlyices2_misc.o: ocamlyices2.h
ocamlyices2_all.o ocamlyices2_terms.o: ocamlyices2_terms_macros.h


# vim:noet:
