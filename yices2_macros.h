/* TYPES */

#define OCAMLYICES_NULLARY_TYPE(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value unit) { \
    CAMLparam1(unit);\
    type_t res = yices_ ## nameIn ();\
  \
    if (res == NULL_TYPE) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }
  
#define OCAMLYICES_NARY_TYPE(nameIn, nameOut) \
  value ocamlyices_ ## nameOut(value tau) {\
    CAMLparam1(tau);\
    type_t raw_res;\
    type_t* raw_tau;\
    size_t n, i;\
  \
    n = Wosize_val(tau);\
    raw_tau = (type_t*) calloc(n, sizeof(type_t*));\
    for (i=0; i < n; i++) {\
      raw_tau[i] = Int_val(Field(tau, i));\
    }\
    NOT_NEEDED_VALUE(tau);\
  \
    raw_res = yices_ ## nameIn (n, raw_tau);\
    free(raw_tau);\
    if (raw_res == NULL_TYPE) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(raw_res));\
  }

#define OCAMLYICES_TYPE_FROM_TYPEs_TYPE(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value dom, value range) {\
    CAMLparam2(dom, range);\
    type_t raw_res, raw_range;\
    type_t* raw_dom;\
    size_t n, i;\
  \
    raw_range = Int_val(range);\
    n = Wosize_val(dom);\
    raw_dom = (type_t*) calloc(n, sizeof(type_t*));\
    for ( i = 0; i < n; i++) {\
      raw_dom[i] = Int_val(Field(dom, i));\
    }\
    NOT_NEEDED_VALUE(dom);\
  \
    raw_res = yices_function_type(n, raw_dom, raw_range);\
    free(raw_dom);\
    if (raw_res == NULL_TYPE) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(raw_res));\
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
  
#define OCAMLYICES_TYPE_FROM_STRING(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_str) {\
    CAMLparam1(v_str);\
    type_t res;\
    const char* str = String_val(v_str); /*should not early-release v_str*/\
  \
    res = yices_ ## nameIn (str);\
    if (res == NULL_TYPE) ocamlyices_failure();\
  \
    CAMLreturn(res);\
  }

#define OCAMLYICES_TYPE_FROM_TERM(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_t) {\
    CAMLparam1(v_t);\
    type_t res;\
    term_t t = Int_val(v_t);\
  \
    res = yices_ ## nameIn(t);\
    if (res == NULL_TYPE) ocamlyices_failure();\
  \
    CAMLreturn(res);\
  }

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
#define OCAMLYICES_TERM_FROM_INT(nameIn, nameOut) \
  OCAMLYICES_TERM_FROM_X(nameIn, nameOut, int32_t, Int_val)
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

#define OCAMLYICES_TERM_FROM_UINT_X(nameIn, nameOut, X_t, X_val) \
  OCAMLYICES_TERM_FROM_X_Y(nameIn, nameOut, uint32_t, Int_val, X_t, X_val)
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
    args = (X_t*) calloc(n, sizeof(X_t*)); \
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

#define OCAMLYICES_ARGSNBODY_TERM(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_args, value v_body) { \
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
    res = yices_ ## nameOut(n, args, body); \
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

#define OCAMLYICES_TERM_FROM_Xs_TERMs(nameIn, nameOut, X_t, X_val) \
  value ocamlyices_ ## nameOut (value v_as, value v_ts) {\
    CAMLparam2(v_as, v_ts);\
    term_t *ts, res;\
    X_t * as;\
    size_t n, i ;\
  \
    n = Wosize_val(v_as);\
    if (n != Wosize_val(v_ts)) caml_invalid_argument("arrays with different sizes");\
    as = (X_t*) calloc(n, sizeof(X_t*));\
    for (i = 0; i < n; i++) {\
      as[i] = X_val(Field(v_as, i));\
    }\
    NOT_NEEDED_VALUE(v_as);\
    ts = (term_t*) calloc(n, sizeof(term_t*));\
    for (i = 0; i < n; i++) {\
      ts[i] = Int_val(Field(v_ts, i));\
    }\
    NOT_NEEDED_VALUE(v_ts);\
  \
    res = yices_ ## nameIn (n, as, ts);\
    free(as); free(ts);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_TERM_FROM_Xs_Xs_TERMs(nameIn, nameOut, X_t, X_val) \
  value ocamlyices_ ## nameOut (value v_as, value v_bs, value v_ts) {\
    CAMLparam3(v_as, v_bs, v_ts);\
    term_t *ts, res;\
    X_t *as, *bs;\
    size_t n, i ;\
  \
    n = Wosize_val(v_as);\
    if (n != Wosize_val(v_bs) || n!= Wosize_val(v_ts)) caml_invalid_argument("arrays with different sizes");\
    as = (X_t*) calloc(n, sizeof(X_t*));\
    for (i = 0; i < n; i++) {\
      as[i] = X_val(Field(v_as, i));\
    }\
    NOT_NEEDED_VALUE(v_as);\
    bs = (X_t*) calloc(n, sizeof(X_t*));\
    for (i = 0; i < n; i++) {\
      bs[i] = X_val(Field(v_bs, i));\
    }\
    NOT_NEEDED_VALUE(v_bs);\
    ts = (term_t*) calloc(n, sizeof(term_t*));\
    for (i = 0; i < n; i++) {\
      ts[i] = Int_val(Field(v_ts, i));\
    }\
    NOT_NEEDED_VALUE(v_ts);\
  \
    res = yices_ ## nameIn (n,as,bs,ts);\
    free(as); free(bs); free(ts);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

