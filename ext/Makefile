SHELL=/bin/bash

all: lib/libyices.a

CONFIGURE_ARGS = --prefix=$(realpath .) \
	--host=$(HOST) --disable-shared --enable-static \
	$(if $(filter yes,$(FORCE_SHARED_GMP)),--with-shared-gmp) \
	$(if $(STATIC_GMP),--with-static-gmp=$(STATIC_GMP))

yices/make.include:
	cd yices && ./configure $(CONFIGURE_ARGS)

lib/libyices.a: yices/make.include
	make lib -C yices -j$$(nproc 2>/dev/null || echo 4)
	make install -C yices

clean:
	rm -rf lib/ share/ bin/ include/
	make -C yices distclean

.PHONY: yices
