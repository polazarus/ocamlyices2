#include "config.h"
#include "contexts.h"

#include <stdint.h> // for (u)int32_t etc.

#include <yices.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>

#include "misc.h"
#include "terms.h"

#include "ocamlyices2.h"

static void _oy_context_finalize(value v);
static void _oy_config_finalize(value v);
static void _oy_params_finalize(value v);

static char _oy_context_id[] = "ocamlyices.context";
static struct custom_operations _oy_context_ops = {
  _oy_context_id,
  &_oy_context_finalize,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default
};

static char _oy_config_id[] = "ocamlyices.config";
static struct custom_operations _oy_ctx_config_ops = {
  _oy_config_id,
  &_oy_config_finalize,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default
};

static char _oy_params_id[] = "ocamlyices.params";
static struct custom_operations _oy_params_ops = {
  _oy_params_id,
  &_oy_params_finalize,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default
};

#define caml_alloc_context() caml_alloc_custom(&_oy_context_ops, sizeof (context_t*), 0, 1)
#define Store_context_val(v, raw) do { *(context_t**)Data_custom_val(v) = raw; } while (0)
#define caml_alloc_config() caml_alloc_custom(&_oy_ctx_config_ops, sizeof (ctx_config_t*), 0, 1)
#define Store_config_val(v, raw) do { *(ctx_config_t**)Data_custom_val(v) = raw; } while (0)
#define Config_val(v) *(ctx_config_t**)Data_custom_val(v)

#define caml_alloc_params() caml_alloc_custom(&_oy_params_ops, sizeof (param_t*), 0, 1)
#define Store_params_val(v, raw) do { *(param_t**)Data_custom_val(v) = raw; } while (0)
#define Params_val(v) *(param_t**)Data_custom_val(v)

CAMLprim value ocamlyices_config_create(UNUSED value unit) {
  CAMLparam0();
  CAMLlocal1(v_res);
  _oy_check_init();
  ctx_config_t *res = yices_new_config();
  v_res = caml_alloc_config();
  Store_config_val(v_res, res);
  CAMLreturn(v_res);
}

static void _oy_config_finalize(value v_config) {
  DEBUG_PRINT("finalize config\n");
  ctx_config_t *config = Config_val(v_config);
  if (config != NULL) {
    yices_free_config(config);
    Store_config_val(v_config, NULL);
  }
}

