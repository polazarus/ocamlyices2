open Yices2

let has_long_in = (max_int lsl 34) != 0

let mk_ctx () =
  let config = Context.Config.create () in
  Context.create ~config ()

let () =
  let ctx = mk_ctx () in
  let int_typ = Type.int () in
  let var = Term.new_uninterpreted int_typ in
  let value = Term.Arith.power (Term.Arith.of_int 2) (if has_long_in then 62 else 30) in
  let formula = Term.Arith.eq var value in
  Context.assert_formula ctx formula;
  let _status = Context.check ctx in
  let model = Context.get_model ctx in

  try
    let _ = Context.Model.get_int model var in assert false
  with Failure _ ->
    ()

