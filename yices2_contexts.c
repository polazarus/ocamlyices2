#include <yices.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/threads.h>
#include <caml/custom.h>
#include <stdio.h>

#include "yices2.h"
//#include "yices2_macros.h"

value ocamlyices_new_config(value unit) {
  CAMLparam1(unit);
  CAMLlocal1(v_res);
  ctx_config_t * res = yices_new_config();
  v_res = caml_alloc_config();
  Store_config_val(v_res, res);
  CAMLreturn(v_res);
}

value ocamlyices_free_config(value v_config) {
  CAMLparam1(v_config);
  ctx_config_t *config = Config_val(v_config);
  if (config != (void*)0) {
    yices_free_config(config);
    Store_config_val(v_config, (void*) 0);
  }
  CAMLreturn(Val_unit);
}

value ocamlyices_set_config(value v_config, value v_name, value v_value) {
  CAMLparam3(v_config, v_name, v_value);
  int32_t res;
  ctx_config_t * config;
  const char* name;
  const char* value_;
  
  name = String_val(v_name);
  value_ = String_val(v_value);
  config = Config_val(v_config);
  NOT_NEEDED_VALUE(v_config);

  if (config == (void*)0) ocamlyices_already_freed_config();

  res = yices_set_config(config, name, value_);
  if (res != 0) ocamlyices_failure();
  CAMLreturn(Val_unit);
}

value ocamlyices_default_config_for_logic(value v_config, value v_logic) {
  CAMLparam2(v_config, v_logic);
  int32_t res;
  ctx_config_t * config;
  const char* logic;
  
  config = Config_val(v_config);
  logic = String_val(v_logic);
  NOT_NEEDED_VALUE(v_config);

  if (config == (void*)0) ocamlyices_already_freed_config();

  res = yices_default_config_for_logic(config, logic);
  if (res != 0) ocamlyices_failure();
  CAMLreturn(Val_unit);
}

value ocamlyices_new_context(value v_config) {
  CAMLparam1(v_config);
  CAMLlocal1(v_res);
  context_t *res;
  ctx_config_t *config;

  config = Config_val(v_config);
  NOT_NEEDED_VALUE(v_config);

  if (config == (void*)0) ocamlyices_already_freed_config();

  res = yices_new_context(config);
  if (res == (void*)0) ocamlyices_failure();

  v_res = caml_alloc_context();
  Store_context_val(v_res, res);
  CAMLreturn(v_res);
}

value ocamlyices_free_context(value v_context) {
  CAMLparam1(v_context);
  context_t* context = Context_val(v_context);
  if (context != (void*)0) {
    yices_free_context(context);
    Store_context_val(v_context, (void*)0);
  }
  CAMLreturn(Val_unit);
}

value ocamlyices_context_status(value v_context) {
  CAMLparam1(v_context);

  context_t *context = Context_val(v_context);
  NOT_NEEDED_VALUE(v_context);
  if (context == (void*)0) ocamlyices_already_freed_context();

  smt_status_t status = yices_context_status(context);

  CAMLreturn(Val_int((int)status));
}

value ocamlyices_reset_context(value v_context) {
  CAMLparam1(v_context);

  context_t* context = Context_val(v_context);
  NOT_NEEDED_VALUE(v_context);
  if (context == (void*)0) ocamlyices_already_freed_context();

  COND_MT_START(MTFLAG_STACKOP);
  yices_reset_context(context);
  COND_MT_END(MTFLAG_STACKOP);

  CAMLreturn(Val_unit);
}

#define OCAMLYICES_CONTEXT_STACKOP(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_context) {\
    CAMLparam1(v_context);\
    int32_t res;\
    context_t* context = Context_val(v_context);\
    NOT_NEEDED_VALUE(v_context);\
  \
    COND_MT_START(MTFLAG_STACKOP);\
    res = yices_ ## nameIn (context);\
    COND_MT_END(MTFLAG_STACKOP);\
  \
    if (res != 0) ocamlyices_failure();\
    CAMLreturn(Val_unit);\
  }

OCAMLYICES_CONTEXT_STACKOP(push, push)
OCAMLYICES_CONTEXT_STACKOP(pop, pop)

#define OCAMLYICES_CONTEXT_TOGGLE_OPTION(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_context, value v_option) {\
    CAMLparam2(v_context, v_option);\
    int32_t res;\
    context_t* context = Context_val(v_context);\
    const char* option = String_val(v_option);\
  \
    res = yices_ ## nameIn (context, option);\
  \
    if (res != 0) ocamlyices_failure();\
    CAMLreturn(Val_unit);\
  }

