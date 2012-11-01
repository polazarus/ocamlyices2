#include "config.h"
#include "ocamlyices2.h"

#define OCAMLYICES_NULLARY_TYPE(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value unit) {\
    CAMLparam1(unit);\
    type_t res = yices_ ## nameIn ();\
    if (res == NULL_TYPE) ocamlyices_failure();\
    CAMLreturn(Val_type(res));\
  }

OCAMLYICES_NULLARY_TYPE(bool_type, bool_type)
OCAMLYICES_NULLARY_TYPE(int_type, int_type)
OCAMLYICES_NULLARY_TYPE(real_type, real_type)
OCAMLYICES_NULLARY_TYPE(new_uninterpreted_type, new_uninterpreted_type)

#define OCAMLYICES_TYPE_FROM_INDEX(nameIn, nameOut) \
  value ocamlyices_ ## nameIn (value v_arg) {\
    CAMLparam1(v_arg);\
    type_t res;\
  \
    intnat arg = Long_val(v_arg);\
    if (arg < 0 && (sizeof(intnat) == 2 || arg > UINT32_MAX)) \
      ocamlyices_invalid_argument("Argument should be between 0 and UINT32_MAX");\
  \
    res = yices_ ## nameOut ((uint32_t)arg);\
    if (res == NULL_TYPE) ocamlyices_failure();\
  \
    CAMLreturn(Val_type(res));\
  }

OCAMLYICES_TYPE_FROM_INDEX(bv_type, bv_type)
OCAMLYICES_TYPE_FROM_INDEX(new_scalar_type, new_scalar_type)

value ocamlyices_tuple_type(value v_taus) {
  CAMLparam1(v_taus);
  type_t res, *taus;
  size_t n;

  ML2C_COPY_ARRAY(v_taus, n, taus, type_t, Val_type);
  NOT_NEEDED_VALUE(v_taus);
  if (taus == (void*)0) ocamlyices_allocation_error();

  res = yices_tuple_type(n, taus);
  free(taus);

  if (res == NULL_TYPE) ocamlyices_failure();

  CAMLreturn(Val_type(res));
}

value ocamlyices_function_type (value v_dom, value v_range) {
  CAMLparam2(v_dom, v_range);
  type_t res, *dom, range;
  size_t n;

  range = Type_val(v_range);
  ML2C_COPY_ARRAY(v_dom, n, dom, type_t, Val_type);
  NOT_NEEDED_VALUE(v_dom);
  if (dom == (void*)0) ocamlyices_allocation_error();

  res = yices_function_type(n, dom, range);
  free(dom);

  if (res == NULL_TYPE) ocamlyices_failure();

  CAMLreturn(Val_type(res));
}


value ocamlyices_parse_type(value v_str) {
  CAMLparam1(v_str);
  type_t res;
  const char* str = String_val(v_str); /*should not early-release v_str*/

  res = yices_parse_type(str);
  if (res == NULL_TYPE) ocamlyices_failure();

  CAMLreturn(Val_type(res));
}
