#if [ -e skip ]; then exit 0; fi

HOST=i686-w64-mingw32

# We build a static version libgmp.a for our build of ocamlyices2.
# But as zarith (opam) also needs gmp, we can install it in /usr so
# that both benefit from this build.
# Note: for downloading, I switched from https://gmplib.org/download/ to a
# mirror of the GNU fundation.
curl -L -O http://gnu.uberglobalmirror.com/gmp/gmp-6.1.2.tar.bz2
curl -L -O http://gnu.uberglobalmirror.com/gmp/gmp-6.1.2.tar.bz2.sig
#gpg --keyserver wwwkeys.pgp.net --recv-keys 28C67298
#gpg --verify gmp-6.1.2.tar.bz2.sig gmp-6.1.2.tar.bz2
tar -jxf gmp-6.1.2.tar.bz2
cd gmp-6.1.2
./configure --prefix=/usr/${HOST}/sys-root/mingw --host=${HOST} --disable-shared --enable-static CC=${HOST}-gcc
make
make install
cd ..

# Now, install the mingw-version of opam and install things needed
curl -O https://dl.dropboxusercontent.com/s/eo4igttab8ipyle/opam32.tar.xz
tar -xf opam32.tar.xz
bash opam32/install.sh
opam init -y -a mingw https://github.com/fdopen/opam-repository-mingw.git --comp 4.02.3+mingw32c --switch 4.02.3+mingw32c
eval `opam config env`
CFLAGS="-I/usr/${HOST}/sys-root/mingw/include -L/usr/${HOST}/sys-root/mingw/lib/" \
    opam install -y ocamlfind zarith ounit

./configure --host=${HOST} --with-static-gmp=/usr/${HOST}/sys-root/mingw/lib/libgmp.a
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
DIST=ocamlyices2-$(git describe --tags)-${HOST}
mv dist $DIST
tar czf ${DIST}.tar.gz $DIST
echo "$(sha256sum ${DIST}.tar.gz)" > ${DIST}.tar.gz.sha256

DIST=gmp-6.1.2-static-${HOST}
mkdir ${DIST}
cp /usr/${HOST}/sys-root/mingw/lib/libgmp.a /usr/${HOST}/sys-root/mingw/include/gmp.h ${DIST}
tar czf ${DIST}.tar.gz $DIST
echo "$(sha256sum ${DIST}.tar.gz)" > ${DIST}.tar.gz.sha256