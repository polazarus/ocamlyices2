DEBIAN_PACKAGES="ocaml-nox ocaml-native-compilers opam libgmp-dev"

if [ -z "$NO_GPERF" ]
then
DEBIAN_PACKAGES="$DEBIAN_PACKAGES gperf"
fi

case "$OCAML_VERSION" in
3|3.12|3.12.1) ppa=avsm/ocaml312+opam11 ;;
4.00|4.00.1) ppa=avsm/ocaml40+opam11 ;;
4.01|4.01.0) ppa=avsm/ocaml41+opam11 ;;
4|4.02|4.02.0) ppa=avsm/ocaml42+opam11 ;;
*) echo Unknown OCaml version: $OCAML_VERSION; exit 1 ;;
esac

sudo add-apt-repository -y ppa:$ppa
sudo apt-get update -qq
sudo apt-get install -qq $DEBIAN_PACKAGES

opam init -y
eval `opam config env`
opam install -y ocamlfind zarith
