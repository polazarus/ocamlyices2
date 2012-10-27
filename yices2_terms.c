#include <yices.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/threads.h>
#include <caml/custom.h>
#include <stdio.h>

#include "yices2.h"
#include "yices2_macros.h"

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

value ocamlyices_update(value v_fun, value v_args, value v_newv) {
  CAMLparam3(v_fun, v_args, v_newv);
  term_t fun, *args, newv, res;
  size_t n;

  fun = Int_val(v_fun);

  ML2C_COPY_ARRAY(v_args, n, args, term_t, Term_val);
  NOT_NEEDED_VALUE(v_args);
  if (args == (void*)0) ocamlyices_allocation_error();

  newv = Int_val(v_newv);

  res = yices_update(fun, n, args, newv);
  free(args);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_int(res));
}

OCAMLYICES_NARY_TERM(distinct, distinct)

OCAMLYICES_ARGSNBODY_TERM(forall, forall)
OCAMLYICES_ARGSNBODY_TERM(exists, exists)
OCAMLYICES_ARGSNBODY_TERM(lambda, lambda)

OCAMLYICES_NULLARY_TERM(zero, zero)

value ocamlyices_int (value v_val) {
  CAMLparam1(v_val);
  term_t res;

  intnat val = Long_val(v_val);
  if (sizeof(intnat) <= 2) {
    res = yices_int32 (val);
    if (res == NULL_TERM) ocamlyices_failure();
  } else {
    res = yices_int64 (val);
    if (res == NULL_TERM) ocamlyices_failure();
  }

  CAMLreturn(Val_term(res));
}

OCAMLYICES_TERM_FROM_INT32(int32, int32)
OCAMLYICES_TERM_FROM_INT64(int64, int64)

value ocamlyices_rational (value v_den, value v_num) {
  CAMLparam2(v_den, v_num);
  term_t res;

  intnat den = Long_val(v_den), num = Long_val(v_num);
  if (sizeof(intnat) <= 2) {
    res = yices_rational32 (den, num);
    if (res == NULL_TERM) ocamlyices_failure();
  } else {
    res = yices_rational64 (den, num);
    if (res == NULL_TERM) ocamlyices_failure();
  }

  CAMLreturn(Val_term(res));
}
value ocamlyices_rational32 (value v_den, value v_num) {
  CAMLparam2(v_den, v_num);
  term_t res;

  int32_t den = Int32_val(v_den), num = Int32_val(v_num);
  res = yices_rational32 (den, num);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_term(res));
}
value ocamlyices_rational64 (value v_den, value v_num) {
  CAMLparam2(v_den, v_num);
  term_t res;

  int32_t den = Int64_val(v_den), num = Int64_val(v_num);
  res = yices_rational64 (den, num);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_term(res));
}

OCAMLYICES_TERM_FROM_STRING(parse_rational, parse_rational)
OCAMLYICES_TERM_FROM_STRING(parse_float, parse_float)

OCAMLYICES_BINARY_TERM(add, add)
OCAMLYICES_BINARY_TERM(sub, sub)
OCAMLYICES_UNARY_TERM(neg, neg)
OCAMLYICES_BINARY_TERM(mul, mul)
OCAMLYICES_UNARY_TERM(square, square)
OCAMLYICES_TERM_FROM_TERM_UINT(power, power)
OCAMLYICES_TERM_FROM_TERM_UINT32(power, power32)

value ocamlyices_poly_int (value v_as, value v_ts) {
  CAMLparam2(v_as, v_ts);
  term_t *ts, res;
  size_t n, m;

  if (sizeof(intnat) <= 2) {
    int32_t *as;

    ML2C_COPY_ARRAY(v_as, n, as, int32_t, Long_val);
    NOT_NEEDED_VALUE(v_as);
    ML2C_COPY_ARRAY(v_ts, m, ts, term_t, Term_val);
    NOT_NEEDED_VALUE(v_ts);

    if (as == (void*)0 || ts == (void*)0) {
      free(as); free(ts);
      ocamlyices_allocation_error();
    }

    if (n != m) {
      free(as); free(ts);
      ocamlyices_bad_array_sizes_error();
    }

    res = yices_poly_int32 (n, as, ts);
    free(as);
    free(ts);
    if (res == NULL_TERM) ocamlyices_failure();

    CAMLreturn(Val_int(res));
  } else {
    int64_t *as;

    ML2C_COPY_ARRAY(v_as, n, as, int64_t, Long_val);
    NOT_NEEDED_VALUE(v_as);
    ML2C_COPY_ARRAY(v_ts, m, ts, term_t, Term_val);
    NOT_NEEDED_VALUE(v_ts);

    if (as == (void*)0 || ts == (void*)0) {
      free(as); free(ts);
      ocamlyices_allocation_error();
    }

    if (n != m) {
      free(as); free(ts);
      ocamlyices_bad_array_sizes_error();
    }

    res = yices_poly_int64 (n, as, ts);
    free(as);
    free(ts);
    if (res == NULL_TERM) ocamlyices_failure();

    CAMLreturn(Val_int(res));
  }
}
value ocamlyices_poly_int32 (value v_as, value v_ts) {
  CAMLparam2(v_as, v_ts);
  term_t *ts, res;
  size_t n, m;
  int32_t *as;

  ML2C_COPY_ARRAY(v_as, n, as, int32_t, Int32_val);
  NOT_NEEDED_VALUE(v_as);
  ML2C_COPY_ARRAY(v_ts, m, ts, term_t, Term_val);
  NOT_NEEDED_VALUE(v_ts);

  if (as == (void*)0 || ts == (void*)0) {
    free(as); free(ts);
    ocamlyices_allocation_error();
  }

  if (n != m) {
    free(as); free(ts);
    ocamlyices_bad_array_sizes_error();
  }

  res = yices_poly_int32 (n, as, ts);
  free(as);
  free(ts);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_int(res));
}
value ocamlyices_poly_int64 (value v_as, value v_ts) {
  CAMLparam2(v_as, v_ts);
  term_t *ts, res;
  size_t n, m;
  int64_t *as;

  ML2C_COPY_ARRAY(v_as, n, as, int64_t, Int64_val);
  NOT_NEEDED_VALUE(v_as);
  ML2C_COPY_ARRAY(v_ts, m, ts, term_t, Term_val);
  NOT_NEEDED_VALUE(v_ts);

  if (as == (void*)0 || ts == (void*)0) {
    free(as); free(ts);
    ocamlyices_allocation_error();
  }

  if (n != m) {
    free(as); free(ts);
    ocamlyices_bad_array_sizes_error();
  }

  res = yices_poly_int64 (n, as, ts);
  free(as);
  free(ts);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_int(res));
}

