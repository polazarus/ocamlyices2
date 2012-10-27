PACKAGE_NAME=ocamlyices2

OCAMLC=ocamlfind c
OCAMLOPT=ocamlfind opt
OCAMLMKLIB=ocamlmklib
OF_INSTALL=ocamlfind install
OF_REMOVE=ocamlfind remove


all: build

build: $(PACKAGE_NAME).cma $(PACKAGE_NAME).cmxa

# Generic compilation rules ####################################################

%.o: %.c
	$(OCAMLC) $<  -ccopt -Wall
%.cmi: %.mli
	$(OCAMLC) -c $<
%.cmo: %.ml
	$(OCAMLC) -c $<
%.cmx: %.ml
	$(OCAMLOPT) -c $<

# Dependencies #################################################################
.depend:
	ocamlfind dep *.ml *.mli > $@

-include .depend

# Library compilation ##########################################################

SOURCES=yices2_contexts.c yices2_models.c yices2_types.c yices2_terms.c yices2_utils.c yices2_misc.c

$(PACKAGE_NAME).cma: $(SOURCES:.c=.o) yices2.cmo
	$(OCAMLMKLIB) -o $(PACKAGE_NAME) $^ -lyices
$(PACKAGE_NAME).cmxa: $(SOURCES:.c=.o) yices2.cmx
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
	rm -f *.[ao] *.cm[aoxi] *.so a.out .depend

.PHONY: all build install uninstall clean test

test: 
	@cd tests; for testfile in *.ml; do\
		if ocaml -I .. ocamlyices2.cma $$testfile ; then\
			echo "test '$${testfile%.ml}' passed";\
		else\
			echo "test '$${testfile%.ml}' failed";\
		fi;\
	done
