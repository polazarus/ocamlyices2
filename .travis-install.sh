case "$TRAVIS_OS_NAME" in
linux)
  # Install Anil's PPAs
  case "$OCAML_VERSION" in
  3|3.12|3.12.1) ppa=avsm/ocaml312+opam11 ;;
  4.00|4.00.1) ppa=avsm/ocaml40+opam11 ;;
  4.01|4.01.0) ppa=avsm/ocaml41+opam11 ;;
  4|4.02|4.02.0) ppa=avsm/ocaml42+opam11 ;;
  *) echo Unknown OCaml version: $OCAML_VERSION; exit 1 ;;
  esac

  sudo add-apt-repository -y ppa:$ppa
  sudo apt-get update -qq
  sudo apt-get install -qq ocaml-nox ocaml-native-compilers opam
;;
osx)
  if [ -z "$MAC_TOO" ]; then
    touch skip
    exit 0
  fi
  brew update
  brew install ocaml opam
;;
esac

opam init
eval `opam config env`
opam install ocamlfind zarith
