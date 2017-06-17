Ocamlyices2: Yices 2 SMT solver binding for OCaml
=================================================
Mickaël Delahaye, 2014

|      Linux, Mac      |         Windows         |
| :------------------: | :---------------------: |
| [![trv-img]][travis] | [![apvy-img]][appveyor] |

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

Build the Ocamlyices2 library (for ocamlopt and ocamlc):

    ./configure
    make

Install the library on the system using Findlib:

    make install

By default, a static version of libgmp will be used. To force the use of the
shared gmp library for zarith and libyices, you can use:

    ./configure --with-shared-gmp

You can also force the use of your own static version of libgmp with

    ./configure --with-static-gmp=libgmp.a

For cross-compiling the library for native windows (using the mingw compiler)
from cygwin, use the `--host=` argument:

    ./configure --host=i686-w64-mingw32

Tested platforms
----------------
The `opam install yices2` has been tested on many OCaml versions (from 3.12.1
to 4.04.0) and on these systems:

* Ubuntu 16.04, 16.10, 12.04
* Debian Jessie (stable)
* MacOS 10.12.4
* Alpine 3.5
* OpenSUSE 42.2
* Fedora 24
* Arch Linux

On some platforms, the only way is to use `./configure && make`. It has
been tested on:

* Cygwin+mingw32 and mingw32-gmp (see cross-compilation above)

Binaries
--------
Yices2 may be somewhat painful to build. To ease the process, you can simply
pick the binaries. These are linked to static gmp libraries to avoid any
shared library problem. For example, for windows, you would do:

    curl -L https://github.com/maelvalais/ocamlyices2/releases/download/v0.0.2/ocamlyices2-v0.0.2-i686-w64-mingw32.tar.gz | tar xz
    ocamlfind install yices2 ocamlyices2-*/*

Usage
-----

With Ocamlfind:

    ocamlfind ocamlc/ocamlopt -package yices2 …

When linking (i.e, actually making an executable), add the `-linkpkg` flag.

Without, well… it depends of your actual setup of OCaml. To compile your
program, add the directory of Ocamlyices2 and of Zarith in the
search path with the flag `-I` of `ocamlc`/`ocamlopt` (run
`ocamlfind query yices2` to find out). And when linking, add the library
`yices2.cma`/`.cmxa` to the command.


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
[travis]: https://travis-ci.org/maelvalais/ocamlyices2
[trv-img]: https://travis-ci.org/maelvalais/ocamlyices2.svg?branch=master
[appveyor]: https://ci.appveyor.com/project/maelvalais/ocamlyices2/branch/master
[apvy-img]: https://ci.appveyor.com/api/projects/status/94xwq5jn37q6hcn6/branch/master?svg=true