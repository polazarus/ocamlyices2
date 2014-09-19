open Yices2

let () =
  prerr_endline "initialized";
  let zone = Z.one in
  prerr_endline "z one";

  let _one = Term.Arith.z zone in
  let _one_third = Term.Arith.q (Q.of_ints 1 3) in
  ()
