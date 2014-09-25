open Yices2;;

let () =
  let bv1 = Term.Bitvector.zero 5 in
  let i64 = Term.Int.of_int64 64L in

  assert (Term.is_bitvector bv1);
  assert (not (Term.is_bitvector i64));

  assert (5 = (Term.bitsize bv1));
  assert (5 = (Type.bitsize (Type.of_term bv1)));

  begin
    try
      let _ = Term.bitsize i64 in
      assert false
    with YicesError (Error.BITVECTOR_REQUIRED, _) ->
      ()
    |  YicesError (_, r) ->
      prerr_endline r.Error.name;
      assert false
  end;

   begin
    try
      let _ = Type.bitsize (Type.of_term i64) in
      assert false
    with YicesError (code, r) ->
      assert (r.Error.name = "BVTYPE_REQUIRED")
  end
