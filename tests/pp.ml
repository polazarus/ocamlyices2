open Yices2

let prstr f arg =
  let res = ref "" in
  f (fun x -> res := !res ^ x) arg;
  String.trim !res

let new_input name typ =
  let term = Term.new_uninterpreted typ in
    Term.set_name term name;
    term

let () =
  let bv1 = Term.Bitvector.zero 5 in
  let i64 = Term.Arith.of_int64 64L in

  let bv_string = prstr Term.print bv1 in
  assert (bv_string = "0b00000");

  let i64_string = prstr Term.print i64 in
  assert (i64_string = "64");

  let bool_typ = Type.bool () in
  let v1 = new_input "v1" bool_typ
  and v2 = new_input "v2" bool_typ
  and v3 = new_input "v3" bool_typ in

  let form = Term.or2 v3 (Term.and2 v1 v2) in
  let form_string = prstr Term.print form in
  assert (form_string = "(or v3 (and v1 v2))");

  let typ1 = Type.bitvector 1 in
  let typ1_string = prstr Type.print typ1 in
  assert (typ1_string = "(bitvector 1)");
