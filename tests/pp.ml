let prstr f arg =
  let res = ref "" in
  f (fun x -> res := !res ^ x) arg;
  !res

let new_input name typ =
  let term = Yices2.new_uninterpreted_term typ in
    Yices2.set_term_name term name;
    term

let () =
  Yices2.init ();

  let bv1 = Yices2.bvconst_zero 5 in
  let i64 = Yices2.int64_ 64L in

  let bv_string = prstr Yices2.pp_term bv1 in
  assert (bv_string = "0b00000\n\n");

  let i64_string = prstr Yices2.pp_term i64 in
  assert (i64_string = "64\n\n");

  let bool_typ = Yices2.bool_type () in
  let v1 = new_input "v1" bool_typ
  and v2 = new_input "v2" bool_typ
  and v3 = new_input "v3" bool_typ in

  let form = Yices2.or_ v3 (Yices2.and_ v1 v2) in
  let form_string = prstr Yices2.pp_term form in
  assert (form_string = "(or v3 (and v1 v2))\n\n");
  
  let typ1 = Yices2.bv_type 1 in
  let typ1_string = prstr Yices2.pp_type typ1 in
  assert (typ1_string = "(bitvector 1)\n\n");
  
  Yices2.exit ();