value ocamlyices_poly_rational (value v_as, value v_bs, value v_ts) {
  CAMLparam3(v_as, v_bs, v_ts);
  term_t *ts, res;
  size_t n, m, p;

  if (sizeof(intnat) <= 2) {
    int32_t *as;
    uint32_t *bs;

    ML2C_COPY_ARRAY(v_as, n, as, int32_t, Long_val);
    NOT_NEEDED_VALUE(v_as);
    ML2C_COPY_ARRAY(v_bs, m, bs, uint32_t, Long_val);
    NOT_NEEDED_VALUE(v_bs);
    ML2C_COPY_ARRAY(v_ts, p, ts, term_t, Term_val);
    NOT_NEEDED_VALUE(v_ts);

    if (as == (void*)0 || bs == (void*)0 || ts == (void*)0) {
      free(as); free(ts);
      ocamlyices_allocation_error();
    }
    if (n != m || n != p) {
      free(as); free(bs); free(ts);
      ocamlyices_bad_array_sizes_error();
    }

    res = yices_poly_rational32 (n,as,bs,ts);
    free(as); free(bs); free(ts);
  } else {
    int64_t *as;
    uint64_t *bs;

    ML2C_COPY_ARRAY(v_as, n, as, int64_t, Long_val);
    NOT_NEEDED_VALUE(v_as);
    ML2C_COPY_ARRAY(v_bs, m, bs, uint64_t, Long_val);
    NOT_NEEDED_VALUE(v_bs);
    ML2C_COPY_ARRAY(v_ts, p, ts, term_t, Term_val);
    NOT_NEEDED_VALUE(v_ts);

    if (as == (void*)0 || bs == (void*)0 || ts == (void*)0) {
      free(as); free(ts);
      ocamlyices_allocation_error();
    }
    if (n != m || n != p) {
      free(as); free(bs); free(ts);
      ocamlyices_bad_array_sizes_error();
    }

    res = yices_poly_rational64 (n,as,bs,ts);
    free(as); free(bs); free(ts);
  }

  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_term(res));
}
value ocamlyices_poly_rational32 (value v_as, value v_bs, value v_ts) {
  CAMLparam3(v_as, v_bs, v_ts);
  term_t *ts, res;
  size_t n, m, p;
  int32_t *as;
  uint32_t *bs;

  ML2C_COPY_ARRAY(v_as, n, as, int32_t, Int32_val);
  NOT_NEEDED_VALUE(v_as);
  ML2C_COPY_ARRAY(v_bs, m, bs, uint32_t, Int32_val);
  NOT_NEEDED_VALUE(v_bs);
  ML2C_COPY_ARRAY(v_ts, p, ts, term_t, Term_val);
  NOT_NEEDED_VALUE(v_ts);

  if (as == (void*)0 || bs == (void*)0 || ts == (void*)0) {
    free(as); free(ts);
    ocamlyices_allocation_error();
  }
  if (n != m || n != p) {
    free(as); free(bs); free(ts);
    ocamlyices_bad_array_sizes_error();
  }

  res = yices_poly_rational32 (n,as,bs,ts);
  free(as); free(bs); free(ts);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_term(res));
}
value ocamlyices_poly_rational64 (value v_as, value v_bs, value v_ts) {
  CAMLparam3(v_as, v_bs, v_ts);
  term_t *ts, res;
  size_t n, m, p;
  int64_t *as;
  uint64_t *bs;

  ML2C_COPY_ARRAY(v_as, n, as, int64_t, Int64_val);
  NOT_NEEDED_VALUE(v_as);
  ML2C_COPY_ARRAY(v_bs, m, bs, uint64_t, Int64_val);
  NOT_NEEDED_VALUE(v_bs);
  ML2C_COPY_ARRAY(v_ts, p, ts, term_t, Term_val);
  NOT_NEEDED_VALUE(v_ts);

  if (as == (void*)0 || bs == (void*)0 || ts == (void*)0) {
    free(as); free(ts);
    ocamlyices_allocation_error();
  }
  if (n != m || n != p) {
    free(as); free(bs); free(ts);
    ocamlyices_bad_array_sizes_error();
  }

  res = yices_poly_rational64 (n,as,bs,ts);
  free(as); free(bs); free(ts);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_term(res));
}

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

OCAMLYICES_TERM_FROM_STRING(parse_term, parse_term)
