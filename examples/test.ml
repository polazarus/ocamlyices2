open Yices2

let () =
  print_endline version;
  print_endline build_arch;
  print_endline build_mode;
  print_endline build_date

let () =
  let _ = Term.true_ () in
  let _ = Term.false_ () in
  let _ = Term.Arith.of_int 1 in
  let _ = Term.Arith.of_int32 32l in
  let i64 = Term.Arith.of_int64 64L in
  let bv1 = Term.Bitvector.zero 5 in
  assert (Term.is_bitvector bv1);
  assert (not (Term.is_bitvector i64));

  assert (5 = (Term.bitsize bv1));
  let () =
    try
      let _ = Term.bitsize i64 in
      assert false
    with YicesError (code, r) ->
      print_endline r.Error.name;
      assert (code == Error.BITVECTOR_REQUIRED)
  in 

  let bool_typ = Type.bool () in
  let var1 = Term.new_uninterpreted bool_typ in
  let var2 = Term.new_uninterpreted bool_typ in
  let formula = Term.and2 (Term.implies var1 var2) var1 in
  begin
    let ctx = Context.create () in
    Context.assert_formula ctx formula;
    let status = Context.check ctx in
    let model = Context.get_model ctx in
    Printf.printf "v1 %B,  v2 %B\n" (Model.get_bool model var1) (Model.get_bool model var2);
    (* let _ = get_bool_value model var1 in*)
  end;

  print_endline "**";

  let ctx = Context.create () in
  let int_typ = Type.int () in
  let var = Term.new_uninterpreted int_typ in
  let formula = Term.Arith.eq var (Term.Arith.power (Term.Arith.of_int 2) 62) in
  Context.assert_formula ctx formula;
  let status = Context.check ctx in
  let model = Context.get_model ctx in
  Printf.printf "v %Ld\n" (Model.get_int64 model var);
  Printf.printf "v %a\n" Z.output (Model.get_z model var);
