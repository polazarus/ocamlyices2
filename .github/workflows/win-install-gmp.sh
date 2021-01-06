#! /bin/sh
set -e
set -x

HOST=x86_64-w64-mingw32

# We build a static version libgmp.a for our build of ocamlyices2.
# But as zarith (opam) also needs gmp, we can install it in /usr so
# that both benefit from this build.
# Note: for downloading, I switched from https://gmplib.org/download/ to a
# mirror of the GNU fundation.
curl -L -O https://gmplib.org/download/gmp/gmp-6.2.1.tar.bz2
curl -L -O https://gmplib.org/download/gmp/gmp-6.2.1.tar.bz2.sig
#gpg --keyserver wwwkeys.pgp.net --recv-keys 28C67298
#gpg --verify gmp-6.2.1.tar.bz2.sig gmp-6.2.1.tar.bz2
tar -jxf gmp-6.2.1.tar.bz2
cd gmp-6.2.1
./configure --prefix=/usr/${HOST}/sys-root/mingw --host=${HOST} --disable-shared --enable-static CC=${HOST}-gcc
make
make install
cd ..

DIST=gmp-6.2.1-static-${HOST}
mkdir ${DIST}
cp /usr/${HOST}/sys-root/mingw/lib/libgmp.a /usr/${HOST}/sys-root/mingw/include/gmp.h ${DIST}
tar czf ${DIST}.tar.gz $DIST
echo "$(sha256sum ${DIST}.tar.gz)" >${DIST}.tar.gz.sha256
