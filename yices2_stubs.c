#include <yices.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/threads.h>
#include <caml/custom.h>
#include <stdio.h>

#include "yices2_macros.h"

#define COND_MT_START(cond) do { if (cond) caml_release_runtime_system(); } while (0)
#define COND_MT_END(cond) do { if (cond) caml_acquire_runtime_system(); } while (0)

#define NOT_NEEDED_VALUE(v) do {v = 0;} while(0)

int mt_level = 0;

value ocamlyices_info(value unit) {
  CAMLparam1 (unit);
  CAMLlocal1 (tuple);
  tuple = caml_alloc_tuple(5);
  Store_field(tuple, 0, caml_copy_string(yices_version));
  Store_field(tuple, 1, caml_copy_string(yices_build_arch));
  Store_field(tuple, 2, caml_copy_string(yices_build_mode));
  Store_field(tuple, 3, caml_copy_string(yices_build_date));
  CAMLreturn (tuple);
}

value ocamlyices_init(value unit) {
  CAMLparam1 (unit);
  yices_init();
  CAMLreturn (unit);
}

value ocamlyices_exit(value unit) {
  CAMLparam1 (unit);
  COND_MT_START(mt_level > 1);
  yices_exit();
  COND_MT_END(mt_level > 1);
  caml_acquire_runtime_system();
  CAMLreturn (unit);
}

value ocamlyices_reset(value unit) {
  CAMLparam1 (unit);
  COND_MT_START(mt_level > 1);
  yices_reset();
  COND_MT_START(mt_level > 1);
  CAMLreturn (unit);
}

char error_message[256];

value term_option(term_t t) {
  CAMLparam0();
  CAMLlocal1(res);
  if (t == NULL_TERM) {
    res = Val_int(0);
  } else {
    res = caml_alloc(1, 0);
    Store_field(res, 1, Val_int(t));
  }
  CAMLreturn(res);
}

value type_option(type_t t) {
  CAMLparam0();
  CAMLlocal1(res);
  if (t == NULL_TERM) {
    res = Val_int(0);
  } else {
    res = caml_alloc(1, 0);
    Store_field(res, 1, Val_int(t));
  }
  CAMLreturn(res);
}

void ocamlyices_check_failure() {
  CAMLparam0();
  CAMLlocal2(arg1,arg2);
  value args[2];
  error_code_t ec = yices_error_code();
  if (ec == NO_ERROR) return;
  error_report_t * report = yices_error_report();
  snprintf(error_message, 256, "Yices Error #%d", ec);
  arg1 = caml_copy_string(error_message);
  arg2 = caml_alloc_tuple(8);
  Store_field(arg2, 0, Val_int(ec));
  Store_field(arg2, 1, Val_int(report->column));
  Store_field(arg2, 2, Val_int(report->line));
  Store_field(arg2, 3, term_option(report->term1));
  Store_field(arg2, 4, type_option(report->type1));
  Store_field(arg2, 5, term_option(report->term2));
  Store_field(arg2, 6, type_option(report->type2));
  Store_field(arg2, 7, caml_copy_int64(report->badval));
  args[0] = arg1;
  args[1] = arg2;
  caml_raise_with_args(*caml_named_value("ocamlyices.exception"), 2, args);
  caml_failwith(error_message);
  CAMLreturn0;
}
void ocamlyices_failure() {
  ocamlyices_check_failure();
  caml_failwith("Unknown error");
}

OCAMLYICES_NULLARY_TYPE(bool_type, bool_type)
OCAMLYICES_NULLARY_TYPE(int_type, int_type)
OCAMLYICES_NULLARY_TYPE(real_type, real_type)
OCAMLYICES_TYPE_FROM_INT(bv_type, bv_type)
OCAMLYICES_TYPE_FROM_INT(new_scalar_type, new_scalar_type)
OCAMLYICES_NULLARY_TYPE(new_uninterpreted_type, new_uninterpreted_type)
OCAMLYICES_NARY_TYPE(tuple_type, tuple_type)
OCAMLYICES_TYPE_FROM_TYPEs_TYPE(function_type, function_type)


OCAMLYICES_NULLARY_TERM(true, true)
OCAMLYICES_NULLARY_TERM(false, false)
OCAMLYICES_TERM_FROM_TYPE_INT(constant, constant)
OCAMLYICES_TERM_FROM_TYPE(new_uninterpreted_term, new_uninterpreted_term)
OCAMLYICES_TERM_FROM_TYPE(new_variable, new_variable)

OCAMLYICES_TERM_FROM_TERM_TERMs(application, application)
OCAMLYICES_TERNARY_TERM(ite, ite)
OCAMLYICES_BINARY_TERM(eq, eq)
OCAMLYICES_BINARY_TERM(neq, neq)

OCAMLYICES_UNARY_TERM(not, not)
OCAMLYICES_NARY_TERM(and, and)
OCAMLYICES_NARY_TERM(or, or)
OCAMLYICES_NARY_TERM(xor, xor)

