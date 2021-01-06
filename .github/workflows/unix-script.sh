#! /bin/sh
set -e
set -x

HOST=$(./autoconf/config.guess)

case "$(uname -s)" in
Linux)
  sudo apt-get install -qq libgmp-dev
  ;;
Darwin)
  brew install gmp coreutils # 'gmp' and 'coreutils' are already installed on travis
  ;;
esac

opam install -y ocamlfind zarith ounit

eval $(opam config env)
./configure
make
# We must uninstall first in case any previous build installed but failed before
# uninstalling.
make uninstall
make install
make test

cd examples
make
./test.byte && ./test.opt
cd ..

make uninstall

make dist
DIST=ocamlyices2-$(git describe --tags --always)-${HOST}
mv dist $DIST
tar czf ${DIST}.tar.gz $DIST
# If SHA256SUM is not set, ${} will return sha256sum.
echo "$(${SHA256SUM:-sha256sum} ${DIST}.tar.gz)" >${DIST}.tar.gz.sha256
