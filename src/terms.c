#include "config.h"

#include <stdlib.h> // for malloc, free
#include <stdint.h> // for (u)int32_t etc.

#ifdef HAVE_GMP_H
#include <gmp.h> // should be included before yices and zarith
#define __GMP_H__
#define __GMP_H
#endif

#include <yices.h>
#include <zarith.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>

#include "ocamlyices2.h"
#include "terms_macros.h"



CAMLprim value ocamlyices_term_true(UNUSED value unit) {
  _oy_check_init();
  return _oy__nullary(&yices_true);
}
CAMLprim value ocamlyices_term_false(UNUSED value unit) {
  _oy_check_init();
  return _oy__nullary(&yices_false);
}

CAMLprim value ocamlyices_term_constant(value vtyp, value vval) {
  // Hyp: type and term values are non-block
  intnat val = Long_val(vval);

  if (sizeof(intnat) > sizeof(int32_t) && val > INT32_MAX) {
    _oy__invalid_argument("index too large");
  }

  term_t res = yices_constant(Type_val(vtyp), (int32_t)val);
  if (res == NULL_TERM) {
    _oy__error();
  }

  return Val_term(res);
}

CAMLprim value ocamlyices_term_new_uninterpreted(value vtyp) {
  // Hyp: type values are non-block
  term_t res = yices_new_uninterpreted_term(Type_val(vtyp));
  return Val_term(res);
}

CAMLprim value ocamlyices_term_new_variable(value vtyp) {
  // Hyp: type values are non-block
  term_t res = yices_new_variable(Type_val(vtyp));
  return Val_term(res);
}

CAMLprim value ocamlyices_term_application(value v_fun, value v_args) {
  // Hyp: term values are non-block
  CAMLparam1(v_args);
  term_t res, *args;
  uint32_t n;

  n = check_Wosize_val(v_args);
  args = _oy__terms_from_values(v_args, n);
  if (args == NULL) {
    _oy__allocation_error();
  }

  res = yices_application(Term_val(v_fun), n, args);
  free(args);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_int(res));
}

CAMLprim value ocamlyices_term_ite(value arg1, value arg2, value arg3) {
  return _oy__ternary(&yices_ite, arg1, arg2, arg3);
}

CAMLprim value ocamlyices_term_eq(value arg1, value arg2) {
  return _oy__binary(&yices_eq, arg1, arg2);
}

CAMLprim value ocamlyices_term_neq(value arg1, value arg2) {
  return _oy__binary(&yices_neq, arg1, arg2);
}

CAMLprim value ocamlyices_term_not(value arg) {
  return _oy__unary(&yices_not, arg);
}

CAMLprim value ocamlyices_term_andn(value args) {
  return _oy__nary(&yices_and, args);
}
CAMLprim value ocamlyices_term_orn(value args) {
  return _oy__nary(&yices_or, args);
}
CAMLprim value ocamlyices_term_xorn(value args) {
  return _oy__nary(&yices_xor, args);
}

CAMLprim value ocamlyices_term_and2(value arg1, value arg2) {
  return _oy__binary(&yices_and2, arg1, arg2);
}
CAMLprim value ocamlyices_term_or2(value arg1, value arg2) {
  return _oy__binary(&yices_or2, arg1, arg2);
}
CAMLprim value ocamlyices_term_xor2(value arg1, value arg2) {
  return _oy__binary(&yices_xor2, arg1, arg2);
}

CAMLprim value ocamlyices_term_and3(value arg1, value arg2, value arg3) {
  return _oy__ternary(&yices_and3, arg1, arg2, arg3);
}
CAMLprim value ocamlyices_term_or3(value arg1, value arg2, value arg3) {
  return _oy__ternary(&yices_or3, arg1, arg2, arg3);
}
CAMLprim value ocamlyices_term_xor3(value arg1, value arg2, value arg3) {
  return _oy__ternary(&yices_xor3, arg1, arg2, arg3);
}

CAMLprim value ocamlyices_term_iff(value arg1, value arg2) {
  return _oy__binary(&yices_iff, arg1, arg2);
}

CAMLprim value ocamlyices_term_implies(value arg1, value arg2) {
  return _oy__binary(&yices_implies, arg1, arg2);
}

CAMLprim value ocamlyices_term_tuple(value args) {
  return _oy__nary_const(&yices_tuple, args);
}

static inline term_t _oy_term_select_invargs(term_t t, uint32_t i) {
  return yices_select(i, t);
}

CAMLprim value ocamlyices_term_select(value v_index, value v_tuple) {
  return _oy_term_of_term_index(&_oy_term_select_invargs, v_tuple, v_index);
}

CAMLprim value ocamlyices_term_tuple_update(value v_tuple, value v_index,
    value v_newv) {
  // Hyp: term values are non-block
  intnat index;

  index = Long_val(v_index);
  if (index < 0) {
    _oy__invalid_argument("negative index");
  }
  if (sizeof(intnat) > sizeof(uint32_t) && index > UINT32_MAX) {
    _oy__invalid_argument("index too large");
  }

  term_t res = yices_tuple_update(Term_val(v_tuple), (uint32_t)index,
                                  Term_val(v_newv));
  if (res == NULL_TERM) {
    _oy__error();
  }

  return Val_term(res);
}

