open Yices2

let () =
  let zone = Z.one in
  let _one = Term.Arith.z zone in
  let _one_third = Term.Arith.q (Q.of_ints 1 3) in
  ()


let () =
  let ctx = Context.create () in
  let var = Term.new_uninterpreted (Type.int ()) in
  let phi = Term.Arith.eq var (Term.Arith.power (Term.Arith.of_int 2) 64) in
  Context.assert_formula ctx phi;
  match Context.check ctx with
  | SAT ->
    let mdl = Context.get_model ctx in
    let z = Context.Model.get_z mdl var in
    assert (z = Z.pow (Z.of_int 2) 64)
  | _ -> assert false


