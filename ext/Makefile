SHELL=/bin/bash
GPERF=$(shell which gperf)

ifeq ($(GPERF),)
GPERF = $(abspath bin/gperf)
endif

export GPERF

all: yices

yices: libyices.a yices.h

yices/configs: $(GPERF)
	cd yices; ./configure --prefix=$(realpath .)
libyices.a: $(GPERF) yices/configs
	$(MAKE) -C yices lib; cp yices/build/*/lib/libyices.a ./

yices.h:
	cp yices/src/include/* ./

gperf: $(GPERF)

gperf.tar.gz:
	wget -O gperf.tar.gz http://ftp.gnu.org/pub/gnu/gperf/gperf-3.0.4.tar.gz

gperf/README: gperf.tar.gz
	mkdir -p gperf; tar -xzf gperf.tar.gz -C gperf --strip-components 1; touch $@

$(GPERF): gperf/README
	cd gperf; ./configure --prefix=$(realpath .); $(MAKE); $(MAKE) install

clean:
	$(RM) -r gperf/ gperf.tar.gz lib/ share/ bin/ yices*.h libyices.a
	$(MAKE) -C yices clean

.PHONY: gperf yices
