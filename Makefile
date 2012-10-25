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
	$(OCAMLC) $<
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

$(PACKAGE_NAME).cma: yices2_stubs.o yices2.cmo
	$(OCAMLMKLIB) -o $(PACKAGE_NAME) $^ -lyices
$(PACKAGE_NAME).cmxa: yices2_stubs.o yices2.cmx
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

.PHONY: all build install uninstall clean
