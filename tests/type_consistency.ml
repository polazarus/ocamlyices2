open Yices2

let type_consistency _ =
  OUnit2.skip_if true "it randomly throws a code 38 error";
  let bool = Type.bool () in
  OUnit2.assert_bool "" (Type.is_bool bool);
  OUnit2.assert_bool "" (not (Type.is_int bool));
  OUnit2.assert_bool "" (not (Type.is_arithmetic bool));
  OUnit2.assert_bool "" (not (Type.is_real bool));
  OUnit2.assert_bool "" (not (Type.is_function bool));
  OUnit2.assert_bool "" (not (Type.is_tuple bool));
  OUnit2.assert_bool "" (not (Type.is_scalar bool));
  OUnit2.assert_bool "" (not (Type.is_bitvector bool));

  let int = Type.int () in
  OUnit2.assert_bool "" (not (Type.is_bool int));
  OUnit2.assert_bool "" (Type.is_int int);
  OUnit2.assert_bool "" (Type.is_arithmetic int);
  OUnit2.assert_bool "" (not (Type.is_real int));
  OUnit2.assert_bool "" (not (Type.is_function int));
  OUnit2.assert_bool "" (not (Type.is_tuple int));
  OUnit2.assert_bool "" (not (Type.is_scalar int));
  OUnit2.assert_bool "" (not (Type.is_bitvector int));

  let real = Type.real () in
  OUnit2.assert_bool "" (not (Type.is_bool real));
  OUnit2.assert_bool "" (not (Type.is_int real));
  OUnit2.assert_bool "" (Type.is_arithmetic real);
  OUnit2.assert_bool "" (Type.is_real real);
  OUnit2.assert_bool "" (not (Type.is_function real));
  OUnit2.assert_bool "" (not (Type.is_tuple real));
  OUnit2.assert_bool "" (not (Type.is_scalar real));
  OUnit2.assert_bool "" (not (Type.is_bitvector real));

  let bv2 = Type.bitvector 2 in
  OUnit2.assert_bool "" (not (Type.is_bool bv2));
  OUnit2.assert_bool "" (not (Type.is_int bv2));
  OUnit2.assert_bool "" (not (Type.is_arithmetic bv2));
  OUnit2.assert_bool "" (not (Type.is_real bv2));
  OUnit2.assert_bool "" (not (Type.is_function bv2));
  OUnit2.assert_bool "" (not (Type.is_tuple bv2));
  OUnit2.assert_bool "" (not (Type.is_scalar bv2));
  OUnit2.assert_bool "" (Type.is_bitvector bv2)