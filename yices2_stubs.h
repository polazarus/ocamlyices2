#include <yices.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/threads.h>
#include <caml/custom.h>

#define OCAMLYICES_NULLARY_TYPE(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value unit) { \
    CAMLparam1(unit);\
    type_t res = yices_ ## nameIn ();\
    if (res == NULL_TYPE) ocamlyices_failure();\
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_TYPE_FROM_INT(nameIn, nameOut) \
  value ocamlyices_ ## nameIn (value v_arg) {\
    CAMLparam1(v_arg);\
    type_t res;\
    uint32_t arg;\
  \
    arg = (uint32_t)Int_val(v_arg);\
  \
    res = yices_ ## nameOut (arg);\
    if (res == NULL_TYPE) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_NULLARY_TERM(nameIn, nameOut) \
  value ocamlyices_ ## nameIn (value unit) { \
    CAMLparam1(unit); \
    term_t raw_res; \
    raw_res = yices_ ## nameOut (); \
    if (raw_res == NULL_TERM) ocamlyices_failure();\
    CAMLreturn(Val_int(raw_res)); \
  }

#define OCAMLYICES_UNARY_TERM(name) \
  value ocamlyices_ ## name (value v_arg) { \
    CAMLparam1(v_arg); \
    term_t arg, res; \
   \
    arg = Int_val(v_arg); \
    NOT_NEEDED_VALUE(v_arg); \
   \
    res = yices_ ## name (arg); \
    if (res == NULL_TERM) ocamlyices_failure(); \
   \
    CAMLreturn(Val_int(res)); \
  }


#define OCAMLYICES_BINARY_TERM(name) \
  value ocamlyices_ ## name (value v_left, value v_right) { \
    CAMLparam2(v_left, v_right); \
    term_t left, right, res; \
   \
    left = Int_val(v_left); \
    right = Int_val(v_right); \
    NOT_NEEDED_VALUE(v_left); \
    NOT_NEEDED_VALUE(v_right); \
   \
    res = yices_ ## name (left, right); \
    if (res == NULL_TERM) ocamlyices_failure(); \
   \
    CAMLreturn(Val_int(res)); \
  }

#define OCAMLYICES_TERNARY_TERM(name) \
  value ocamlyices_ ## name (value v_arg1, value v_arg2, value v_arg3) { \
    CAMLparam3(v_arg1, v_arg2, v_arg3); \
    term_t arg1, arg2, arg3, res; \
   \
    arg1 = Int_val(v_arg1); \
    arg2 = Int_val(v_arg2); \
    arg3 = Int_val(v_arg3); \
    NOT_NEEDED_VALUE(v_arg1); \
    NOT_NEEDED_VALUE(v_arg2); \
    NOT_NEEDED_VALUE(v_arg3); \
   \
    res = yices_ ## name (arg1, arg2, arg3); \
    if (res == NULL_TERM) ocamlyices_failure(); \
   \
    CAMLreturn(Val_int(res)); \
  }

#define OCAMLYICES_NARY_TERM(name) \
  value ocamlyices_ ## name(value v_args) { \
    CAMLparam1(v_args); \
    term_t* args; \
    term_t res; \
    size_t n, i; \
   \
    n = Wosize_val(v_args); \
    args = (type_t*) calloc(n, sizeof(term_t*)); \
   \
    for (i = 0; i < n; i++) { \
      args[i] = Int_val(Field(v_args, i)); \
    } \
    NOT_NEEDED_VALUE(v_args); \
   \
    res = yices_ ## name(n, args); \
    if (res == NULL_TERM) ocamlyices_failure(); \
   \
    CAMLreturn(Val_int(res)); \
  }

#define OCAMLYICES_ARGSNBODY_TERM(name) \
  value ocamlyices_ ## name(value v_args, value v_body) { \
    CAMLparam2(v_args, v_body); \
    term_t* args; \
    term_t body, res; \
    size_t n, i; \
   \
    body = Int_val(v_body); \
    NOT_NEEDED_VALUE(v_body); \
    n = Wosize_val(v_args); \
    args = (type_t*) calloc(n, sizeof(term_t*)); \
   \
    for (i = 0; i < n; i++) { \
      args[i] = Int_val(Field(v_args, i)); \
    } \
    NOT_NEEDED_VALUE(v_args); \
   \
    res = yices_ ## name(n, args, body); \
    if (res == NULL_TERM) ocamlyices_failure(); \
   \
    CAMLreturn(Val_int(res)); \
  }


