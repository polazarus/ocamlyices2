#include <yices.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/threads.h>
#include <caml/custom.h>
#include <stdio.h>

#include "yices2.h"
#include "yices2_macros.h"

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
  COND_MT_START(MTFLAG_INIT);
  yices_init();
  COND_MT_START(MTFLAG_INIT);
  CAMLreturn (unit);
}

value ocamlyices_exit(value unit) {
  CAMLparam1 (unit);
  COND_MT_START(MTFLAG_EXIT);
  yices_exit();
  COND_MT_END(MTFLAG_EXIT);
  caml_acquire_runtime_system();
  CAMLreturn (unit);
}

value ocamlyices_reset(value unit) {
  CAMLparam1 (unit);
  COND_MT_START(MTFLAG_RESET);
  yices_reset();
  COND_MT_START(MTFLAG_RESET);
  CAMLreturn (unit);
}


value ocamlyices_subst_term(value v_vars, value v_repls, value v_t) {
  CAMLparam3(v_vars, v_repls, v_t);
  term_t res, t;
  term_t *repls, *vars;
  size_t n, m;
  
  ML2C_COPY_ARRAY(v_vars, n, vars, term_t, Term_val);
  NOT_NEEDED_VALUE(v_vars);
  ML2C_COPY_ARRAY(v_repls, m, repls, term_t, Term_val);
  NOT_NEEDED_VALUE(v_repls);

  if (n != m) {
    free(vars); free(repls);
    ocamlyices_bad_array_sizes_error();
  }

  if (vars == (void*)0 || repls == (void*)0) {
    free(vars); free(repls);
    ocamlyices_allocation_error();
  }

  t = Term_val(v_t);

  res = yices_subst_term(n, vars, repls, t);
  free(vars);
  free(repls);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_int(res));
}

value ocamlyices_subst_term_array(value v_vars, value v_repls, value v_ts) {
  CAMLparam3(v_vars, v_repls, v_ts);
  term_t res;
  term_t *repls, *vars, *ts;
  size_t n, m, p;
  
  ML2C_COPY_ARRAY(v_vars, n, vars, term_t, Term_val);
  NOT_NEEDED_VALUE(v_vars);

  ML2C_COPY_ARRAY(v_repls, m, repls, term_t, Term_val);
  NOT_NEEDED_VALUE(v_repls);

  if (n != m) {
    free(vars); free(repls);
    ocamlyices_bad_array_sizes_error();
  }

  ML2C_COPY_ARRAY(v_ts, p, ts, term_t, Term_val);
  NOT_NEEDED_VALUE(v_ts);

  if (vars == (void*)0 || repls == (void*)0 || ts == (void*) 0) {
    free(vars); free(repls); free(ts);
    ocamlyices_allocation_error();
  }

  res = yices_subst_term_array(n, vars, repls, p, ts);
  free(vars);
  free(repls);
  free(ts);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_int(res));
}

OCAMLYICES_ERRVOID_FROM_X_STRING(set_type_name, set_type_name, type_t, Int_val)
OCAMLYICES_ERRVOID_FROM_X_STRING(set_term_name, set_term_name, term_t, Int_val)

OCAMLYICES_VOID_FROM_STRING(remove_type_name, remove_type_name)
OCAMLYICES_VOID_FROM_STRING(remove_term_name, remove_term_name)

value ocamlyices_get_type_by_name(value v_str) {
  CAMLparam1(v_str);
  type_t res;
  const char* str = String_val(v_str);
  /*should not early-release v_str*/

  res = yices_get_type_by_name(str);
  if (res == NULL_TYPE) ocamlyices_failure();

  CAMLreturn(Val_long(res));
}
value ocamlyices_get_term_by_name(value v_str) {
  CAMLparam1(v_str);
  term_t res;
  const char* str = String_val(v_str);
  /*should not early-release v_str*/

  res = yices_get_term_by_name(str);
  if (res == NULL_TYPE) ocamlyices_failure();

  CAMLreturn(Val_long(res));
}

OCAMLYICES_ERRVOID_FROM_X(clear_type_name, clear_type_name, type_t, Int_val)
OCAMLYICES_ERRVOID_FROM_X(clear_term_name, clear_term_name, term_t, Int_val)

// pp

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

