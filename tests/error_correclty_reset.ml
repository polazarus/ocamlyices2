open Yices2

(* This error was created because of a bug: after an error thrown using the
   YicesError exception, any function after it would also raise the same
   error even though no error actually happened. 
   This has been fixed by reseting the error state right before the exception
   is raised. *)
let error_correclty_reset _ =
  try let _ = Term.bitsize (Term.Int.of_int64 64L)
    in OUnit2.assert_failure "an error should have been raised"
  with YicesError (_, _) -> ();
  Type.is_int (Type.bool ());
  ()