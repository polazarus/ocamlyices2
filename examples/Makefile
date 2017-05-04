EXAMPLES=test
OC=ocamlfind c -package yices2 -linkpkg
OO=ocamlfind opt -package yices2 -linkpkg

all: byte native

byte: $(EXAMPLES:=.byte)

native: $(EXAMPLES:=.opt)

%.byte:%.ml
	$(OC) $< -o $@

%.opt:%.ml
	$(OO) $< -o $@

clean:
	$(RM) *.byte *.opt *.cm[oxti] *.o

.PHONY: all byte native clean
