SHELL=/bin/bash

all: yices

yices: libyices.a yices.h

yices/configs:
	cd yices; ./configure --prefix=$(realpath .)
libyices.a: $(GPERF) yices/configs
	$(MAKE) -C yices lib; cp yices/build/*/lib/libyices.a ./

yices.h:
	cp yices/src/include/* ./

clean:
	$(RM) -rf lib/ share/ bin/
	$(MAKE) -C yices clean

.PHONY: yices
