Ocamlyices2: Yices 2 SMT solver binding for OCaml
=================================================
Mickaël Delahaye, 2012

BEWARE: highly unstable API.

Requirements
------------

* Yices 2 (installed)
* GMP Header (libgmp-dev)
* OCaml 3.12 or later
* Findlib


Build and install
-----------------

    $ ./configure
    $ make
    $ sudo make install

TODO
----

* Mechanism to select working implementation of pretty-printing functions
  (fopencookie or pthread-based), autoconf seems a bit heavy-handed just for a
  single variable
* Cleanup API name convention
* Split into modules?
* Add some examples

License
-------

Copyright (c) 2012, Mickaël Delahaye.

Permission to use, copy, modify, and/or distribute this software for any purpose
with or without fee is hereby granted, provided that the above copyright notice
and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.