#define OCAMLYICES_TERM_FROM_TERM_TERMs(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_fun, value v_args) {\
    CAMLparam2(v_fun, v_args);\
    term_t* args;\
    term_t fun, res;\
    size_t n, i;\
  \
    fun = Int_val(v_fun);\
    n = Wosize_val(v_args);\
    args = (term_t*) calloc(n, sizeof(term_t*));\
    for (i = 0; i < n; i++) {\
      args[i] = Int_val(Field(v_args, i));\
    }\
    NOT_NEEDED_VALUE(v_args);\
  \
    res = yices_ ## nameOut (fun, n, args);\
    free(args);\
  \
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }
#define OCAMLYICES_TERM_FROM_TERM_TERMs_TERM(nameIn, nameOut) \
  value ocamlyices_ ## nameOut(value v_fun, value v_args, value v_newv) {\
    CAMLparam3(v_fun, v_args, v_newv);\
    term_t fun, newv, res;\
    term_t* args;\
    size_t n, i ;\
  \
    fun = Int_val(v_fun);\
    newv = Int_val(v_newv);\
  \
    n = Wosize_val(v_args);\
    args = (term_t*) calloc(n, sizeof(term_t*));\
    for (i=0; i < n; i++) {\
      args[i] = Int_val(Field(v_args, i));\
    }\
    NOT_NEEDED_VALUE(v_args);\
  \
    res = yices_update(fun, n, args, newv);\
    free(args);\
    if (res == NULL_TERM) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

/* Around terms */

#define OCAMLYICES_BOOL_FROM_TERM(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_t) {\
    CAMLparam1(v_t);\
    term_t t = Int_val(v_t);\
  \
    int32_t res = yices_ ## nameIn (t);\
    if (res == -1) ocamlyices_failure();\
  \
    CAMLreturn(Val_int(res != 0));\
  }

#define OCAMLYICES_INT_FROM_TERM(nameIn, nameOut) \
  value ocamlyices_ ## nameOut (value v_t) {\
    CAMLparam1(v_t);\
    term_t t = Int_val(v_t);\
  \
    int32_t res = yices_ ## nameIn(t);\
    if (res == 0) ocamlyices_check_failure();\
  \
    CAMLreturn(Val_int(res));\
  }

static struct custom_operations ocamlyices_context_ops = {
  "ocamlyices.context",
  custom_finalize_default,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

#define caml_alloc_context_term() caml_alloc_custom(&ocamlyices_context_ops, sizeof (context_t*), 0, 1)
#define Store_ocamlyices_context_val(v, raw) do { *(context_t**)Data_custom_val(v) = raw; } while (0)
#define Ocamlyices_context_val(v) *(context_t**)Data_custom_val(v);


static struct custom_operations ocamlyices_ctx_config_ops = {
  "ocamlyices.ctx_config",
  custom_finalize_default,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

#define caml_alloc_ctx_config_term() caml_alloc_custom(&ocamlyices_ctx_config_ops, sizeof (ctx_config_t*), 0, 1)
#define Store_ocamlyices_ctx_config_val(v, raw) do { *(ctx_config_t**)Data_custom_val(v) = raw; } while (0)
#define Ocamlyices_ctx_config_val(v) *(ctx_config_t**)Data_custom_val(v); 
