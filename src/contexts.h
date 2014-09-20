#ifndef __OY_CONTEXTS_H__
#define __OY_CONTEXTS_H__

#include <yices.h>
#include <caml/mlvalues.h>

CAMLprim value ocamlyices_config_create(value);
CAMLprim value ocamlyices_config_set(value, value, value);
CAMLprim value ocamlyices_config_default_for_logic(value, value);
CAMLprim value ocamlyices_context_create(value, value);
CAMLprim value ocamlyices_context_status(value);
CAMLprim value ocamlyices_context_reset(value);
CAMLprim value ocamlyices_context_push(value);
CAMLprim value ocamlyices_context_pop(value);
CAMLprim value ocamlyices_context_enable_option(value, value);
CAMLprim value ocamlyices_context_disable_option(value, value);
CAMLprim value ocamlyices_context_assert_formula(value, value);
CAMLprim value ocamlyices_context_assert_formulas(value, value);
CAMLprim value ocamlyices_context_check(value, value);
CAMLprim value ocamlyices_context_assert_blocking_clause(value);
CAMLprim value ocamlyices_context_stop_search(value);
CAMLprim value ocamlyices_params_create(value);
CAMLprim value ocamlyices_params_set(value, value, value);

static inline context_t *Context_val(value v) {
  return (*((context_t **)Data_custom_val(v)));
}

#endif