CAMLprim value ocamlyices_term_update (value v_fun, value v_args,
                                       value v_newv) {
  // Hyp: term values are non-block
  CAMLparam1(v_args);
  term_t *args, res;
  uint32_t largs;

  largs = check_Wosize_val(v_args);
  args = _oy__terms_from_values(v_args, largs);
  if (args == NULL) {
    _oy__allocation_error();
  }

  res = yices_update(Term_val(v_fun), largs, args, Term_val(v_newv));
  free(args);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_distinct (value args) {
  return _oy__nary(&yices_distinct, args);
}

CAMLprim value ocamlyices_term_forall (value arg, value body) {
  return _oy__unary_quantifier(&yices_forall, arg, body);
}

CAMLprim value ocamlyices_term_exists (value arg, value body) {
  return _oy__unary_quantifier(&yices_exists, arg, body);
}

CAMLprim value ocamlyices_term_lambda (value v_arg, value v_body) {
  // Hyp: term values are non-block

  term_t arg = Term_val(v_arg);
  term_t res = yices_lambda(1, &arg, Term_val(v_body));
  if (res == NULL_TERM) {
    _oy__error();
  }

  return Val_term(res);
}

CAMLprim value ocamlyices_term_foralln (value args, value body) {
  return _oy__nary_quantifier(&yices_forall, args, body);
}

CAMLprim value ocamlyices_term_existsn (value args, value body) {
  return _oy__nary_quantifier(&yices_exists, args, body);
}

CAMLprim value ocamlyices_term_lambdan (value v_args, value v_body) {
  // Hyp: term values are non-block
  CAMLparam1(v_args);
  term_t res, *args;
  uint32_t n;

  n = check_Wosize_val(v_args);
  args = _oy__terms_from_values(v_args, n);
  if (args == NULL) {
    _oy__allocation_error();
  }

  res = yices_lambda(n, args, Term_val(v_body));
  free(args);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}


CAMLprim value ocamlyices_term_zero (UNUSED value unit) {
  _oy_check_init();
  return _oy__nullary(&yices_zero);
}

CAMLprim value ocamlyices_term_int (value v_val) {
  term_t res;
  intnat val;
  _oy_check_init();

  val = Long_val(v_val);
  if (sizeof(intnat) == sizeof(int32_t)) {
    res = yices_int32 ((int32_t)val);
    if (res == NULL_TERM) {
      _oy__error();
    }
  } else {
    res = yices_int64 ((int64_t)val);
    if (res == NULL_TERM) {
      _oy__error();
    }
  }

  return Val_term(res);
}

CAMLprim value ocamlyices_term_int32 (value v_val) {
  CAMLparam1(v_val);
  term_t res;
  _oy_check_init();

  res = yices_int32 (Int32_val(v_val));
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_int64 (value v_val) {
  CAMLparam1(v_val);
  term_t res;
  _oy_check_init();

  res = yices_int64 (Int64_val(v_val));
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_nativeint (value v_val) {
  CAMLparam1(v_val);
  term_t res;
  _oy_check_init();

  intnat val = Nativeint_val(v_val);
  if (sizeof(intnat) == sizeof(int32_t)) {
    res = yices_int32 ((int32_t)val);
    if (res == NULL_TERM) {
      _oy__error();
    }
  } else {
    res = yices_int64 ((int64_t)val);
    if (res == NULL_TERM) {
      _oy__error();
    }
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_rational_int (value v_den, value v_num) {
  term_t res;
  _oy_check_init();

  intnat den = Long_val(v_den), num = Long_val(v_num);
  if (num < 0) {
    _oy__invalid_argument("negative numerator");
  }
  if (sizeof(intnat) == sizeof(int32_t)) {
    res = yices_rational32 ((int32_t)den, (uint32_t)num);
    if (res == NULL_TERM) {
      _oy__error();
    }
  } else {
    res = yices_rational64 ((int64_t)den, (uint64_t)num);
    if (res == NULL_TERM) {
      _oy__error();
    }
  }

  return res;
}

CAMLprim value ocamlyices_term_rational_int32 (value v_den, value v_num) {
  CAMLparam2(v_den, v_num);
  term_t res;
  _oy_check_init();

  int32_t den = Int32_val(v_den), num = Int32_val(v_num);
  if (num < 0) {
    _oy__invalid_argument("negative numerator");
  }
  res = yices_rational32 (den, (uint32_t)num);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_rational_int64 (value v_den, value v_num) {
  CAMLparam2(v_den, v_num);
  term_t res;
  _oy_check_init();

  int64_t den = Int64_val(v_den), num = Int64_val(v_num);
  if (num < 0) {
    _oy__invalid_argument("negative numerator");
  }
  res = yices_rational64 (den, (uint64_t)num);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_rational_nativeint (value v_den, value v_num) {
  CAMLparam2(v_den, v_num);
  term_t res;
  _oy_check_init();

  intnat den = Nativeint_val(v_den), num = Nativeint_val(v_num);
  if (num < 0) {
    _oy__invalid_argument("negative numerator");
  }
  if (sizeof(intnat) == sizeof(int32_t)) {
    res = yices_rational32 ((int32_t)den, (uint32_t)num);
    if (res == NULL_TERM) {
      _oy__error();
    }
  } else {
    res = yices_rational64 ((int64_t)den, (uint64_t)num);
    if (res == NULL_TERM) {
      _oy__error();
    }
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_z(value v_z) {
  CAMLparam1(v_z);
  term_t res;
  mpz_t z;
  _oy_check_init();

  ml_z_mpz_init_set_z(z, v_z);
  res = yices_mpz(z);
  mpz_clear(z);

  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_q(value v_q) {
  CAMLparam1(v_q);
  term_t res;
  mpz_t tmp;
  mpq_t q;
  _oy_check_init();

  mpq_init(q);
  ml_z_mpz_init_set_z(tmp, Field(v_q, 0));
  mpq_set_num(q, tmp);
  ml_z_mpz_set_z(tmp, Field(v_q, 1));
  mpq_set_den(q, tmp);
  mpz_clear(tmp);

  res = yices_mpq(q);
  mpq_clear(q);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_parse_rational(value v_arg) {
  return _oy__parser(&yices_parse_rational, v_arg);
}

CAMLprim value ocamlyices_term_parse_float(value v_arg) {
  return _oy__parser(&yices_parse_float, v_arg);
}

CAMLprim value ocamlyices_term_add(value arg1, value arg2) {
  return _oy__binary(&yices_add, arg1, arg2);
}
CAMLprim value ocamlyices_term_addn(value args) {
  return _oy__nary_const(&yices_sum, args);
}

CAMLprim value ocamlyices_term_sub(value arg1, value arg2) {
  return _oy__binary(&yices_sub, arg1, arg2);
}

CAMLprim value ocamlyices_term_neg(value arg) {
  return _oy__unary(&yices_neg, arg);
}

CAMLprim value ocamlyices_term_mul(value arg1, value arg2) {
  return _oy__binary(&yices_mul, arg1, arg2);
}
CAMLprim value ocamlyices_term_muln(value args) {
  return _oy__nary_const(&yices_product, args);
}

CAMLprim value ocamlyices_term_square(value arg) {
  return _oy__unary(&yices_square, arg);
}

CAMLprim value ocamlyices_term_power_int(value v_t, value v_d) {
  return _oy_term_of_term_degree(&yices_power, v_t, v_d);
}

CAMLprim value ocamlyices_term_power_int64(value v_t, value v_d) {
  return _oy_term_of_term_degree64(&yices_power, v_t, v_d);
}

CAMLprim value ocamlyices_term_div(value a, value b) {
  return _oy__binary(&yices_division, a, b);
}

CAMLprim value ocamlyices_term_poly_int (value v_as, value v_ts) {
  CAMLparam2(v_as, v_ts);
  term_t *ts, res;
  uint32_t las, lts;
  intnat *as;
  _oy_check_init();

  las = check_Wosize_val(v_as);
  lts = check_Wosize_val(v_ts);
  if (las != lts) {
    _oy__invalid_array_sizes();
  }

  as = _oy__nativeints_from_int_values(v_as, las);
  ts = _oy__terms_from_values(v_ts, lts);
  if (as == NULL || ts == NULL) {
    free(as);
    free(ts);
    _oy__allocation_error();
  }

  if (sizeof(intnat) == sizeof(int32_t)) {
    res = yices_poly_int32(las, (int32_t *)as, ts);
  } else {
    res = yices_poly_int64(las, (int64_t *)as, ts);
  }
  free(as);
  free(ts);

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_poly_nativeint (value v_as, value v_ts) {
  CAMLparam2(v_as, v_ts);
  term_t *ts, res;
  uint32_t las, lts;
  intnat *as;
  _oy_check_init();

  las = check_Wosize_val(v_as);
  lts = check_Wosize_val(v_ts);
  if (las != lts) {
    _oy__invalid_array_sizes();
  }

  as = _oy__nativeints_from_nativeint_values(v_as, las);
  ts = _oy__terms_from_values(v_ts, lts);
  if (as == NULL || ts == NULL) {
    free(as);
    free(ts);
    _oy__allocation_error();
  }

  if (sizeof(intnat) == sizeof(int32_t)) {
    res = yices_poly_int32(las, (int32_t *)as, ts);
  } else {
    res = yices_poly_int64(las, (int64_t *)as, ts);
  }
  free(as);
  free(ts);

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_poly_int32 (value v_as, value v_ts) {
  CAMLparam2(v_as, v_ts);
  term_t *ts, res;
  uint32_t las, lts;
  int32_t *as;
  _oy_check_init();

  las = check_Wosize_val(v_as);
  lts = check_Wosize_val(v_ts);
  if (las != lts) {
    _oy__invalid_array_sizes();
  }

  as = _oy__int32s_from_int32_values(v_as, las);
  ts = _oy__terms_from_values(v_ts, lts);
  if (as == NULL || ts == NULL) {
    free(as);
    free(ts);
    _oy__allocation_error();
  }

  res = yices_poly_int32(las, as, ts);
  free(as);
  free(ts);

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_poly_int64 (value v_as, value v_ts) {
  CAMLparam2(v_as, v_ts);
  term_t *ts, res;
  uint32_t las, lts;
  int64_t *as;
  _oy_check_init();

  las = check_Wosize_val(v_as);
  lts = check_Wosize_val(v_ts);
  if (las != lts) {
    _oy__invalid_array_sizes();
  }

  as = _oy__int64s_from_int64_values(v_as, las);
  ts = _oy__terms_from_values(v_ts, lts);
  if (as == NULL || ts == NULL) {
    free(as);
    free(ts);
    _oy__allocation_error();
  }

  res = yices_poly_int64(las, as, ts);
  free(as);
  free(ts);

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_poly_rational_int (value v_nums, value v_dens,
    value v_ts) {
  CAMLparam3(v_nums, v_dens, v_ts);
  term_t *ts, res;
  uint32_t lnums, ldens, lts, i;
  intnat *nums, *dens;
  _oy_check_init();

  lnums = check_Wosize_val(v_nums);
  ldens = check_Wosize_val(v_dens);
  lts = check_Wosize_val(v_ts);
  if (lnums != ldens || lnums != lts) {
    _oy__invalid_array_sizes();
  }

  nums = _oy__nativeints_from_int_values(v_nums, lnums);
  dens = _oy__nativeints_from_int_values(v_dens, ldens);
  ts = _oy__terms_from_values(v_ts, lts);
  if (dens == NULL || nums == NULL || ts == NULL) {
    free(nums);
    free(dens);
    free(ts);
    _oy__allocation_error();
  }

  for (i = 0; i < ldens; i++) {
    if (dens[i] <= 0) {
      free(nums);
      free(dens);
      free(ts);
      _oy__invalid_argument("null or negative denominator");
    }
  }

  if (sizeof(intnat) == sizeof(int32_t)) {
    res = yices_poly_rational32(lnums, (int32_t *)nums, (uint32_t *)dens, ts);
  } else {
    res = yices_poly_rational64(lnums, (int64_t *)nums, (uint64_t *)dens, ts);
  }
  free(nums);
  free(dens);
  free(ts);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_poly_rational_nativeint (value v_nums,
    value v_dens, value v_ts) {
  CAMLparam3(v_nums, v_dens, v_ts);
  term_t *ts, res;
  uint32_t lnums, ldens, lts, i;
  intnat *nums, *dens;
  _oy_check_init();

  lnums = check_Wosize_val(v_nums);
  ldens = check_Wosize_val(v_dens);
  lts = check_Wosize_val(v_ts);
  if (lnums != ldens || lnums != lts) {
    _oy__invalid_array_sizes();
  }

  ts = _oy__terms_from_values(v_ts, lts);
  nums = _oy__nativeints_from_nativeint_values(v_nums, lnums);
  dens = _oy__nativeints_from_nativeint_values(v_dens, ldens);
  if (dens == NULL || nums == NULL || ts == NULL) {
    free(nums);
    free(dens);
    free(ts);
    _oy__allocation_error();
  }

  for (i = 0; i < ldens; i++) {
    if (dens[i] < 0) {
      free(nums);
      free(dens);
      free(ts);
      _oy__invalid_argument("negative denominator");
    }
  }

  if (sizeof(intnat) == sizeof(int32_t)) {
    res = yices_poly_rational32(lnums, (int32_t *)nums, (uint32_t *)dens, ts);
  } else {
    res = yices_poly_rational64(lnums, (int64_t *)nums, (uint64_t *)dens, ts);
  }
  free(nums);
  free(dens);
  free(ts);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_poly_rational_int32 (value v_nums, value v_dens,
    value v_ts) {
  CAMLparam3(v_nums, v_dens, v_ts);
  term_t *ts, res;
  uint32_t lnums, ldens, lts, i;
  int32_t *nums, *dens;
  _oy_check_init();

  lnums = check_Wosize_val(v_nums);
  ldens = check_Wosize_val(v_dens);
  lts = check_Wosize_val(v_ts);
  if (lnums != ldens || lnums != lts) {
    _oy__invalid_array_sizes();
  }

  nums = _oy__int32s_from_int32_values(v_nums, lnums);
  dens = _oy__int32s_from_int32_values(v_dens, ldens);
  ts = _oy__terms_from_values(v_ts, lts);
  if (dens == NULL || nums == NULL || ts == NULL) {
    free(nums);
    free(dens);
    free(ts);
    _oy__allocation_error();
  }

  for (i = 0; i < ldens; i++) {
    if (dens[i] < 0) {
      free(nums);
      free(dens);
      free(ts);
      _oy__invalid_argument("negative denominator");
    }
  }

  res = yices_poly_rational32(lnums, (int32_t *)nums, (uint32_t *)dens, ts);
  free(nums);
  free(dens);
  free(ts);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_poly_rational_int64 (value v_nums, value v_dens,
    value v_ts) {
  CAMLparam3(v_nums, v_dens, v_ts);
  term_t *ts, res;
  uint32_t lnums, ldens, lts, i;
  int64_t *nums, *dens;
  _oy_check_init();

  lnums = check_Wosize_val(v_nums);
  ldens = check_Wosize_val(v_dens);
  lts = check_Wosize_val(v_ts);
  if (lnums != ldens || lnums != lts) {
    _oy__invalid_array_sizes();
  }

  nums = _oy__int64s_from_int64_values(v_nums, lnums);
  dens = _oy__int64s_from_int64_values(v_dens, ldens);
  ts = _oy__terms_from_values(v_ts, lts);

  if (dens == NULL || nums == NULL || ts == NULL) {
    free(nums);
    free(dens);
    free(ts);
    _oy__allocation_error();
  }

  for (i = 0; i < ldens; i++) {
    if (dens[i] <= 0) {
      free(nums);
      free(dens);
      free(ts);
      _oy__invalid_argument("null or negative denominator");
    }
  }

  res = yices_poly_rational64(lnums, (int64_t *)nums, (uint64_t *)dens, ts);
  free(nums);
  free(dens);
  free(ts);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}


CAMLprim value ocamlyices_term_poly_z(value v_zs, value v_ts) {
  CAMLparam2(v_zs, v_ts);
  term_t *ts, res;
  mpz_t *zs;
  uint32_t lzs, lts, i;
  _oy_check_init();

  lzs = check_Wosize_val(v_zs);
  lts = check_Wosize_val(v_ts);
  if (lts != lzs) {
    _oy__invalid_array_sizes();
  }

  ts = _oy__terms_from_values(v_ts, lts);
  if (ts == NULL) {
    _oy__allocation_error();
  }

  zs = (mpz_t *)malloc(sizeof(mpz_t[lzs]));
  if (zs == NULL) {
    free(ts);
    _oy__allocation_error();
  }
  for (i = 0; i < lzs; i++) {
    ml_z_mpz_init_set_z(zs[i],Field(v_zs,i));
  }

  res = yices_poly_mpz(lzs, (const mpz_t *)zs, ts);

  for (i = 0; i < lzs; i++) {
    mpz_clear(zs[i]);
  }
  free(zs);
  free(ts);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_poly_q(value v_qs, value v_ts) {
  CAMLparam2(v_qs, v_ts);
  term_t *ts, res;
  mpq_t *qs;
  mpz_t temp;
  uint32_t lqs, lts, i;
  _oy_check_init();

  lqs = check_Wosize_val(v_qs);
  lts = check_Wosize_val(v_ts);

  ts = _oy__terms_from_values(v_ts, lts);
  if (ts == NULL) {
    _oy__allocation_error();
  }

  if (lts != lqs) {
    free(ts);
    _oy__invalid_array_sizes();
  }

  qs = (mpq_t *)malloc(sizeof(mpq_t[lqs]));
  if (qs == NULL) {
    free(ts);
    _oy__allocation_error();
  }
  mpz_init(temp);
  for (i = 0; i < lqs; i++) {
    ml_z_mpz_set_z(temp, Field(Field(v_qs,i),0));
    mpq_set_num(qs[i], temp);
    ml_z_mpz_set_z(temp, Field(Field(v_qs,i),1));
    mpq_set_den(qs[i], temp);
  }
  mpz_clear(temp);

  res = yices_poly_mpq(lqs, (const mpq_t *)qs, ts);

  for (i = 0; i < lqs; i++) {
    mpq_clear(qs[i]);
  }
  free(qs);
  free(ts);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_arith_eq(value arg1, value arg2) {
  return _oy__binary(&yices_arith_eq_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_arith_neq(value arg1, value arg2) {
  return _oy__binary(&yices_arith_neq_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_arith_geq(value arg1, value arg2) {
  return _oy__binary(&yices_arith_geq_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_arith_leq(value arg1, value arg2) {
  return _oy__binary(&yices_arith_leq_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_arith_gt(value arg1, value arg2) {
  return _oy__binary(&yices_arith_gt_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_arith_lt(value arg1, value arg2) {
  return _oy__binary(&yices_arith_lt_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_arith_eq0(value arg) {
  return _oy__unary(&yices_arith_eq0_atom, arg);
}

CAMLprim value ocamlyices_term_arith_neq0(value arg) {
  return _oy__unary(&yices_arith_neq0_atom, arg);
}

CAMLprim value ocamlyices_term_arith_geq0(value arg) {
  return _oy__unary(&yices_arith_geq0_atom, arg);
}

CAMLprim value ocamlyices_term_arith_leq0(value arg) {
  return _oy__unary(&yices_arith_leq0_atom, arg);
}

CAMLprim value ocamlyices_term_arith_gt0(value arg) {
  return _oy__unary(&yices_arith_gt0_atom, arg);
}

CAMLprim value ocamlyices_term_arith_lt0(value arg) {
  return _oy__unary(&yices_arith_lt0_atom, arg);
}

/** BITVECTOR **/

CAMLprim value ocamlyices_term_bitvector_of_int(value vn, value vval) {
  intnat n = Long_val(vn);
  intnat val = Long_val(vval);
  term_t res;
  _oy_check_init();

  if (n < 0) {
    _oy__invalid_argument("negative bitsize");
  }

  if (sizeof(intnat) == sizeof(uint32_t)) {
    res = yices_bvconst_uint32((uint32_t)n, (uint32_t)val);
  }  else {
    if (n > UINT32_MAX) {
      _oy__invalid_argument("too large bitsize");
    }
    res = yices_bvconst_uint64((uint32_t)n, (uint64_t)val);
  }

  if (res == NULL_TERM) {
    _oy__error();
  }

  return Val_term(res);
}

CAMLprim value ocamlyices_term_bitvector_of_nativeint(value vn, value vval) {
  CAMLparam2(vn, vval);
  intnat n = Long_val(vn);
  intnat val = Nativeint_val(vval);
  term_t res;
  _oy_check_init();

  if (n < 0) {
    _oy__invalid_argument("negative bitsize");
  }

  if (sizeof(intnat) == sizeof(uint32_t)) {
    res = yices_bvconst_uint32((uint32_t)n, (uint32_t)val);
  }  else {
    if (n > UINT32_MAX) {
      _oy__invalid_argument("too large bitsize");
    }
    res = yices_bvconst_uint64((uint32_t)n, (uint64_t)val);
  }

  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_bitvector_of_int32(value vn, value vval) {
  CAMLparam1(vval);
  intnat n = Long_val(vn);
  int32_t val = Int32_val(vval);
  term_t res;
  _oy_check_init();

  if (n < 0) {
    _oy__invalid_argument("negative bitsize");
  }

  if (Max_long > UINT32_MAX && n > UINT32_MAX) {
    _oy__invalid_argument("too large bitsize");
  }

  res = yices_bvconst_uint32((uint32_t)n, (uint32_t)val);

  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_bitvector_of_int64(value vn, value vval) {
  CAMLparam1(vval);
  intnat n = Long_val(vn);
  int64_t val = Int64_val(vval);
  term_t res;
  _oy_check_init();

  if (n < 0) {
    _oy__invalid_argument("negative bitsize");
  }

  if (Max_long > UINT32_MAX && n > UINT32_MAX) {
    _oy__invalid_argument("too large bitsize");
  }

  res = yices_bvconst_uint64((uint32_t)n, (uint64_t)val);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_bitvector_of_z(value vn, value vval) {
  CAMLparam1(vval);
  intnat n = Long_val(vn);
  mpz_t val;
  term_t res;
  _oy_check_init();

  if (n < 0) {
    _oy__invalid_argument("negative bitsize");
  }

  if (Max_long > UINT32_MAX && n > UINT32_MAX) {
    _oy__invalid_argument("too large bitsize");
  }

  ml_z_mpz_init_set_z(val, vval);
  res = yices_bvconst_mpz((uint32_t)n, val);
  mpz_clear(val);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}


CAMLprim value ocamlyices_term_bitvector_zero(value v_n) {
  _oy_check_init();
  return _oy__bitvector_const(&yices_bvconst_zero, v_n);
}
CAMLprim value ocamlyices_term_bitvector_one(value v_n) {
  _oy_check_init();
  return _oy__bitvector_const(&yices_bvconst_one, v_n);
}
CAMLprim value ocamlyices_term_bitvector_minus_one(value v_n) {
  _oy_check_init();
  return _oy__bitvector_const(&yices_bvconst_minus_one, v_n);
}

CAMLprim value ocamlyices_term_bitvector_of_bools(value v_arr) {
  CAMLparam1(v_arr);
  term_t res;
  int32_t *arr;
  uint32_t i, n;
  _oy_check_init();

  n = check_Wosize_val(v_arr);

  arr = (int32_t *) malloc(sizeof(int32_t[n]));
  if (arr) {
    _oy__allocation_error();
  }
  for (i = 0; i < n; i++) {
    arr[i] = Bool_val(Field(v_arr, i)) != 0;
  }

  res = yices_bvconst_from_array(n, arr);
  free(arr);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_bitvector_of_bin(value v_arg) {
  return _oy__parser(&yices_parse_bvbin, v_arg);
}
CAMLprim value ocamlyices_term_bitvector_of_hex(value v_arg) {
  return _oy__parser(&yices_parse_bvhex, v_arg);
}

CAMLprim value ocamlyices_term_bitvector_add(value arg1, value arg2) {
  return _oy__binary(&yices_bvadd, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_sub(value arg1, value arg2) {
  return _oy__binary(&yices_bvsub, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_neg(value arg) {
  return _oy__unary(&yices_bvneg, arg);
}

CAMLprim value ocamlyices_term_bitvector_mul(value arg1, value arg2) {
  return _oy__binary(&yices_bvmul, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_square(value arg) {
  return _oy__unary(&yices_bvsquare, arg);
}

CAMLprim value ocamlyices_term_bitvector_power_int(value v_bv, value v_degree) {
  return _oy_term_of_term_degree(&yices_bvpower, v_bv, v_degree);
}

CAMLprim value ocamlyices_term_bitvector_power_int64(value v_bv,
    value v_degree) {
  return _oy_term_of_term_degree64(&yices_bvpower, v_bv, v_degree);
}

CAMLprim value ocamlyices_term_bitvector_div(value arg1, value arg2) {
  return _oy__binary(&yices_bvdiv, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_rem(value arg1, value arg2) {
  return _oy__binary(&yices_bvrem, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_sdiv(value arg1, value arg2) {
  return _oy__binary(&yices_bvsdiv, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_srem(value arg1, value arg2) {
  return _oy__binary(&yices_bvsrem, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_smod(value arg1, value arg2) {
  return _oy__binary(&yices_bvsmod, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_not(value arg) {
  return _oy__unary(&yices_bvnot, arg);
}

CAMLprim value ocamlyices_term_bitvector_and(value arg1, value arg2) {
  return _oy__binary(&yices_bvand, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_or(value arg1, value arg2) {
  return _oy__binary(&yices_bvor, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_xor(value arg1, value arg2) {
  return _oy__binary(&yices_bvxor, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_nand(value arg1, value arg2) {
  return _oy__binary(&yices_bvnand, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_nor(value arg1, value arg2) {
  return _oy__binary(&yices_bvnor, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_xnor(value arg1, value arg2) {
  return _oy__binary(&yices_bvxnor, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_shl(value arg1, value arg2) {
  return _oy__binary(&yices_bvshl, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_lshr(value arg1, value arg2) {
  return _oy__binary(&yices_bvlshr, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_ashr(value arg1, value arg2) {
  return _oy__binary(&yices_bvashr, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_shift_left0(value arg1, value arg2) {
  return _oy_term_of_term_shift(&yices_shift_left0, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_shift_left1(value arg1, value arg2) {
  return _oy_term_of_term_shift(&yices_shift_left1, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_shift_right0(value arg1, value arg2) {
  return _oy_term_of_term_shift(&yices_shift_right0, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_shift_right1(value arg1, value arg2) {
  return _oy_term_of_term_shift(&yices_shift_right1, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_ashift_right(value arg1, value arg2) {
  return _oy_term_of_term_shift(&yices_ashift_right, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_rotate_left(value arg1, value arg2) {
  return _oy_term_of_term_shift(&yices_rotate_left, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_rotate_right(value arg1, value arg2) {
  return _oy_term_of_term_shift(&yices_rotate_right, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_extract(value v_t, value v_i1,
    value v_i2) {
  // Hyp: v_t is a non-block value
  term_t res;

  intnat i1 = Long_val(v_i1);
  intnat i2 = Long_val(v_i2);

  if (i1 < 0) {
    _oy__invalid_argument("negative first index");
  }
  if (sizeof(intnat) > sizeof(uint32_t) && i1 > UINT32_MAX) {
    _oy__invalid_argument("first index too large");
  }
  if (i2 < 0) {
    _oy__invalid_argument("negative second index");
  }
  if (sizeof(intnat) > sizeof(uint32_t) && i2 > UINT32_MAX) {
    _oy__invalid_argument("second index too large");
  }

  res = yices_bvextract(Term_val(v_t), (uint32_t)i1, (uint32_t)i2);
  if (res == NULL_TERM) {
    _oy__error();
  }

  return Val_term(res);
}

CAMLprim value ocamlyices_term_bitvector_concat(value arg1, value arg2) {
  return _oy__binary(&yices_bvconcat, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_repeat(value arg1, value arg2) {
  return _oy_term_of_term_MLintCuint32(&yices_bvrepeat, arg1, arg2,
                                       "negative number of repetitions", "number of repetitions too large");
}

CAMLprim value ocamlyices_term_bitvector_sign_extend(value arg1, value arg2) {
  return _oy_term_of_term_MLintCuint32(&yices_sign_extend, arg1, arg2,
                                       "negative bitsize", "bitsize too large");
}

CAMLprim value ocamlyices_term_bitvector_zero_extend(value arg1, value arg2) {
  return _oy_term_of_term_MLintCuint32(&yices_zero_extend, arg1, arg2,
                                       "negative bitsize", "bitsize too large");
}

CAMLprim value ocamlyices_term_bitvector_redand(value arg) {
  return _oy__unary(&yices_redand, arg);
}

CAMLprim value ocamlyices_term_bitvector_redor(value arg) {
  return _oy__unary(&yices_redor, arg);
}

CAMLprim value ocamlyices_term_bitvector_redcomp(value arg1, value arg2) {
  return _oy__binary(&yices_redcomp, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_of_terms(value v_args) {
  return _oy__nary_const(&yices_bvarray, v_args);
}

CAMLprim value ocamlyices_term_bitvector_bitextract(value v_bv, value v_index) {
  return _oy_term_of_term_index(&yices_bitextract, v_bv, v_index);
}

CAMLprim value ocamlyices_term_bitvector_eq(value arg1, value arg2) {
  return _oy__binary(&yices_bveq_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_neq(value arg1, value arg2) {
  return _oy__binary(&yices_bvneq_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_ge(value arg1, value arg2) {
  return _oy__binary(&yices_bvge_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_gt(value arg1, value arg2) {
  return _oy__binary(&yices_bvgt_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_le(value arg1, value arg2) {
  return _oy__binary(&yices_bvle_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_lt(value arg1, value arg2) {
  return _oy__binary(&yices_bvlt_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_sge(value arg1, value arg2) {
  return _oy__binary(&yices_bvsge_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_sgt(value arg1, value arg2) {
  return _oy__binary(&yices_bvsgt_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_sle(value arg1, value arg2) {
  return _oy__binary(&yices_bvsle_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_bitvector_slt(value arg1, value arg2) {
  return _oy__binary(&yices_bvslt_atom, arg1, arg2);
}

CAMLprim value ocamlyices_term_subst(value v_vars, value v_repls, value v_t) {
  // Hyp: v_t is a non-block value
  CAMLparam2(v_vars, v_repls);
  term_t res;
  term_t *repls, *vars;
  uint32_t lvars, lrepls;

  lvars = check_Wosize_val(v_vars);
  lrepls = check_Wosize_val(v_repls);
  if (lvars != lrepls) {
    _oy__invalid_array_sizes();
  }

  vars = _oy__terms_from_values(v_vars, lvars);
  repls = _oy__terms_from_values(v_repls, lrepls);
  if (vars == NULL || repls == NULL) {
    free(vars);
    free(repls);
    _oy__allocation_error();
  }

  res = yices_subst_term(lvars, vars, repls, Term_val(v_t));
  free(vars);
  free(repls);
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_int(res));
}

CAMLprim value ocamlyices_term_subst_array(value v_vars, value v_repls,
    value v_ts) {
  CAMLparam3(v_vars, v_repls, v_ts);
  int32_t res;
  term_t *repls, *vars, *ts;
  uint32_t lrepls, lvars, lts, i;

  lvars = check_Wosize_val(v_vars);
  lrepls = check_Wosize_val(v_repls);
  if (lvars != lrepls) {
    _oy__invalid_array_sizes();
  }
  lts = check_Wosize_val(v_ts);

  vars = _oy__terms_from_values(v_vars, lvars);
  repls = _oy__terms_from_values(v_repls, lrepls);
  ts = _oy__terms_from_values(v_ts, lts);
  if (vars == NULL || repls == NULL || ts == NULL) {
    free(vars);
    free(repls);
    free(ts);
    _oy__allocation_error();
  }

  res = yices_subst_term_array(lvars, vars, repls, lts, ts);
  free(vars);
  free(repls);
  if (res == NULL_TERM) {
    free(ts);
    _oy__error();
  }

  for (i = 0; i < lts; i++) {
    Store_field(v_ts, i, ts[i]);
  }

  free(ts);

  CAMLreturn(Val_unit);
}

// NAMES

CAMLprim value ocamlyices_term_set_name (value v_t, value v_name) {
  // Hyp: v_t is a non-block value
  CAMLparam1(v_name);
  int32_t res;

  res = yices_set_term_name(Term_val(v_t), String_val(v_name));
  if (res != 0) {
    _oy__error();
  }

  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_term_remove_name (value v_name) {
  CAMLparam1(v_name);
  _oy_check_init();

  yices_remove_term_name(String_val(v_name));

  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_term_clear_name (value v_t) {
  // Hyp: v_t is a non-block value
  int32_t res;

  res = yices_clear_term_name(Term_val(v_t));
  if (res != 0) {
    _oy__error();
  }

  return Val_unit;
}

CAMLprim value ocamlyices_term_get_name (value v_t) {
  // Hyp: v_t is a non-block value
  const char *res;

  res = yices_get_term_name(Term_val(v_t));
  if (res == NULL) {
    _oy__error();
  }

  return caml_copy_string(res);
}

CAMLprim value ocamlyices_term_by_name (value v_name) {
  CAMLparam1(v_name);
  term_t res;
  _oy_check_init();

  res = yices_get_term_by_name(String_val(v_name));
  if (res == NULL_TERM) {
    _oy__error();
  }

  CAMLreturn(Val_term(res));
}

CAMLprim value ocamlyices_term_bitsize (value v_t) {
  // HYP: term values are non-block
  uint32_t res;
  res = yices_term_bitsize(Term_val(v_t));
  if (res == 0) {
    _oy__error();
  }

  if (YICES_MAX_BVSIZE > Max_long /* should not happen */ && res > Max_long) {
    _oy__binding_overflow_error();
  }

  return Val_long((intnat)res);
}

// Generic parse

CAMLprim value ocamlyices_term_parse (value v_str) {
  return _oy__parser(&yices_parse_term, v_str);
}

// Type testing

CAMLprim value ocamlyices_term_is_bool(value v_t) {
  return _oy_bool_of_term(&yices_term_is_bool, v_t);
}

CAMLprim value ocamlyices_term_is_int(value v_t) {
  return _oy_bool_of_term(&yices_term_is_int, v_t);
}

CAMLprim value ocamlyices_term_is_real(value v_t) {
  return _oy_bool_of_term(&yices_term_is_real, v_t);
}

CAMLprim value ocamlyices_term_is_arithmetic(value v_t) {
  return _oy_bool_of_term(&yices_term_is_arithmetic, v_t);
}

CAMLprim value ocamlyices_term_is_bitvector(value v_t) {
  return _oy_bool_of_term(&yices_term_is_bitvector, v_t);
}

CAMLprim value ocamlyices_term_is_tuple(value v_t) {
  return _oy_bool_of_term(&yices_term_is_tuple, v_t);
}

CAMLprim value ocamlyices_term_is_function(value v_t) {
  return _oy_bool_of_term(&yices_term_is_function, v_t);
}

CAMLprim value ocamlyices_term_is_scalar(value v_t) {
  return _oy_bool_of_term(&yices_term_is_scalar, v_t);
}

CAMLprim value ocamlyices_term_is_ground(value v_t) {
  return _oy_bool_of_term(&yices_term_is_ground, v_t);
}
