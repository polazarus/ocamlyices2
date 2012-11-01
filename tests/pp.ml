let () =
  Yices2.init ();

  let bv1 = Yices2.bvconst_zero 5 in
  let i64 = Yices2.int64_ 64L in

  let bv_string = ref "" in
  let i64_string = ref "" in
  let collect_string r arg =
    r := !r ^ arg
  in
  Yices2.pp_term (collect_string bv_string) bv1;
  assert (!bv_string = "0b00000\n\n");
  Yices2.pp_term (collect_string i64_string) i64;
  assert (!i64_string = "64\n\n");
  let form_string = ref "" in
  let v1 = Yices2.new_uninterpreted_term (Yices2.bool_type ()) in
  let v2 = Yices2.new_uninterpreted_term (Yices2.bool_type ()) in
  Yices2.set_term_name v1 "v1";
  Yices2.set_term_name v2 "v2";
  Yices2.pp_term (collect_string form_string) (Yices2.or_ (Yices2.and_ v1 v2) (Yices2.or_ v1 (Yices2.not v2)));
  assert (!form_string = "(or (=> v2 v1) (and v1 v2))\n\n")
