if [ -e skip ]; exit 0
eval `opam config env`
./configure
make
make install
make test
