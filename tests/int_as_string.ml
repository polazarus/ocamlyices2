let () =
  Yices2.init ();
  
  let ctx = Yices2.new_context () in
  let int_typ = Yices2.int_type () in
  let real_typ = Yices2.real_type () in

  let i1 = Yices2.int_ 55 in
  let i2 = Yices2.parse_rational "123467890123456789/3" in
  
  let v1 = Yices2.new_uninterpreted_term int_typ in
  let v2 = Yices2.new_uninterpreted_term real_typ in
  Yices2.set_term_name v1 "v1";
  Yices2.set_term_name v2 "v2";

  let f1 = Yices2.arith_eq v1 i1 in

  let mul = Yices2.mul i1 i2 in

  let f2 = Yices2.arith_eq v2 mul in

  Yices2.assert_formula ctx f1;
  Yices2.assert_formula ctx f2;

  let status = Yices2.check ctx in
  assert (status = Yices2.SAT);
  let model = Yices2.get_model ctx in
  let () =
    try
      let m = Yices2.get_int_value_as_string model v1 in
        assert (m = "55");
      let p = Yices2.get_rational_value_as_string model v2 in
        assert (p = "123467890123456789");
    with _ ->
      ()
  in

  Yices2.exit ()
