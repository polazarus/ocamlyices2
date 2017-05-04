open Yices2
open OUnit2

let () =
run_test_tt_main (
"yices2">:::[
  "error_correclty_reset">::(Error_correclty_reset.error_correclty_reset);
  "bitsize">::(Bitsize.bitsize);
  "checkoverflow">::(Checkoverflow.checkoverflow);
  "forall">::(Forall.forall);
  "pp">::(fun _ -> (OUnit2.skip_if (Sys.os_type = "Win32")
          "'print' functions won't work on windows (unix-only)"; Pp.pp ()));
  "type_consistency">::(Type_consistency.type_consistency);
  "z_nocrash">::(Z_nocrash.z_nocrash);
  "z_nocrash">::(Z_nocrash.z_nocrash2);
])