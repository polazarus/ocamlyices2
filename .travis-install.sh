case "$TRAVIS_OS_NAME" in
  linux)
    case "$OCAML_VERSION" in
      3|3.12|3.12.1) ppa=avsm/ocaml312+opam12 ;;
      4.00|4.00.1) ppa=avsm/ocaml40+opam12 ;;
      4.01|4.01.0) ppa=avsm/ocaml41+opam12 ;;
      4|4.02|4.02.0) ppa=avsm/ocaml42+opam12 ;;
      *) echo Unknown OCaml version: $OCAML_VERSION; exit 1 ;;
    esac
    sudo add-apt-repository -y ppa:$ppa
    sudo apt-get update -qq
    # if NOGPERF is not defined, ${NOGPERF:+gperf} will expand to nothing
    sudo apt-get install -qq ocaml-nox ocaml-native-compilers opam libgmp-dev ${NOGPERF:+gperf}
  ;;
  osx)
    brew update > /dev/null
    brew install opam # 'gmp' and 'coreutils' are already installed on travis
  ;;
esac

opam init -y
eval `opam config env`
opam install -y ocamlfind zarith ounit
