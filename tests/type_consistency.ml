open Yices2

let () =
  let bool = Type.bool () in
  assert (Type.is_bool bool);
  assert (not (Type.is_int bool));
  assert (not (Type.is_arithmetic bool));
  assert (not (Type.is_real bool));
  assert (not (Type.is_function bool));
  assert (not (Type.is_tuple bool));
  assert (not (Type.is_scalar bool));
  assert (not (Type.is_bitvector bool));

  let int = Type.int () in
  assert (not (Type.is_bool int));
  assert (Type.is_int int);
  assert (Type.is_arithmetic int);
  assert (not (Type.is_real int));
  assert (not (Type.is_function int));
  assert (not (Type.is_tuple int));
  assert (not (Type.is_scalar int));
  assert (not (Type.is_bitvector int));

  let real = Type.real () in
  assert (not (Type.is_bool real));
  assert (not (Type.is_int real));
  assert (Type.is_arithmetic real);
  assert (Type.is_real real);
  assert (not (Type.is_function real));
  assert (not (Type.is_tuple real));
  assert (not (Type.is_scalar real));
  assert (not (Type.is_bitvector real));

  let bv2 = Type.bitvector 2 in
  assert (not (Type.is_bool bv2));
  assert (not (Type.is_int bv2));
  assert (not (Type.is_arithmetic bv2));
  assert (not (Type.is_real bv2));
  assert (not (Type.is_function bv2));
  assert (not (Type.is_tuple bv2));
  assert (not (Type.is_scalar bv2));
  assert (Type.is_bitvector bv2)
