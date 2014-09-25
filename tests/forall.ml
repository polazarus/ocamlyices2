open Yices2

let () =
  let int = Type.int () in
  let input = Term.new_uninterpreted int in
  Term.set_name input "x";
  let body var =
    Term.Arith.eq0 (Term.Arith.mul var input)
  in
  let term = Term.Bool.forall int body in
  ()

  (* quantifier are not yet supported for anything else :( *)

