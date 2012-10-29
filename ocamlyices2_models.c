#include "ocamlyices2.h"

value ocamlyices_get_model(value v_keepsubst, value v_context) {
  CAMLparam2(v_keepsubst, v_context);
  CAMLlocal1(v_res);
  model_t* res;
  context_t *context;
  int32_t keepsubst;

  context = Context_val(v_context);
  NOT_NEEDED_VALUE(v_context);
  keepsubst = (Is_block(v_keepsubst) ? Int_val(Field(v_keepsubst, 0)) : 1);
  NOT_NEEDED_VALUE(v_keepsubst);

  COND_MT_START(MTFLAG_GET_MODEL);
  res = yices_get_model(context, keepsubst);
  COND_MT_END(MTFLAG_GET_MODEL);
  if (res == (void*)0) ocamlyices_failure();

  v_res = caml_alloc_custom(&ocamlyices_model_ops, sizeof (model_t*), 0, 1);
  Store_model_val(v_res, res);

  CAMLreturn(v_res);
}

value ocamlyices_free_model(value v_model) {
  CAMLparam1(v_model);
  model_t * model = Model_val(v_model);

  if (model != (void*)0) {
    COND_MT_START(MTFLAG_FREE_MODEL);
    yices_free_model(model);
    COND_MT_END(MTFLAG_FREE_MODEL);
    Store_model_val(v_model, (void*)0);
  }

  CAMLreturn(Val_unit);
}

value ocamlyices_model_as_string(value v_mdl) {
  CAMLparam1(v_mdl);
  CAMLlocal1(v_res);
  model_t * mdl = Model_val(v_mdl);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  char* buf;
  size_t loc;
  FILE* memfile = open_memstream(&buf, &loc);
  if (memfile == (void*)0) ocamlyices_allocation_error();

  //COND_MT_START(MTFLAG_FREE_MODEL);
  // yices_print_model(memfile, mdl);
  yices_pp_model(memfile, mdl, 80, 2, 0);
  //COND_MT_END(MTFLAG_FREE_MODEL);

  fclose(memfile);
  v_res = caml_copy_string(buf);
  free(buf);

  CAMLreturn(v_res);
}
//print_model
//pp_model

value ocamlyices_get_bool_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  model_t *mdl;
  term_t t;
  int32_t res, val;

  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  res = yices_get_bool_value(mdl, t, &val);
  if (res != 0) ocamlyices_failure();

  CAMLreturn(Val_long(val != 0));
}

value ocamlyices_get_int_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  model_t *mdl;
  term_t t;
  
  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  intnat val;
  if (sizeof(intnat) <= 2) {
    int32_t res, val32;
    res = yices_get_int32_value(mdl, t, &val32);
    if (res != 0) ocamlyices_failure();
    val = val32;
  } else {
    int32_t res;
    int64_t val64;
    res = yices_get_int64_value(mdl, t, &val64);
    if (res != 0) ocamlyices_failure();
    val = val64;
  }

  if (val > CAML_MAX_INT || val < CAML_MIN_INT)
    ocamlyices_eval_binding_overflow();

  CAMLreturn(Val_long(val));
}

value ocamlyices_get_int32_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  model_t *mdl;
  term_t t;
  int32_t res, val;

  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  res = yices_get_int32_value(mdl, t, &val);
  if (res != 0) ocamlyices_failure();
  CAMLreturn(caml_copy_int32(val));
}

value ocamlyices_get_int64_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  model_t *mdl;
  term_t t;
  int32_t res;
  int64_t val;

  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  res = yices_get_int64_value(mdl, t, &val);
  if (res != 0) ocamlyices_failure();

  CAMLreturn(caml_copy_int64(val));
}

value ocamlyices_get_rational_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  CAMLlocal1(v_res);
  model_t *mdl;
  term_t t;
  intnat num, den;
  
  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  if (sizeof(intnat) <= 2) {
    int32_t res, num32;
    uint32_t den32;
    res = yices_get_rational32_value(mdl, t, &num32, &den32);
    if (res != 0) ocamlyices_failure();
    num = num32;
    den = den32;
  } else {
    int32_t res;
    int64_t num64;
    uint64_t den64;
    res = yices_get_rational64_value(mdl, t, &num64, &den64);
    if (res != 0) ocamlyices_failure();
    num = num64;
    den = den64;
  }

  if (num > CAML_MAX_INT || num < CAML_MIN_INT)
    ocamlyices_eval_binding_overflow();
  if (den > CAML_MAX_INT || den < 0)
    ocamlyices_eval_binding_overflow();

  v_res = caml_alloc_tuple(2);
  Store_field(v_res, 0, Val_long(num));
  Store_field(v_res, 1, Val_long(den));

  CAMLreturn(v_res);
}

