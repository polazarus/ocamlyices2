#include <yices.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/threads.h>
#include <caml/custom.h>
#include <stdio.h>

#include "yices2.h"

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
    int64_t res, val64;
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

  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  int32_t res, val;
  res = yices_get_int32_value(mdl, t, &val);
  if (res != 0) ocamlyices_failure();
  CAMLreturn(caml_copy_int32(val));
}

value ocamlyices_get_int64_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  model_t *mdl;
  term_t t;

  mdl = Model_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  int64_t res, val;
  res = yices_get_int64_value(mdl, t, &val);
  if (res != 0) ocamlyices_failure();

  CAMLreturn(caml_copy_int64(val));
}

value ocamlyices_get_bv_value(value v_mdl, value v_t) {
  CAMLparam2(v_mdl, v_t);
  CAMLlocal1(v_bv);
  model_t *mdl;
  term_t t;
  int32_t res, n, *bv;
  size_t i;

  mdl = *(model_t**)Data_custom_val(v_mdl);
  NOT_NEEDED_VALUE(v_mdl);
  t = Term_val(v_t);

  if (mdl == (void*)0) ocamlyices_already_freed_model();

  n = yices_term_bitsize(t);
  if (n <= 0) ocamlyices_failure();

  bv = calloc(n, sizeof(int32_t));
  if (bv == (void*)0) {
    ocamlyices_allocation_error();
    CAMLreturn(Val_unit);
  }

  res = yices_get_bv_value(mdl, t, bv);

  if (res != 0) {
    free(bv);
    ocamlyices_failure();
    CAMLreturn(Val_unit);
  }

  v_bv = caml_alloc(n, 0);
  for (i = 0; i < n; i++)
    Store_field(v_bv, i, Val_long(bv[i] != 0));

  CAMLreturn(v_bv);
}

