Ocamlyices2: Yices 2 SMT solver binding for OCaml, version 0.1.0
================================================================
Mickaël Delahaye, 2014

BEWARE: highly unstable API, see the to-do list.

[Yices 2][1] is an efficient SMT solver developed at SRI International.
Ocamlyices2 lets you use this SMT solver inside your own program in OCaml.

You might also be interested in [Ocamlyices][2], a binding for Yices 1 SMT solver.

To do
-----

* Cleanup API name convention
* Split into modules?
* Add some examples
* Documentation

Requirements
------------

* Yices 2 (installed)
  After downloading the tarball from their website, you can use:

        ./install-yices.sh yicesXYZ.tar.gz

  to install yices in `/usr/local` and
  register the DLL. You can change destination directories with parameters:

        ./install-yices yices.tar.gz /usr/local /usr/local/lib64

* OCaml 3.12 or later

* Findlib

* GMP header file (gmp.h), needed to extract big integer and rational values
  from models (as strings), available on most systems (for instance, in the
  package libgmp-dev on Debian and Ubuntu)


Build and install
-----------------

    $ ./configure
    $ make

Build the Ocamlyices2 library (for ocamlopt and ocamlc).

    $ sudo make install

Install the library on the system using Findlib.

Usage
-----

With Ocamlfind:

    ocamlfind ocamlc/ocamlopt -package ocamlyices2 …

Or without:

    ocamlc -I +ocamlyices2 ocamlyices2.cma …
    ocamlopt -I +ocamlyices2 ocamlyices2.cmxa …

License
-------

Copyright (c) 2014, [Mickaël Delahaye][3].

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


[1]: http://yices.csl.sri.com/
[2]: https://github.com/polazarus/ocamlyices
[3]: http://micdel.fr
