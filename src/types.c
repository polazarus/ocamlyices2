#include "config.h"
#include "types.h"

#include <stdlib.h> // for malloc, free
#include <stdint.h> // for (u)int32_t etc.

#include <yices.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h> // copy_string

#include "terms.h"
#include "misc.h"


// Utilities

static inline value _oy_nullary_type(type_t (*f)()) {
  type_t res = (*f)();
  if (res == NULL_TYPE) {
    _oy_error();
  }

  return Val_type(res);
}

static inline value _oy_bool_of_type (int32_t (*f) (type_t), value v_t) {
  CAMLparam1(v_t);
  int32_t res = f(Type_val(v_t));
  if (res == 0) {
    _oy_check_error();
  }
  CAMLreturn(Val_bool(res));
}


static inline value _oy_type_of_MLintCuint32(type_t (*f)(uint32_t), value v_arg,
    const char *negative, const char *toolarge) {
  CAMLparam1(v_arg);
  type_t res;

  intnat arg = Long_val (v_arg);
  if (arg < 0) {
    _oy_invalid_argument(negative);
  }
  if (Max_long > UINT32_MAX && arg > UINT32_MAX) {
    _oy_invalid_argument(toolarge);
  }

  res = (*f)((uint32_t)arg);
  if (res == NULL_TYPE) {
    _oy_error();
  }

  CAMLreturn(Val_type(res));
}

static inline type_t *_oy_types_from_values(value v_arr, uint32_t n) {
  uint32_t i;
  type_t *arr = (type_t *)malloc(sizeof(type_t[n]));
  if (arr) {
    for (i = 0; i < n; i++) {
      arr[i] = Type_val(Field(v_arr, i));
    }
  }
  return arr;
}

// Exported bindings

CAMLprim value ocamlyices_type_bool(UNUSED value _unit) {
  _oy_check_init();
  return _oy_nullary_type(&yices_bool_type);
}
CAMLprim value ocamlyices_type_int(UNUSED value unit) {
  _oy_check_init();
  return _oy_nullary_type(&yices_int_type);
}
CAMLprim value ocamlyices_type_real(UNUSED value unit) {
  _oy_check_init();
  return _oy_nullary_type(&yices_real_type);
}
CAMLprim value ocamlyices_type_new_uninterpreted(UNUSED value unit) {
  _oy_check_init();
  return _oy_nullary_type(&yices_new_uninterpreted_type);
}

CAMLprim value ocamlyices_type_bitvector(value v_bitsize) {
  _oy_check_init();
  return _oy_type_of_MLintCuint32(&yices_bv_type, v_bitsize,
                                  "negative bitsize", "bitsize too large");
}

CAMLprim value ocamlyices_type_new_scalar(value v_card) {
  _oy_check_init();
  return _oy_type_of_MLintCuint32(&yices_new_scalar_type, v_card,
                                  "negative cardinality", "cardinality too large");
}

CAMLprim value ocamlyices_type_tuple(value v_taus) {
  CAMLparam1(v_taus);
  type_t res, *taus;
  uint32_t n;

  n = check_Wosize_val(v_taus);
  taus = _oy_types_from_values(v_taus, n);
  if (taus == NULL) {
    _oy_allocation_error();
  }

  res = yices_tuple_type(n, taus);
  free(taus);

  if (res == NULL_TYPE) {
    _oy_error();
  }

  CAMLreturn(Val_type(res));
}

CAMLprim value ocamlyices_type_tuple1(value v_tau1) {
  CAMLparam1(v_tau1);
  type_t res;

  res = yices_tuple_type1(Type_val(v_tau1));
  if (res == NULL_TYPE) {
    _oy_error();
  }

  CAMLreturn(Val_type(res));
}

CAMLprim value ocamlyices_type_tuple2(value v_tau1, value v_tau2) {
  CAMLparam2(v_tau1, v_tau2);
  type_t res;

  res = yices_tuple_type2(Type_val(v_tau1), Type_val(v_tau2));
  if (res == NULL_TYPE) {
    _oy_error();
  }

  CAMLreturn(Val_type(res));
}

CAMLprim value ocamlyices_type_tuple3(value v_tau1, value v_tau2,
                                      value v_tau3) {
  CAMLparam3(v_tau1, v_tau2, v_tau3);
  type_t res;

  res = yices_tuple_type3(Type_val(v_tau1), Type_val(v_tau2), Type_val(v_tau3));
  if (res == NULL_TYPE) {
    _oy_error();
  }

  CAMLreturn(Val_type(res));
}

CAMLprim value ocamlyices_type_function (value v_dom, value v_range) {
  CAMLparam2(v_dom, v_range);
  type_t res, *dom;
  uint32_t n;

  n = check_Wosize_val(v_dom);
  dom = _oy_types_from_values(v_dom, n);
  if (dom == NULL) {
    _oy_allocation_error();
  }

  res = yices_function_type(n, dom, Type_val(v_range));
  free(dom);

  if (res == NULL_TYPE) {
    _oy_error();
  }

  CAMLreturn(Val_type(res));
}

CAMLprim value ocamlyices_type_function1(value v_dom1, value v_range) {
  CAMLparam2(v_dom1, v_range);
  type_t res;

  res = yices_function_type1(Type_val(v_dom1), Type_val(v_range));
  if (res == NULL_TYPE) {
    _oy_error();
  }

  CAMLreturn(Val_type(res));
}

CAMLprim value ocamlyices_type_function2(value v_dom1, value v_dom2,
    value v_range) {
  CAMLparam3(v_dom1, v_dom2, v_range);
  type_t res;

  res = yices_function_type2(Type_val(v_dom1), Type_val(v_dom2),
                             Type_val(v_range));
  if (res == NULL_TYPE) {
    _oy_error();
  }

  CAMLreturn(Val_type(res));
}

