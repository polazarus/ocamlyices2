open Yices2

let () =
  print_endline version;
  print_endline build_arch;
  print_endline build_mode;
  print_endline build_date

let () =
  Yices2.init ();

  let _ = true_ () in
  let _ = false_ () in
  let _ = int_ 1 in
  let _ = int32_ 32l in
  let i64 = int64_ 64L in
  let bv1 = bvconst_zero 5 in
  assert (term_is_bitvector bv1);
  assert (Pervasives.not (term_is_bitvector i64));
  
  assert (5 = (term_bitsize bv1));
  let () =
    try
      let _ = term_bitsize i64 in
      assert false
    with YicesError (msg, r) ->
      print_endline msg; print_int r.code;
      assert (r.code == 24)
  in 
  
  let bool_typ = bool_type () in
  let var1 = new_uninterpreted_term bool_typ in
  let var2 = new_uninterpreted_term bool_typ in
  let formula = and2 (implies var1 var2) var1 in
  let cfg = new_config () in
  let ctx = new_context cfg in
  assert_formula ctx formula;
  let status = check ctx in
  let model = get_model ctx in
  Printf.printf "v1 %B,  v2 %B\n" (get_bool_value model var1) (get_bool_value model var2);
  free_model model;
  (* let _ = get_bool_value model var1 in*)
  free_context ctx;
  
  print_endline "**";
  
  let ctx = new_context cfg in
  let int_typ = int_type () in
  let var = new_uninterpreted_term int_typ in
  let formula = arith_eq var (power (int_ 2) 62) in
  assert_formula ctx formula;
  let status = check ctx in
  let model = get_model ctx in
  Printf.printf "v %d\n" (get_int_value model var);
  (* free_model model;
  free_context ctx; *)
  
  let v1 = Yices2.int_ 60 in
  let v2 = Yices2.int_ 62 in
  let _ = Yices2.add v1 v2 in
  Yices2.exit ()