CAMLprim value ocamlyices_config_set(value v_config, value v_name,
                                     value v_value) {
  CAMLparam3(v_config, v_name, v_value);
  int32_t res;
  ctx_config_t *config;
  const char *name;
  const char *value_;

  name = String_val(v_name);
  value_ = String_val(v_value);
  config = Config_val(v_config);

  if (config == NULL) {
    _oy_freed_config_error();
  }

  res = yices_set_config(config, name, value_);
  if (res != 0) {
    _oy_error();
  }
  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_config_default_for_logic(value v_config,
    value v_logic) {
  CAMLparam2(v_config, v_logic);
  int32_t res;
  ctx_config_t *config;
  const char *logic;

  config = Config_val(v_config);
  logic = String_val(v_logic);

  if (config == NULL) {
    _oy_freed_config_error();
  }

  res = yices_default_config_for_logic(config, logic);
  if (res != 0) {
    _oy_error();
  }
  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_context_create(value v_config_opt,
    UNUSED value unit) {
  CAMLparam1(v_config_opt);
  CAMLlocal1(v_res);
  context_t *res;
  ctx_config_t *config;

  if (Is_block(v_config_opt)) {
    config = Config_val(Field(v_config_opt, 0));

    if (config == NULL) {
      _oy_freed_config_error();
    }
  } else {
    config = NULL;
  }

  _oy_check_init();
  res = yices_new_context(config);
  if (res == NULL) {
    _oy_error();
  }

  v_res = caml_alloc_context();
  Store_context_val(v_res, res);
  CAMLreturn(v_res);
}

static void _oy_context_finalize(value v_context) {
  context_t *context = Context_val(v_context);
  if (context != NULL) {
    yices_free_context(context);
    Store_context_val(v_context, NULL);
  }
}

CAMLprim value ocamlyices_context_status(value v_context) {
  CAMLparam1(v_context);

  context_t *context = Context_val(v_context);
  if (context == NULL) {
    _oy_freed_context_error();
  }

  smt_status_t status = yices_context_status(context);

  CAMLreturn(Val_int((int)status));
}

CAMLprim value ocamlyices_context_reset(value v_context) {
  CAMLparam1(v_context);

  context_t *context = Context_val(v_context);
  if (context == NULL) {
    _oy_freed_context_error();
  }

  COND_MT_START(MTFLAG_STACKOP);
  yices_reset_context(context);
  COND_MT_END(MTFLAG_STACKOP);

  CAMLreturn(Val_unit);
}

static inline value _oy_context_stackop(int32_t (*f)(context_t *),
    value v_context) {
  CAMLparam1(v_context);
  int32_t res;
  context_t *context;

  context = Context_val(v_context);
  COND_MT_START(MTFLAG_STACKOP);
  res = (*f)(context);
  COND_MT_END(MTFLAG_STACKOP);
  if (res != 0) {
    _oy_error();
  }

  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_context_push(value v_ctx) {
  return _oy_context_stackop(&yices_push, v_ctx);
}

CAMLprim value ocamlyices_context_pop(value v_ctx) {
  return _oy_context_stackop(&yices_pop, v_ctx);
}

static inline value _oy_context_toggle_option(
  int32_t (*f)(context_t *, const char *),
  value v_context, value v_option
) {
  CAMLparam2(v_context, v_option);
  int32_t res;
  context_t *context = Context_val(v_context);
  const char *option = String_val(v_option);

  res = (*f)(context, option);
  if (res != 0) {
    _oy_error();
  }

  CAMLreturn(Val_unit);
  \
}

CAMLprim value ocamlyices_context_enable_option(value v_ctx, value v_opt) {
  return _oy_context_toggle_option(&yices_context_enable_option, v_ctx, v_opt);
}
CAMLprim value ocamlyices_context_disable_option(value v_ctx, value v_opt) {
  return _oy_context_toggle_option(&yices_context_disable_option, v_ctx, v_opt);
}

CAMLprim value ocamlyices_context_assert_formula(value v_context,
    value v_formula) {
  // Hyp: v_formula is a non-block value
  CAMLparam1(v_context);
  int32_t res;
  term_t formula = Int_val(v_formula);
  context_t *context = Context_val(v_context);

  COND_MT_START(MTFLAG_ASSERT);
  res = yices_assert_formula(context, formula);
  COND_MT_END(MTFLAG_ASSERT);

  if (res != 0) {
    _oy_error();
  }
  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_context_assert_formulas(value v_context,
    value v_formulas) {
  CAMLparam2(v_context, v_formulas);
  int32_t res;
  uint32_t n;
  term_t *formulas;

  n = check_Wosize_val(v_formulas);
  formulas = _oy_terms_from_values(v_formulas, n);
  if (formulas == NULL) {
    _oy_allocation_error();
  }

  COND_MT_START(MTFLAG_ASSERT);
  res = yices_assert_formulas(Context_val(v_context), n, formulas);
  COND_MT_END(MTFLAG_ASSERT);
  free(formulas);
  if (res != 0) {
    _oy_error();
  }

  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_context_check(value v_param_opt, value v_context) {
  CAMLparam2(v_param_opt, v_context);
  smt_status_t res;
  context_t *context = Context_val(v_context);
  param_t *param = (Is_block(v_param_opt) ? Params_val(Field(v_param_opt,
                    0)) : NULL);

  COND_MT_START(MTFLAG_CHECK);
  res = yices_check_context(context, param);
  COND_MT_END(MTFLAG_CHECK);

  CAMLreturn(Val_int((int)res));
}

CAMLprim value ocamlyices_context_assert_blocking_clause(value v_context) {
  CAMLparam1(v_context);
  int32_t res;
  context_t *context = Context_val(v_context);

  COND_MT_START(MTFLAG_ASSERT);
  res = yices_assert_blocking_clause(context);
  COND_MT_END(MTFLAG_ASSERT);
  if (res != 0) {
    _oy_error();
  }

  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_context_stop_search(value v_context) {
  CAMLparam1(v_context);
  context_t *context = Context_val(v_context);

  COND_MT_START(MTFLAG_STOP_SEARCH);
  yices_stop_search(context);
  COND_MT_END(MTFLAG_STOP_SEARCH);

  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_params_create(value unit) {
  CAMLparam1(unit);
  CAMLlocal1(v_res);

  param_t *res = yices_new_param_record();
  if (res == NULL) {
    _oy_error();
  }

  v_res = caml_alloc_params();
  Store_params_val(v_res, res);

  CAMLreturn(Val_int(v_res));
}

CAMLprim value ocamlyices_params_set(value v_params, value v_name,
                                     value v_value) {
  CAMLparam3(v_params, v_name, v_value);
  param_t *params = Params_val(v_params);
  const char *name = String_val(v_name);
  const char *value_ = String_val(v_value);
  int32_t res;

  res = yices_set_param(params, name, value_);
  if (res != 0) {
    _oy_error();
  }

  CAMLreturn(Val_unit);
}

static void _oy_params_finalize(value v_params) {
  param_t *params = Params_val(v_params);
  DEBUG_PRINT("finalize params at %p\n", params);
  if (params != 0) {
    Store_params_val(params, NULL);
    yices_free_param_record(params);
  }
}