OCAMLYICES_BINARY_TERM(and2, and2)
OCAMLYICES_BINARY_TERM(or2, or2)
OCAMLYICES_BINARY_TERM(xor2, xor2)

OCAMLYICES_TERNARY_TERM(and3, and3)
OCAMLYICES_TERNARY_TERM(or3, or3)
OCAMLYICES_TERNARY_TERM(xor3, xor3)
OCAMLYICES_BINARY_TERM(iff, iff)
OCAMLYICES_BINARY_TERM(implies, implies)

OCAMLYICES_NARY_TERM(tuple, tuple)
OCAMLYICES_TERM_FROM_UINT_TERM(select, select)
OCAMLYICES_TERM_FROM_TERM_UINT_TERM(tuple_update, tuple_update)

OCAMLYICES_TERM_FROM_TERM_TERMs_TERM(update, update)

OCAMLYICES_NARY_TERM(distinct, distinct)

OCAMLYICES_ARGSNBODY_TERM(forall, forall)
OCAMLYICES_ARGSNBODY_TERM(exists, exists)
OCAMLYICES_ARGSNBODY_TERM(lambda, lambda)

OCAMLYICES_NULLARY_TERM(zero, zero)

OCAMLYICES_TERM_FROM_INT(int32, int)
OCAMLYICES_TERM_FROM_INT32(int32, int32)
OCAMLYICES_TERM_FROM_INT64(int64, int64)

OCAMLYICES_TERM_FROM_X_X(rational32, rational, int32_t, Int_val)
OCAMLYICES_TERM_FROM_X_X(rational32, rational32, int32_t, Int32_val)
OCAMLYICES_TERM_FROM_X_X(rational64, rational64, int64_t, Int64_val)

OCAMLYICES_TERM_FROM_STRING(parse_rational, parse_rational)
OCAMLYICES_TERM_FROM_STRING(parse_float, parse_float)

OCAMLYICES_BINARY_TERM(add, add)
OCAMLYICES_BINARY_TERM(sub, sub)
OCAMLYICES_UNARY_TERM(neg, neg)
OCAMLYICES_BINARY_TERM(mul, mul)
OCAMLYICES_UNARY_TERM(square, square)
OCAMLYICES_TERM_FROM_TERM_UINT(power, power)
OCAMLYICES_TERM_FROM_TERM_UINT32(power, power32)

OCAMLYICES_TERM_FROM_Xs_TERMs(poly_int32, poly_int, int32_t, Int_val)
OCAMLYICES_TERM_FROM_Xs_TERMs(poly_int32, poly_int32, int32_t, Int32_val)
OCAMLYICES_TERM_FROM_Xs_TERMs(poly_int64, poly_int64, int64_t, Int64_val)

OCAMLYICES_TERM_FROM_Xs_Xs_TERMs(poly_rational32, poly_rational, int32_t, Int_val)
OCAMLYICES_TERM_FROM_Xs_Xs_TERMs(poly_rational32, poly_rational32, int32_t, Int32_val)
OCAMLYICES_TERM_FROM_Xs_Xs_TERMs(poly_rational64, poly_rational64, int64_t, Int64_val)

OCAMLYICES_BINARY_TERM(arith_eq_atom, arith_eq)
OCAMLYICES_BINARY_TERM(arith_neq_atom, arith_neq)
OCAMLYICES_BINARY_TERM(arith_geq_atom, arith_geq)
OCAMLYICES_BINARY_TERM(arith_leq_atom, arith_leq)
OCAMLYICES_BINARY_TERM(arith_gt_atom, arith_gt)
OCAMLYICES_BINARY_TERM(arith_lt_atom, arith_lt)

OCAMLYICES_UNARY_TERM(arith_eq0_atom, arith_eq0)
OCAMLYICES_UNARY_TERM(arith_neq0_atom, arith_neq0)
OCAMLYICES_UNARY_TERM(arith_geq0_atom, arith_geq0)
OCAMLYICES_UNARY_TERM(arith_leq0_atom, arith_leq0)
OCAMLYICES_UNARY_TERM(arith_gt0_atom, arith_gt0)
OCAMLYICES_UNARY_TERM(arith_lt0_atom, arith_lt0)

OCAMLYICES_TERM_FROM_UINT_X(bvconst_uint32, bvconst_int, uint32_t, Int_val)
OCAMLYICES_TERM_FROM_UINT_X(bvconst_uint32, bvconst_int32, uint32_t, Int32_val)
OCAMLYICES_TERM_FROM_UINT_X(bvconst_uint64, bvconst_int64, uint64_t, Int64_val)

OCAMLYICES_TERM_FROM_INT(bvconst_zero, bvconst_zero)
OCAMLYICES_TERM_FROM_INT(bvconst_one, bvconst_one)
OCAMLYICES_TERM_FROM_INT(bvconst_minus_one, bvconst_minus_one)

OCAMLYICES_TERM_FROM_INTs(bvconst_from_array, bvconst_from_array)
OCAMLYICES_TERM_FROM_STRING(parse_bvbin, parse_bvbin)
OCAMLYICES_TERM_FROM_STRING(parse_bvhex, parse_bvhex)

