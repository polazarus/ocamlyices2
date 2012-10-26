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
    with YicesError (msg, r) -> print_endline msg; print_int r.code; assert (r.code == 24)
  in 
  
  let v1 = Yices2.int_ 60 in
  let v2 = Yices2.int_ 62 in
  let _ = Yices2.add v1 v2 in
  Yices2.exit ()
