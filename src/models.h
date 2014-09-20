#ifdef HAVE_GMP_H
#include <gmp.h>
#define __GMP_H__
#define __GMP_H
#endif

#include <yices.h>
#include <caml/mlvalues.h>

CAMLprim value ocamlyices_context_get_model(value, value);
CAMLprim value ocamlyices_model_get_bool(value, value);
CAMLprim value ocamlyices_model_get_int(value, value);
CAMLprim value ocamlyices_model_get_int32(value, value);
CAMLprim value ocamlyices_model_get_int64(value, value);
CAMLprim value ocamlyices_model_get_nativeint(value, value);
CAMLprim value ocamlyices_model_get_rational_int(value, value);
CAMLprim value ocamlyices_model_get_rational_int32(value, value);
CAMLprim value ocamlyices_model_get_rational_int64(value, value);
CAMLprim value ocamlyices_model_get_rational_nativeint(value, value);
CAMLprim value ocamlyices_model_get_float(value, value);
CAMLprim value ocamlyices_model_get_z(value, value);
CAMLprim value ocamlyices_model_get_bitvector(value, value);
CAMLprim value ocamlyices_model_get_scalar(value, value);
CAMLprim value ocamlyices_model_get_as_term(value, value);
CAMLprim value ocamlyices_model_get_as_terms(value, value);
