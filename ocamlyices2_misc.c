#include "ocamlyices2.h"

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
  COND_MT_END(MTFLAG_INIT);
  CAMLreturn (unit);
}

value ocamlyices_exit(value unit) {
  CAMLparam1 (unit);
  COND_MT_START(MTFLAG_EXIT);
  yices_exit();
  COND_MT_END(MTFLAG_EXIT);
  CAMLreturn (unit);
}

value ocamlyices_reset(value unit) {
  CAMLparam1 (unit);
  COND_MT_START(MTFLAG_RESET);
  yices_reset();
  COND_MT_END(MTFLAG_RESET);
  CAMLreturn (unit);
}

/* Term and type naming */

// Set name of
value ocamlyices_set_type_name (value v_t, value v_name) {
  CAMLparam2(v_t, v_name);
  int32_t res;
  COND_MT_START(MTFLAG_NAMING);
  res = yices_set_type_name(Type_val(v_t), String_val(v_name));
  COND_MT_END(MTFLAG_NAMING);
  if (res != 0) ocamlyices_failure();
  CAMLreturn(Val_unit);
}
value ocamlyices_set_term_name (value v_t, value v_name) {
  CAMLparam2(v_t, v_name);
  int32_t res;
  COND_MT_START(MTFLAG_NAMING);
  res = yices_set_term_name(Term_val(v_t), String_val(v_name));
  COND_MT_END(MTFLAG_NAMING);
  if (res != 0) ocamlyices_failure();
  CAMLreturn(Val_unit);
}
// Remove name
value ocamlyices_remove_type_name (value v_name) {
  CAMLparam1(v_name);
  COND_MT_START(MTFLAG_NAMING);
  yices_remove_type_name(String_val(v_name));
  COND_MT_END(MTFLAG_NAMING);
  CAMLreturn(Val_unit);
}
value ocamlyices_remove_term_name (value v_name) {
  CAMLparam1(v_name);
  COND_MT_START(MTFLAG_NAMING);
  yices_remove_term_name(String_val(v_name));
  COND_MT_END(MTFLAG_NAMING);
  CAMLreturn(Val_unit);
}
// Get by name
value ocamlyices_get_type_by_name(value v_str) {
  CAMLparam1(v_str);
  const char* str = String_val(v_str); /*should not early-release v_str*/
  type_t res;
  COND_MT_START(MTFLAG_NAMING);
  res = yices_get_type_by_name(str);
  COND_MT_END(MTFLAG_NAMING);
  if (res == NULL_TYPE) ocamlyices_failure();
  CAMLreturn(Val_type(res));
}
value ocamlyices_get_term_by_name(value v_str) {
  CAMLparam1(v_str);
  const char* str = String_val(v_str); /*should not early-release v_str*/
  term_t res;
  COND_MT_START(MTFLAG_NAMING);
  res = yices_get_term_by_name(str);
  COND_MT_END(MTFLAG_NAMING);
  if (res == NULL_TYPE) ocamlyices_failure();
  CAMLreturn(Val_type(res));
}
// Clear name of
value ocamlyices_clear_type_name (value v_arg) {
  CAMLparam1(v_arg);
  int32_t res;
  COND_MT_START(MTFLAG_NAMING);
  res = yices_clear_type_name(Type_val(v_arg));
  COND_MT_END(MTFLAG_NAMING);
  if (res != 0) ocamlyices_failure();
  CAMLreturn(Val_unit);
}
value ocamlyices_clear_term_name (value v_arg) {
  CAMLparam1(v_arg);
  int32_t res;
  COND_MT_START(MTFLAG_NAMING);
  res = yices_clear_term_name(Term_val(v_arg));
  COND_MT_END(MTFLAG_NAMING);
  if (res != 0) ocamlyices_failure();
  CAMLreturn(Val_unit);
}

// pp

struct pp_term_arg {
  term_t t;
  uint32_t width, height, offset;
};

int ocamlyices_internal_pp_term(FILE* output, void* arg_) {
  struct pp_term_arg* arg = arg_;
  return yices_pp_term(output, arg->t, arg->width, arg->height, arg->offset);
}

value ocamlyices_pp_term(value v_width_opt, value v_height_opt, value v_offset_opt, value v_cb, value v_t) {
  CAMLparam5(v_width_opt, v_height_opt, v_offset_opt, v_cb, v_t);
  term_t t = Term_val(v_t);
  uint32_t width = Is_block(v_width_opt) ? (uint32_t)Long_val(Field(v_width_opt, 0)) : 80;
  uint32_t height = Is_block(v_height_opt) ? (uint32_t)Long_val(Field(v_height_opt, 0)) : 1;
  uint32_t offset = Is_block(v_offset_opt) ? (uint32_t)Long_val(Field(v_offset_opt, 0)) : 0;
  struct pp_term_arg arg = { t, width, height, offset };
  int res = ocamlyices_internal_pp_with_callback(v_cb, &ocamlyices_internal_pp_term, &arg);
  if (res != 0) ocamlyices_failure();
  CAMLreturn(Val_unit);
}

/* TYPES */

#define OCAMLYICES_TYPE_FROM_TERM(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_t) {\
    CAMLparam1(v_t);\
    type_t res;\
    term_t t = Term_val(v_t);\
  \
    res = yices_ ## nameIn(t);\
    if (res == NULL_TYPE) ocamlyices_failure();\
  \
    CAMLreturn(Type_val(res));\
  }

OCAMLYICES_TYPE_FROM_TERM(type_of_term, type_of_term)

#define OCAMLYICES_BOOL_FROM_TERM(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_t) {\
    CAMLparam1(v_t);\
    term_t t = Term_val(v_t);\
  \
    int32_t res = yices_ ## nameIn (t);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_bool(res));\
  }

OCAMLYICES_BOOL_FROM_TERM(term_is_bool, term_is_bool)
OCAMLYICES_BOOL_FROM_TERM(term_is_int, term_is_int)
OCAMLYICES_BOOL_FROM_TERM(term_is_real, term_is_real)
OCAMLYICES_BOOL_FROM_TERM(term_is_arithmetic, term_is_arithmetic)
OCAMLYICES_BOOL_FROM_TERM(term_is_bitvector, term_is_bitvector)
OCAMLYICES_BOOL_FROM_TERM(term_is_tuple, term_is_tuple)
OCAMLYICES_BOOL_FROM_TERM(term_is_function, term_is_function)
OCAMLYICES_BOOL_FROM_TERM(term_is_scalar, term_is_scalar)

value ocamlyices_term_bitsize(value v_t) {
  CAMLparam1(v_t);
  term_t t = Term_val(v_t);

  int32_t res = yices_term_bitsize(t);
  if (res == 0) ocamlyices_check_failure();

  CAMLreturn(Val_long(res));
}
