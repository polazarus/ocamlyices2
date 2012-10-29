let has_long_in = (max_int lsl 34) != 0

let () =
  Yices2.init ();
  
  let config = Yices2.new_config () in
  let ctx = Yices2.new_context ~config () in

  let int_typ = Yices2.int_type () in
  let var = Yices2.new_uninterpreted_term int_typ in
  let formula = Yices2.arith_eq var (Yices2.power (Yices2.int_ 2) (if has_long_in then 62 else 30)) in
  Yices2.assert_formula ctx formula;
  let _status = Yices2.check ctx in
  let model = Yices2.get_model ctx in
  
  try
    let _ = Yices2.get_int_value model var in assert false
  with _ ->
    Yices2.exit ()