OCAMLYICES_BINARY_TERM(bvadd, bvadd)
OCAMLYICES_BINARY_TERM(bvsub, bvsub)
OCAMLYICES_UNARY_TERM(bvneg, bvneg)
OCAMLYICES_BINARY_TERM(bvmul, bvmul)
OCAMLYICES_UNARY_TERM(bvsquare, bvsquare)

OCAMLYICES_TERM_FROM_TERM_UINT(bvpower, bvpower)
OCAMLYICES_TERM_FROM_TERM_UINT32(bvpower,bvpower32)

OCAMLYICES_BINARY_TERM(bvdiv, bvdiv)
OCAMLYICES_BINARY_TERM(bvrem, bvrem)
OCAMLYICES_BINARY_TERM(bvsdiv, bvsdiv)
OCAMLYICES_BINARY_TERM(bvsrem, bvsrem)
OCAMLYICES_BINARY_TERM(bvsmod, bvsmod)

OCAMLYICES_UNARY_TERM(bvnot, bvnot)
OCAMLYICES_BINARY_TERM(bvand, bvand)
OCAMLYICES_BINARY_TERM(bvor, bvor)
OCAMLYICES_BINARY_TERM(bvxor, bvxor)
OCAMLYICES_BINARY_TERM(bvnand, bvnand)
OCAMLYICES_BINARY_TERM(bvnor, bvnor)
OCAMLYICES_BINARY_TERM(bvxnor, bvxnor)
OCAMLYICES_BINARY_TERM(bvshl, bvshl)
OCAMLYICES_BINARY_TERM(bvlshr, bvlshr)
OCAMLYICES_BINARY_TERM(bvashr, bvashr)

OCAMLYICES_TERM_FROM_TERM_UINT(shift_left0, shift_left0)
OCAMLYICES_TERM_FROM_TERM_UINT(shift_left1, shift_left1)
OCAMLYICES_TERM_FROM_TERM_UINT(shift_right0, shift_right0)
OCAMLYICES_TERM_FROM_TERM_UINT(shift_right1, shift_right1)
OCAMLYICES_TERM_FROM_TERM_UINT(ashift_right, ashift_right)
OCAMLYICES_TERM_FROM_TERM_UINT(rotate_left, rotate_left)
OCAMLYICES_TERM_FROM_TERM_UINT(rotate_right, rotate_right)

OCAMLYICES_TERM_FROM_TERM_INT_INT(bvextract, bvextract)

OCAMLYICES_BINARY_TERM(bvconcat, bvconcat)

OCAMLYICES_TERM_FROM_TERM_UINT(bvrepeat, bvrepeat)
OCAMLYICES_TERM_FROM_TERM_UINT(sign_extend, sign_extend)
OCAMLYICES_TERM_FROM_TERM_UINT(zero_extend, zero_extend)

OCAMLYICES_UNARY_TERM(redand, redand)
OCAMLYICES_UNARY_TERM(redor, redor)
OCAMLYICES_BINARY_TERM(redcomp, redcomp)

OCAMLYICES_NARY_TERM(bvarray, bvarray)
OCAMLYICES_TERM_FROM_TERM_UINT(bitextract, bitextract)

OCAMLYICES_BINARY_TERM(bveq_atom, bveq)
OCAMLYICES_BINARY_TERM(bvneq_atom, bvneq)
OCAMLYICES_BINARY_TERM(bvge_atom, bvge)
OCAMLYICES_BINARY_TERM(bvgt_atom, bvgt)
OCAMLYICES_BINARY_TERM(bvle_atom, bvle)
OCAMLYICES_BINARY_TERM(bvlt_atom, bvlt)
OCAMLYICES_BINARY_TERM(bvsge_atom, bvsge)
OCAMLYICES_BINARY_TERM(bvsgt_atom, bvsgt)
OCAMLYICES_BINARY_TERM(bvsle_atom, bvsle)
OCAMLYICES_BINARY_TERM(bvslt_atom, bvslt)

OCAMLYICES_TYPE_FROM_STRING(parse_type, parse_type)
OCAMLYICES_TERM_FROM_STRING(parse_term, parse_term)


OCAMLYICES_TYPE_FROM_TERM(type_of_term, type_of_term)

OCAMLYICES_BOOL_FROM_TERM(term_is_bool, term_is_bool)
OCAMLYICES_BOOL_FROM_TERM(term_is_int, term_is_int)
OCAMLYICES_BOOL_FROM_TERM(term_is_real, term_is_real)
OCAMLYICES_BOOL_FROM_TERM(term_is_arithmetic, term_is_arithmetic)
OCAMLYICES_BOOL_FROM_TERM(term_is_bitvector, term_is_bitvector)
OCAMLYICES_BOOL_FROM_TERM(term_is_tuple, term_is_tuple)
OCAMLYICES_BOOL_FROM_TERM(term_is_function, term_is_function)
OCAMLYICES_BOOL_FROM_TERM(term_is_scalar, term_is_scalar)

OCAMLYICES_INT_FROM_TERM(term_bitsize, term_bitsize)


