#if [ -e skip ]; then exit 0; fi
eval `opam config env`
./configure
make
make install
make test

cd examples
make
./test.byte && ./test.opt