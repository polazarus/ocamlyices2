#ifndef __OY_TERMS_H__
#define __OY_TERMS_H__

#ifdef HAVE_GMP_H
#include <gmp.h> // should be included before yices and zarith
#define __GMP_H__
#define __GMP_H
#endif

#include <yices.h>
#include <caml/mlvalues.h>

CAMLprim value ocamlyices_term_true (value unit);
CAMLprim value ocamlyices_term_false (value unit);
CAMLprim value ocamlyices_term_constant (value, value);
CAMLprim value ocamlyices_term_new_uninterpreted (value vtyp);
CAMLprim value ocamlyices_term_new_variable (value);
CAMLprim value ocamlyices_term_application (value fn, value args);
CAMLprim value ocamlyices_term_ite (value, value, value);
CAMLprim value ocamlyices_term_eq (value, value);
CAMLprim value ocamlyices_term_neq (value, value);
CAMLprim value ocamlyices_term_not (value);
CAMLprim value ocamlyices_term_andn (value);
CAMLprim value ocamlyices_term_orn (value);
CAMLprim value ocamlyices_term_xorn (value);
CAMLprim value ocamlyices_term_and2 (value, value);
CAMLprim value ocamlyices_term_or2 (value, value);
CAMLprim value ocamlyices_term_xor2 (value, value);
CAMLprim value ocamlyices_term_and3 (value, value, value);
CAMLprim value ocamlyices_term_or3 (value, value, value);
CAMLprim value ocamlyices_term_xor3 (value, value, value);
CAMLprim value ocamlyices_term_iff (value, value);
CAMLprim value ocamlyices_term_implies (value, value);
CAMLprim value ocamlyices_term_tuple (value args);
CAMLprim value ocamlyices_term_select (value index, value tuple);
CAMLprim value ocamlyices_term_tuple_update (value tuple, value index,
    value newv);
CAMLprim value ocamlyices_term_update (value fun, value args, value newv);
CAMLprim value ocamlyices_term_distinct (value args);
CAMLprim value ocamlyices_term_forall (value arg, value body);
CAMLprim value ocamlyices_term_exists (value arg, value body);
CAMLprim value ocamlyices_term_lambda (value arg, value body);
CAMLprim value ocamlyices_term_foralln (value arg, value body);
CAMLprim value ocamlyices_term_existsn (value arg, value body);
CAMLprim value ocamlyices_term_lambdan (value arg, value body);

CAMLprim value ocamlyices_term_zero (value unit);
CAMLprim value ocamlyices_term_int (value val);
CAMLprim value ocamlyices_term_int32 (value val);
CAMLprim value ocamlyices_term_int64 (value val);
CAMLprim value ocamlyices_term_nativeint (value val);
CAMLprim value ocamlyices_term_rational_int (value den, value num);
CAMLprim value ocamlyices_term_rational_int32 (value den, value num);
CAMLprim value ocamlyices_term_rational_int64 (value den, value num);
CAMLprim value ocamlyices_term_rational_nativeint (value den, value num);
CAMLprim value ocamlyices_term_z (value z);
CAMLprim value ocamlyices_term_q (value q);
CAMLprim value ocamlyices_term_parse_rational (value arg);
CAMLprim value ocamlyices_term_parse_float (value arg);
CAMLprim value ocamlyices_term_add (value arg1, value arg2);
CAMLprim value ocamlyices_term_addn (value args);
CAMLprim value ocamlyices_term_sub (value arg1, value arg2);
CAMLprim value ocamlyices_term_neg (value arg);
CAMLprim value ocamlyices_term_mul (value arg1, value arg2);
CAMLprim value ocamlyices_term_muln (value args);
CAMLprim value ocamlyices_term_square (value arg);
CAMLprim value ocamlyices_term_power_int (value t, value d);
CAMLprim value ocamlyices_term_power_int64 (value t, value d);
CAMLprim value ocamlyices_term_div (value a, value b);
CAMLprim value ocamlyices_term_poly_int (value as, value ts);
CAMLprim value ocamlyices_term_poly_nativeint (value as, value ts);
CAMLprim value ocamlyices_term_poly_int32 (value as, value ts);
CAMLprim value ocamlyices_term_poly_int64 (value as, value ts);
CAMLprim value ocamlyices_term_poly_rational_int (value nums, value dens,
    value ts);
CAMLprim value ocamlyices_term_poly_rational_nativeint (value nums, value dens,
    value ts);
CAMLprim value ocamlyices_term_poly_rational_int32 (value nums, value dens,
    value ts);
CAMLprim value ocamlyices_term_poly_rational_int64 (value nums, value dens,
    value ts);
