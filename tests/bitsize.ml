let () =
  Yices2.init ();

  let bv1 = Yices2.bvconst_zero 5 in
  let i64 = Yices2.int64_ 64L in
  assert (Yices2.term_is_bitvector bv1);
  assert (Pervasives.not (Yices2.term_is_bitvector i64));
  
  assert (5 = (Yices2.term_bitsize bv1));
  (
    try
      let _ = Yices2.term_bitsize i64 in
      assert false
    with Yices2.YicesError (msg, r) ->
      assert (r.Yices2.code == 24)
  );
  Yices2.exit ()