value ocamlyices_get_rational32_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  CAMLlocal1(v_res);
  model_t *mdl;
  term_t t;
  int32_t res, num;
  uint32_t den;
  
  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  res = yices_get_rational32_value(mdl, t, &num, &den);
  if (res != 0) ocamlyices_failure();

  if (((int32_t)den) < 0) ocamlyices_eval_binding_overflow();

  v_res = caml_alloc_tuple(2);
  Store_field(v_res, 0, caml_copy_int32(num));
  Store_field(v_res, 1, caml_copy_int32(den));

  CAMLreturn(v_res);
}

value ocamlyices_get_rational64_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  CAMLlocal1(v_res);
  model_t *mdl;
  term_t t;
  int32_t res;
  int64_t num;
  uint64_t den;

  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  res = yices_get_rational64_value(mdl, t, &num, &den);
  if (res != 0) ocamlyices_failure();

  if (((int32_t)den) < 0) ocamlyices_eval_binding_overflow();

  v_res = caml_alloc_tuple(2);
  Store_field(v_res, 0, caml_copy_int64(num));
  Store_field(v_res, 1, caml_copy_int64(den));

  CAMLreturn(v_res);
}

value ocamlyices_get_double_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  model_t *mdl;
  term_t t;
  int32_t res;
  double val;

  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  res = yices_get_double_value(mdl, t, &val);
  if (res != 0) ocamlyices_failure();

  CAMLreturn(caml_copy_double(val));
}

#ifdef __GMP_H__

value ocamlyices_get_int_value_as_string(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  CAMLlocal1(v_res);
  model_t *mdl;
  term_t t;
  int32_t res;
  mpz_t val;
  mpz_init (val);

  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  res = yices_get_mpz_value(mdl, t, val);
  if (res != 0) ocamlyices_failure();
  
  {
    size_t buf_size = mpz_sizeinbase (val, 10) + 2;
    char* buf = malloc(sizeof(char[buf_size]));
    if (buf == (void*)0) ocamlyices_allocation_error();
    mpz_get_str(buf, 10, val);
    v_res = caml_copy_string(buf);
    free(buf);
  }

  CAMLreturn(v_res);

}

value ocamlyices_get_rational_value_as_string(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  CAMLlocal1(v_res);
  model_t *mdl;
  term_t t;
  int32_t res;
  mpq_t val;
  mpq_init (val);

  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  res = yices_get_mpq_value(mdl, t, val);
  if (res != 0) ocamlyices_failure();

  {
    size_t buf_size = mpz_sizeinbase (mpq_numref(val), 10) + mpz_sizeinbase (mpq_denref(val), 10) + 3;
    char* buf = malloc(sizeof(char[buf_size]));
    if (buf == (void*)0) ocamlyices_allocation_error();
    mpq_get_str(buf, 10, val);
    v_res = caml_copy_string(buf);
    free(buf);
  }

  CAMLreturn(v_res);

}

#endif


value ocamlyices_get_bv_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  CAMLlocal1(v_bv);
  model_t *mdl;
  term_t t;
  int32_t res, n, *bv;
  size_t i;

  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  n = yices_term_bitsize(t);
  if (n <= 0) ocamlyices_failure();

  bv = calloc(n, sizeof(int32_t));
  if (bv == (void*)0) {
    ocamlyices_allocation_error();
  }

  res = yices_get_bv_value(mdl, t, bv);

  if (res != 0) {
    free(bv);
    ocamlyices_failure();
  }

  v_bv = caml_alloc(n, 0);
  for (i = 0; i < n; i++)
    Store_field(v_bv, i, Val_long(bv[i] != 0));

  CAMLreturn(v_bv);
}

value ocamlyices_get_scalar_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  model_t *mdl;
  term_t t;
  int32_t res, val;

  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  res = yices_get_scalar_value(mdl, t, &val);

  if (res != 0) ocamlyices_failure();

  CAMLreturn(Val_long(val));
}