CAMLprim value ocamlyices_type_function3(value v_dom1, value v_dom2,
    value v_dom3, value v_range) {
  CAMLparam4(v_dom1, v_dom2, v_dom3, v_range);
  type_t res;

  res = yices_function_type3(Type_val(v_dom1), Type_val(v_dom2), Type_val(v_dom3),
                             Type_val(v_range));
  if (res == NULL_TYPE) {
    _oy_error();
  }

  CAMLreturn(Val_type(res));
}


CAMLprim value ocamlyices_type_parse (value v_str) {
  CAMLparam1(v_str);
  type_t res;
  _oy_check_init();

  res = yices_parse_type(String_val(v_str));
  if (res == NULL_TYPE) {
    _oy_error();
  }

  CAMLreturn(Val_type(res));
}

// NAMES

CAMLprim value ocamlyices_type_set_name (value v_t, value v_name) {
  // Hyp: type values are non-block
  CAMLparam1(v_name);
  int32_t res;

  res = yices_set_type_name(Type_val(v_t), String_val(v_name));
  if (res != 0) {
    _oy_error();
  }

  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_type_remove_name (value v_name) {
  CAMLparam1(v_name);
  _oy_check_init();

  yices_remove_type_name(String_val(v_name));

  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_type_clear_name (value v_typ) {
  // Hyp: type values are non-block
  int32_t res;

  res = yices_clear_type_name(Type_val(v_typ));
  if (res != 0) {
    _oy_error();
  }

  return Val_unit;
}

CAMLprim value ocamlyices_type_get_name (value v_typ) {
  // Hyp: type values are non-block
  const char *res;

  res = yices_get_type_name(Type_val(v_typ));
  if (res == NULL) {
    _oy_error();
  }

  return caml_copy_string(res);
}

CAMLprim value ocamlyices_type_by_name (value v_name) {
  CAMLparam1(v_name);
  type_t res;
  _oy_check_init();

  res = yices_get_type_by_name(String_val(v_name));
  if (res == NULL_TYPE) {
    _oy_error();
  }

  CAMLreturn(Val_type(res));
}

// Bitsize

CAMLprim value ocamlyices_type_bitsize (value v_typ) {
  // Hyp: type values are non-block

  uint32_t bitsize = yices_bvtype_size(Type_val(v_typ));
  if (bitsize == 0) {
    _oy_error();
  }

  if (YICES_MAX_BVSIZE > Max_long /* should not happen */ && bitsize > Max_long) {
    _oy_binding_overflow_error();
  }

  return Val_long((intnat)bitsize);
}

// Type of term

CAMLprim value ocamlyices_type_of_term(value v_t) {
  // Hyp: term and type values are non-block

  type_t res = yices_type_of_term(Term_val(v_t));
  if (res == NULL_TYPE) {
    _oy_error();
  }

  return Val_type(res);
}

// Type testing

CAMLprim value ocamlyices_type_is_bool(value v_t) {
  return _oy_bool_of_type(&yices_type_is_bool, v_t);
}

CAMLprim value ocamlyices_type_is_int(value v_t) {
  return _oy_bool_of_type(&yices_type_is_int, v_t);
}

CAMLprim value ocamlyices_type_is_real(value v_t) {
  return _oy_bool_of_type(&yices_type_is_real, v_t);
}

CAMLprim value ocamlyices_type_is_arithmetic(value v_t) {
  return _oy_bool_of_type(&yices_type_is_arithmetic, v_t);
}

CAMLprim value ocamlyices_type_is_bitvector(value v_t) {
  return _oy_bool_of_type(&yices_type_is_bitvector, v_t);
}

CAMLprim value ocamlyices_type_is_tuple(value v_t) {
  return _oy_bool_of_type(&yices_type_is_tuple, v_t);
}

CAMLprim value ocamlyices_type_is_function(value v_t) {
  return _oy_bool_of_type(&yices_type_is_function, v_t);
}

CAMLprim value ocamlyices_type_is_scalar(value v_t) {
  return _oy_bool_of_type(&yices_type_is_scalar, v_t);
}

CAMLprim value ocamlyices_type_is_uninterpreted(value v_t) {
  return _oy_bool_of_type(&yices_type_is_uninterpreted, v_t);
}

CAMLprim value ocamlyices_type_is_subtype(value v_t1, value v_t2) {
  CAMLparam2(v_t1, v_t2);
  int32_t res;

  res = yices_test_subtype(Type_val(v_t1), Type_val(v_t2));
  if (res == 0) {
    _oy_check_error();
  }

  CAMLreturn(Val_bool(res != 0));
}

// Pretty printing

struct pp_type_arg {
  type_t t;
  uint32_t width, height, offset;
};

static int _oy_type_pp(FILE *output, void *arg_) {
  struct pp_type_arg *arg = (struct pp_type_arg *)arg_;
  return yices_pp_type(output, arg->t, arg->width, arg->height, arg->offset);
}

CAMLprim value ocamlyices_type_print(value v_width_opt, value v_height_opt,
                                     value v_offset_opt, value v_cb, value v_t) {
  CAMLparam4(v_width_opt, v_height_opt, v_offset_opt, v_cb);
  type_t t = Type_val(v_t);
  uint32_t width = (uint32_t)Long_option_val(v_width_opt, UINT32_MAX);
  uint32_t height = (uint32_t)Long_option_val(v_height_opt, 1);
  uint32_t offset = (uint32_t)Long_option_val(v_offset_opt, 0);
  struct pp_type_arg arg = { t, width, height, offset };
  int res = _oy_callback_print(v_cb, &_oy_type_pp, &arg);
  if (res != 0) {
    _oy_error();
  }
  CAMLreturn(Val_unit);
}