#define OCAMLYICES_TERM_FROM_INT(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_val) {\
    CAMLparam1(v_val);\
    term_t res; \
  \
    int32_t val = Int_val(v_val);\
    NOT_NEEDED_VALUE(v_val);\
  \
    res = yices_ ## nameIn (val);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_TERM_FROM_INT32(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_val) {\
    CAMLparam1(v_val);\
    term_t res;\
  \
    int32_t val = Int32_val(v_val);\
    NOT_NEEDED_VALUE(v_val);\
  \
    res = yices_ ## nameIn (val);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_TERM_FROM_INT64(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_val) {\
    CAMLparam1(v_val);\
    term_t res;\
  \
    int64_t val = Int64_val(v_val);\
    NOT_NEEDED_VALUE(v_val);\
  \
    res = yices_ ## nameIn (val);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_TERM_FROM_INT_INT(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_num, value v_den) {\
    CAMLparam2(v_num, v_den);\
    term_t res;\
  \
    int32_t num = Int_val(v_num);\
    int32_t den = Int_val(v_den);\
  \
    res = yices_ ## nameIn (num, den);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_TERM_FROM_INT32_INT32(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_num, value v_den) {\
    CAMLparam2(v_num, v_den);\
    term_t res;\
  \
    int32_t num = Int32_val(v_num);\
    int32_t den = Int32_val(v_den);\
    NOT_NEEDED_VALUE(v_num);\
    NOT_NEEDED_VALUE(v_den);\
  \
    res = yices_ ## nameIn (num, den);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }
#define OCAMLYICES_TERM_FROM_INT64_INT64(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_num, value v_den) {\
    CAMLparam2(v_num, v_den);\
    term_t res;\
  \
    int64_t num = Int64_val(v_num);\
    int64_t den = Int64_val(v_den);\
    NOT_NEEDED_VALUE(v_num);\
    NOT_NEEDED_VALUE(v_den);\
  \
    res = yices_ ## nameIn (num, den);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_TERM_FROM_TERM_AND_INT(name) \
  value ocamlyices_ ## name (value v_term, value v_intval) {\
    CAMLparam2(v_term, v_intval);\
    term_t res;\
  \
    term_t term = Int_val(v_term);\
    int32_t intval = Int_val(v_intval);\
  \
    res = yices_ ## name (term, intval);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_TERM_FROM_TERM_AND_INT32(name, nameExt) \
  value ocamlyices_ ## nameExt (value v_term, value v_intval) {\
    CAMLparam2(v_term, v_intval);\
    term_t res;\
  \
    term_t term = Int_val(v_term);\
    int32_t intval = Int32_val(v_intval);\
    NOT_NEEDED_VALUE(v_intval);\
  \
    res = yices_ ## name (term, intval);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_TERM_FROM_TERM_UINT(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_t, value v_n) {\
    CAMLparam2(v_t, v_n);\
    term_t res, t = Int_val(v_t);\
    uint32_t n = Int_val(v_n);\
  \
    res = yices_ ## nameIn (t, n);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(res);\
  }

#define OCAMLYICES_TERM_FROM_TERM_INT_INT(nameIn, nameOut) \
  value ocamlyices_ ## nameOut(value v_t, value v_i, value v_j) {\
    CAMLparam3(v_t, v_i, v_j);\
    term_t res, t = Int_val(v_t);\
    uint32_t i = Int_val(v_i), j = Int_val(v_j);\
  \
    res = yices_ ## nameIn(t, i, j);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(res);\
  }

#define OCAMLYICES_PARSE_TERM(name) \
  value ocamlyices_ ## name (value v_arg) { \
    CAMLparam1(v_arg); \
    term_t res;\
   \
    const char* arg = String_val(v_arg); /* no freeing for v_arg*/\
   \
    res = yices_ ## name (arg); \
    if (res == NULL_TERM) ocamlyices_failure(); \
   \
    CAMLreturn(Val_int(res)); \
  }
  
#define OCAMLYICES_TERM_FROM_TYPE(name) \
  value ocamlyices_ ## name (value v_tau) {\
    CAMLparam1(v_tau);\
    term_t res;\
  \
    type_t tau = Int_val(v_tau);\
  \
    res = yices_ ## name (tau);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_TERM_CHECK(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_t) {\
    CAMLparam1(v_t);\
    int32_t res;\
    term_t t = Int_val(v_t);\
  \
    res = yices_ ## nameIn (t);\
    if (res == -1 /* FIXME check error code */) ocamlyices_failure();\
  \
    CAMLreturn(Int_val(res != 0));\
  }