OCAMLYICES_CONTEXT_TOGGLE_OPTION(context_enable_option, context_enable_option)
OCAMLYICES_CONTEXT_TOGGLE_OPTION(context_disable_option, context_disable_option)

value ocamlyices_assert_formula(value v_context, value v_formula) {
  CAMLparam2(v_context, v_formula);
  int32_t res;
  term_t formula = Int_val(v_formula);
  context_t* context = Context_val(v_context);
  NOT_NEEDED_VALUE(v_context);

  COND_MT_START(MTFLAG_ASSERT);
  res = yices_assert_formula(context, formula);
  COND_MT_END(MTFLAG_ASSERT);

  if (res != 0) ocamlyices_failure();
  CAMLreturn(Val_unit);
}

value ocamlyices_assert_formulas(value v_context, value v_formulas) {
  CAMLparam2(v_context, v_formulas);
  int32_t res;
  context_t* context;
  size_t n;
  term_t* formulas;

  context = Context_val(v_context);
  NOT_NEEDED_VALUE(v_context);

  ML2C_COPY_ARRAY(v_formulas, n, formulas, term_t, Term_val);
  NOT_NEEDED_VALUE(v_formulas);
  if (formulas == (void*)0) ocamlyices_allocation_error();

  COND_MT_START(MTFLAG_ASSERT);
  res = yices_assert_formulas(context, n, formulas);
  free(formulas);
  COND_MT_END(MTFLAG_ASSERT);

  if (res != 0) ocamlyices_failure();
  CAMLreturn(Val_unit);
}

value ocamlyices_check(value v_param_opt, value v_context) {
  CAMLparam2(v_param_opt, v_context);
  smt_status_t res;
  context_t *context = Context_val(v_context);
  param_t *param = (Is_block(v_param_opt) ?
    *(param_t**)Data_custom_val(Field(v_param_opt, 0)) : (void*)0);
  NOT_NEEDED_VALUE(v_context);
  NOT_NEEDED_VALUE(v_param_opt);

  COND_MT_START(MTFLAG_CHECK);
  res = yices_check_context(context, param);
  COND_MT_END(MTFLAG_CHECK);

  CAMLreturn(Val_int((int)res));
}

value ocamlyices_assert_blocking_clause(value v_context) {
  CAMLparam1(v_context);
  int32_t res;
  context_t *context = Context_val(v_context);
  NOT_NEEDED_VALUE(v_context);

  COND_MT_START(MTFLAG_ASSERT);
  res = yices_assert_blocking_clause(context);
  COND_MT_END(MTFLAG_ASSERT);

  if (res != 0) ocamlyices_failure();
  CAMLreturn(Val_int(0));
}

value ocamlyices_stop_search(value v_context) {
  CAMLparam1(v_context);
  context_t* context = Context_val(v_context);
  NOT_NEEDED_VALUE(v_context);

  COND_MT_START(MTFLAG_STOP_SEARCH);
  yices_stop_search(context);
  COND_MT_END(MTFLAG_STOP_SEARCH);

  CAMLreturn(Val_int(0));
}

value ocamlyices_new_param_record(value unit) {
  CAMLparam1(unit);
  CAMLlocal1(v_res);

  param_t *res = yices_new_param_record();
  if (res == (void*)0) ocamlyices_failure();

  v_res = caml_alloc_custom(&ocamlyices_param_ops, sizeof (param_t*), 0, 1);
  *(param_t**)Data_custom_val(v_res) = res;

  CAMLreturn(Val_int(v_res));
}

value ocamlyices_set_param(value v_param, value v_name, value v_value) {
  CAMLparam3(v_param, v_name, v_value);
  param_t * param = *(param_t**)Data_custom_val(v_param);
  const char* name = String_val(v_name);
  const char* value_ = String_val(v_value);
  int32_t res;
  NOT_NEEDED_VALUE(v_param);
  /* Cannot release v_name and v_value */

  res = yices_set_param(param, name, value_);
  if (res != 0) ocamlyices_failure();

  CAMLreturn(Val_unit);
}

value ocamlyices_free_param_record(value v_param) {
  CAMLparam1(v_param);
  param_t *param = *(param_t**)Data_custom_val(v_param);
  NOT_NEEDED_VALUE(v_param);

  yices_free_param_record(param);

  CAMLreturn(Val_unit);
}


