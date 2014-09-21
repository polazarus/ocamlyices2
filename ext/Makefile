SHELL=/bin/bash


all: yices

yices: gperf libyices.a yices.h

yices/configs:
	cd yices; PATH="$(realpath bin):$$PATH" ./configure --prefix=$(realpath .)
libyices.a: yices/configs
	export PATH="$(realpath bin):$$PATH"; $(MAKE) -C yices lib; cp yices/build/*/lib/libyices.a ./

yices.h:
	cp yices/src/include/* ./



gperf.tar.gz:
	wget -O gperf.tar.gz http://ftp.gnu.org/pub/gnu/gperf/gperf-3.0.4.tar.gz

gperf/README: gperf.tar.gz
	mkdir -p gperf; tar -xzf gperf.tar.gz -C gperf --strip-components 1; touch $@

bin/gperf: gperf/README
	cd gperf; ./configure --prefix=$(realpath .); $(MAKE); $(MAKE) install

gperf: bin/gperf

clean:
	$(RM) -r gperf/ gperf.tar.gz lib/ share/ bin/
	$(MAKE) -C yices clean

.PHONY: gperf yices
