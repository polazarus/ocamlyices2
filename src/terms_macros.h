/* TERMS */

#define OCAMLYICES_NULLARY_TERM(nameIn, nameOut) \
  value ocamlyices_ ## nameIn (value unit) { \
    CAMLparam1(unit); \
    term_t raw_res; \
    raw_res = yices_ ## nameOut (); \
    if (raw_res == NULL_TERM) ocamlyices_failure();\
    CAMLreturn(Val_int(raw_res)); \
  }

#define OCAMLYICES_UNARY_TERM(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X(nameIn, nameOut, term_t, Int_val)
#define OCAMLYICES_BINARY_TERM(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X_Y(nameIn, nameOut, term_t, Int_val, term_t, Int_val)
#define OCAMLYICES_TERNARY_TERM(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X_Y_Z(nameIn, nameOut, term_t, Int_val, term_t, Int_val, term_t, Int_val)
#define OCAMLYICES_NARY_TERM(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_Xs(nameIn, nameOut, term_t, Int_val)

#define OCAMLYICES_TERM_FROM_TYPE(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X(nameIn, nameOut, type_t, Int_val)
#define OCAMLYICES_TERM_FROM_INT32(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X(nameIn, nameOut, int32_t, Int32_val)
#define OCAMLYICES_TERM_FROM_INT64(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X(nameIn, nameOut, int32_t, Int64_val)
#define OCAMLYICES_TERM_FROM_INTs(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_Xs(nameIn, nameOut, int32_t, Int_val)

#define OCAMLYICES_TERM_FROM_UINT_TERM(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X_Y(nameIn, nameOut, uint32_t, Int_val, term_t, Int_val)
#define OCAMLYICES_TERM_FROM_TYPE_INT(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X_Y(nameIn, nameOut, type_t, Int_val, int32_t, Int_val)
#define OCAMLYICES_TERM_FROM_TERM_UINT(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X_Y(nameIn, nameOut, term_t, Int_val, uint32_t, Int_val)
#define OCAMLYICES_TERM_FROM_TERM_UINT32(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X_Y(nameIn, nameOut, term_t, Int_val, uint32_t, Int32_val)

#define OCAMLYICES_TERM_FROM_TERM_UINT_TERM(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X_Y_Z(nameIn, nameOut, term_t, Int_val, uint32_t, Int_val, term_t, Int_val)
#define OCAMLYICES_TERM_FROM_TERM_INT_INT(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X_Y_Z(nameIn, nameOut, term_t, Int_val, uint32_t, Int_val, uint32_t, Int_val)

#define OCAMLYICES_TERM_FROM_X_X(nameIn, nameOut, X_t, X_val) \
  OCAMLYICES_TERM_FROM_X_Y(nameIn, nameOut, X_t, X_val, X_t, X_val)

#define OCAMLYICES_TERM_FROM_X(nameIn, nameOut, X_t, X_val) \
  value ocamlyices_ ## nameOut (value v_val) {\
    CAMLparam1(v_val);\
    term_t res;\
  \
    X_t val = X_val(v_val);\
    NOT_NEEDED_VALUE(v_val);\
  \
    res = yices_ ## nameIn (val);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }
#define OCAMLYICES_TERM_FROM_X_Y(nameIn, nameOut, X_t, X_val, Y_t, Y_val) \
  value ocamlyices_ ## nameOut (value v_x, value v_y) {\
    CAMLparam2(v_x, v_y);\
    term_t res;\
  \
    X_t x = X_val(v_x);\
    Y_t y = Y_val(v_y);\
    NOT_NEEDED_VALUE(v_x);\
    NOT_NEEDED_VALUE(v_y);\
  \
    res = yices_ ## nameIn (x, y);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }
#define OCAMLYICES_TERM_FROM_X_Y_Z(nameIn, nameOut, X_t, X_val, Y_t, Y_val, Z_t, Z_val) \
  value ocamlyices_ ## nameOut (value v_arg1, value v_arg2, value v_arg3) { \
    CAMLparam3(v_arg1, v_arg2, v_arg3);\
    X_t arg1; Y_t arg2; Z_t arg3; term_t res;\
  \
    arg1 = X_val(v_arg1);\
    arg2 = Y_val(v_arg2);\
    arg3 = Z_val(v_arg3);\
    NOT_NEEDED_VALUE(v_arg1);\
    NOT_NEEDED_VALUE(v_arg2);\
    NOT_NEEDED_VALUE(v_arg3);\
  \
    res = yices_ ## nameIn (arg1, arg2, arg3);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }
#define OCAMLYICES_TERM_FROM_Xs(nameIn, nameOut, X_t, X_val) \
  value ocamlyices_ ## nameOut (value v_args) { \
    CAMLparam1(v_args); \
    X_t* args; \
    term_t res; \
    size_t n, i; \
   \
    n = Wosize_val(v_args); \
    args = (X_t*) malloc(sizeof(X_t[n])); \
   \
    for (i = 0; i < n; i++) { \
      args[i] = X_val(Field(v_args, i)); \
    } \
    NOT_NEEDED_VALUE(v_args); \
   \
    res = yices_ ## nameIn(n, args); \
    free(args);\
    if (res == NULL_TERM) ocamlyices_failure(); \
   \
    CAMLreturn(Val_int(res)); \
  }

#define OCAMLYICES_TERM_FROM_STRING(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_arg) { \
    CAMLparam1(v_arg); \
    term_t res;\
    const char* arg = String_val(v_arg); /* no early-releasing of v_arg*/\
   \
    res = yices_ ## nameIn (arg); \
    if (res == NULL_TERM) ocamlyices_failure(); \
   \
    CAMLreturn(Val_int(res)); \
  }

#define OCAMLYICES_TERM_FROM_TERM_TERMs(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_fun, value v_args) {\
    CAMLparam2(v_fun, v_args);\
    term_t fun, res, *args;\
    size_t n;\
  \
    fun = Int_val(v_fun);\
  \
    ML2C_COPY_ARRAY(v_args, n, args, term_t, Term_val);\
    NOT_NEEDED_VALUE(v_args);\
    if (args == (void*)0) ocamlyices_allocation_error();\
  \
    res = yices_ ## nameIn (fun, n, args);\
    free(args);\
  \
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

/* Around terms */

#define OCAMLYICES_ERRVOID_FROM_X_STRING(nameIn, nameOut, X_t, X_val) \
  value ocamlyices_ ## nameOut (value v_tau, value v_name) {\
    CAMLparam2(v_tau, v_name);\
    X_t tau = X_val(v_tau);\
    const char* name = String_val(v_name);\
    int32_t res = yices_ ## nameIn (tau, name);\
    if (res != 0) ocamlyices_failure();\
    CAMLreturn(Val_int(0));\
  }
#define OCAMLYICES_VOID_FROM_STRING(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_name) {\
    CAMLparam1(v_name);\
    const char* name = String_val(v_name);\
    yices_ ## nameIn (name);\
    CAMLreturn(Val_int(0));\
  }
#define OCAMLYICES_ERRVOID_FROM_STRING(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_name) {\
    CAMLparam1(v_name);\
    const char* name = String_val(v_name);\
    int32_t res = yices_ ## nameIn (name);\
    if (res != 0) ocamlyices_failure();\
    CAMLreturn(Val_int(0));\
  }

#define OCAMLYICES_VOID_FROM_CONTEXT(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_context) {\
    CAMLparam1(v_context);\
    yices_ ## nameIn (Ocamlyices_context_val(v_context));\
    CAMLreturn(Val_int(0));\
  }
