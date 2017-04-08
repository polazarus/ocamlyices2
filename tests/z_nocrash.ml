open Yices2
open OUnit2

let z_nocrash2 _ =
  let zone = Z.one in
  let _one = Term.Int.of_z zone in
  let _one_third = Term.Ratio.of_q (Q.of_ints 1 3) in
  ()


let z_nocrash _ =
  let ctx = Context.create () in
  let var = Term.new_uninterpreted (Type.int ()) in
  let phi = Term.Arith.eq var (Term.Arith.power (Term.Int.of_int 2) 64) in
  Context.assert_formula ctx phi;
  match Context.check ctx with
  | SAT ->
    let mdl = Context.get_model ctx in
    let z = Model.get_z mdl var in
    OUnit2.assert_equal z (Z.pow (Z.of_int 2) 64)
  | _ -> OUnit2.assert_failure ""