CAMLprim value ocamlyices_term_poly_z (value zs, value ts);
CAMLprim value ocamlyices_term_poly_q (value qs, value ts);
CAMLprim value ocamlyices_term_arith_eq (value arg1, value arg2);
CAMLprim value ocamlyices_term_arith_neq (value arg1, value arg2);
CAMLprim value ocamlyices_term_arith_geq (value arg1, value arg2);
CAMLprim value ocamlyices_term_arith_leq (value arg1, value arg2);
CAMLprim value ocamlyices_term_arith_gt (value arg1, value arg2);
CAMLprim value ocamlyices_term_arith_lt (value arg1, value arg2);
CAMLprim value ocamlyices_term_arith_eq0 (value arg);
CAMLprim value ocamlyices_term_arith_neq0 (value arg);
CAMLprim value ocamlyices_term_arith_geq0 (value arg);
CAMLprim value ocamlyices_term_arith_leq0 (value arg);
CAMLprim value ocamlyices_term_arith_gt0 (value arg);
CAMLprim value ocamlyices_term_arith_lt0 (value arg);

CAMLprim value ocamlyices_term_bitvector_of_int (value vn, value vval);
CAMLprim value ocamlyices_term_bitvector_of_nativeint (value vn, value vval);
CAMLprim value ocamlyices_term_bitvector_of_int32 (value vn, value vval);
CAMLprim value ocamlyices_term_bitvector_of_int64 (value vn, value vval);
CAMLprim value ocamlyices_term_bitvector_of_z (value vn, value vval);
CAMLprim value ocamlyices_term_bitvector_zero (value n);
CAMLprim value ocamlyices_term_bitvector_one (value n);
CAMLprim value ocamlyices_term_bitvector_minus_one (value n);
CAMLprim value ocamlyices_term_bitvector_of_bools (value arr);
CAMLprim value ocamlyices_term_bitvector_of_bin (value arg);
CAMLprim value ocamlyices_term_bitvector_of_hex (value arg);
CAMLprim value ocamlyices_term_bitvector_add (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_sub (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_neg (value arg);
CAMLprim value ocamlyices_term_bitvector_mul (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_square (value arg);
CAMLprim value ocamlyices_term_bitvector_power_int (value bv, value degree);
CAMLprim value ocamlyices_term_bitvector_power_int64 (value bv, value degree);
CAMLprim value ocamlyices_term_bitvector_div (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_rem (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_sdiv (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_srem (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_smod (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_not (value arg);
CAMLprim value ocamlyices_term_bitvector_and (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_or (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_xor (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_nand (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_nor (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_xnor (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_shl (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_lshr (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_ashr (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_shift_left0 (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_shift_left1 (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_shift_right0 (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_shift_right1 (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_ashift_right (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_rotate_left (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_rotate_right (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_extract (value t, value i1, value i2);
CAMLprim value ocamlyices_term_bitvector_concat (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_repeat (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_sign_extend (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_zero_extend (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_redand (value arg);
CAMLprim value ocamlyices_term_bitvector_redor (value arg);
CAMLprim value ocamlyices_term_bitvector_redcomp (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_of_terms (value args);
CAMLprim value ocamlyices_term_bitvector_bitextract (value bv, value index);
CAMLprim value ocamlyices_term_bitvector_eq (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_neq (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_ge (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_gt (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_le (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_lt (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_sge (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_sgt (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_sle (value arg1, value arg2);
CAMLprim value ocamlyices_term_bitvector_slt (value arg1, value arg2);

CAMLprim value ocamlyices_term_subst (value vars, value repls, value t);
CAMLprim value ocamlyices_term_subst_array (value vars, value repls, value ts);
CAMLprim value ocamlyices_term_set_name (value t, value name);
CAMLprim value ocamlyices_term_remove_name (value name);
CAMLprim value ocamlyices_term_clear_name (value t);
CAMLprim value ocamlyices_term_get_name (value t);
CAMLprim value ocamlyices_term_by_name (value name);
CAMLprim value ocamlyices_term_bitsize (value t);
CAMLprim value ocamlyices_term_parse (value str);
CAMLprim value ocamlyices_term_is_bool (value t);
CAMLprim value ocamlyices_term_is_int (value t);
CAMLprim value ocamlyices_term_is_real (value t);
CAMLprim value ocamlyices_term_is_arithmetic (value t);
CAMLprim value ocamlyices_term_is_bitvector (value t);
CAMLprim value ocamlyices_term_is_tuple (value t);
CAMLprim value ocamlyices_term_is_function (value t);
CAMLprim value ocamlyices_term_is_scalar (value t);
CAMLprim value ocamlyices_term_is_ground (value t);

CAMLprim value ocamlyices_term_print (value width_opt, value height_opt,
                                      value offset_opt, value cb, value t);

static inline term_t Term_val (value v) {
  return (term_t)Long_val(v);
}

static inline value Val_term (term_t term) {
  return Val_long((intnat)term);
}

static inline term_t *_oy_terms_from_values (value v_arr, uint32_t n) {
  term_t *arr;
  uint32_t i;

  arr = (term_t *) malloc(sizeof(term_t[n]));
  if (arr) {
    for (i = 0; i < n; i++) {
      arr[i] = Term_val(Field(v_arr, i));
    }
  }

  return arr;
}

#endif
