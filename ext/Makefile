SHELL=/bin/bash

all: yices

yices: libyices.a yices.h

yices/make.include:
	cd yices && ./configure --prefix=$(realpath .)
libyices.a: yices/make.include
	$(MAKE) lib -j -C yices; cp yices/build/*/lib/libyices.a ./

yices.h:
	cp yices/src/include/* ./

clean:
	$(RM) -rf lib/ share/ bin/
	$(MAKE) -C yices clean

.PHONY: yices
