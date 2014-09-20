#ifndef __OY_TYPES_H__
#define __OY_TYPES_H__

#include <caml/mlvalues.h>
#include <yices.h>

CAMLprim value ocamlyices_type_bool (value unit);
CAMLprim value ocamlyices_type_int (value unit);
CAMLprim value ocamlyices_type_real (value unit);
CAMLprim value ocamlyices_type_new_uninterpreted (value unit);
CAMLprim value ocamlyices_type_bitvector (value bitsize);
CAMLprim value ocamlyices_type_new_scalar (value card);
CAMLprim value ocamlyices_type_tuple (value taus);
CAMLprim value ocamlyices_type_tuple1 (value tau1);
CAMLprim value ocamlyices_type_tuple2 (value tau1, value tau2);
CAMLprim value ocamlyices_type_tuple3 (value tau1, value tau2, value tau3);
CAMLprim value ocamlyices_type_function (value dom, value range);
CAMLprim value ocamlyices_type_function1 (value dom1, value range);
CAMLprim value ocamlyices_type_function2 (value dom1, value dom2, value range);
CAMLprim value ocamlyices_type_function3 (value dom1, value dom2, value dom3, value range);
CAMLprim value ocamlyices_type_parse (value str);
CAMLprim value ocamlyices_type_set_name (value t, value name);
CAMLprim value ocamlyices_type_remove_name (value name);
CAMLprim value ocamlyices_type_clear_name (value typ);
CAMLprim value ocamlyices_type_get_name (value typ);
CAMLprim value ocamlyices_type_by_name (value name);
CAMLprim value ocamlyices_type_bitsize (value typ);
CAMLprim value ocamlyices_type_of_term (value t);
CAMLprim value ocamlyices_type_is_bool (value t);
CAMLprim value ocamlyices_type_is_int (value t);
CAMLprim value ocamlyices_type_is_real (value t);
CAMLprim value ocamlyices_type_is_arithmetic (value t);
CAMLprim value ocamlyices_type_is_bitvector (value t);
CAMLprim value ocamlyices_type_is_tuple (value t);
CAMLprim value ocamlyices_type_is_function (value t);
CAMLprim value ocamlyices_type_is_scalar (value t);
CAMLprim value ocamlyices_type_is_uninterpreted (value t);
CAMLprim value ocamlyices_type_is_subtype (value t1, value t2);
CAMLprim value ocamlyices_type_print (value width_opt, value height_opt, value offset_opt, value cb, value t);

static inline type_t Type_val(value v) {
  return (type_t)Long_val(v);
}

static inline value Val_type(type_t typ) {
  return Val_long((intnat)typ);
}

#endif
