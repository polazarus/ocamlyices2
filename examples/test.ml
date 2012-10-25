let () = print_endline Yices2.version;
  print_endline Yices2.build_arch;
  print_endline Yices2.build_mode;
  print_endline Yices2.build_date


let () =
  Yices2.init ();
  let _ = Yices2.true_ () in
  let v1 = Yices2.int_ 60 in
  let v2 = Yices2.int_ 62 in
  let _ = Yices2.add v1 v2 in
  Yices2.exit ()
