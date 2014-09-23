Ocamlyices2: Yices 2 SMT solver binding for OCaml, version 0.1.0
================================================================
Mickaël Delahaye, 2014

[![Build Status](https://travis-ci.org/polazarus/ocamlyices2.svg?branch=master)](https://travis-ci.org/polazarus/ocamlyices2)

BEWARE: highly unstable API, see the to-do list.

[Yices 2][1] is an efficient SMT solver developed at SRI International.
Ocamlyices2 lets you use this SMT solver inside your own program in OCaml.

**N.B:** Yices is only available freely for *non-commercial use*. See Yices'
[license terms][2]. So, although Ocamlyices2's own code (the binding) is freely
available, Ocamlyices2 is only usable under the same restrictions.

You might also be interested in [Ocamlyices][3], a binding for Yices 1 SMT solver.

To do
-----

* Cleanup API name convention (mostly done)
* Add some examples
* Documentation

Requirements
------------

* [OCaml][4] 3.12 or later

* [Findlib][5] (`ocamlfind`), also available through [OPAM][6]

* [Zarith OCaml library][7], also available through [OPAM][6]

* GMP with header file (`gmp.h`), needed to handle big integer and rational
  values (as `Z.t` and `Q.t` from Zarith), available on most systems (for
  instance, in the package `libgmp-dev` on Debian and Ubuntu).


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

When linking (i.e, actually making an executable), add the `-linkpkg` flag.

Without, well… it depends of your actual setup of OCaml. To compile your
program, add the directory of Ocamlyices2 and of Zarith in the
search path with the flag `-I` of `ocamlc`/`ocamlopt` (run
`ocamlfind query ocamlyices2` to find out). And when linking, add the library
`ocamlyices2.cma`/`.cmxa` to the command.


License
-------

Copyright (c) 2014, [Mickaël Delahaye][8].

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
[2]: LICENSE_yices
[3]: https://github.com/polazarus/ocamlyices
[4]: http://ocaml.org
[5]: http://projects.camlcity.org/projects/findlib.html
[6]: http://opam.ocaml.org
[7]: https://forge.ocamlcore.org/projects/zarith
[8]: http://micdel.fr
