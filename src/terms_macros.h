/* TERMS */


static inline value _oy_bitvector_const(term_t (*f)(uint32_t), value v_n) {
  intnat n = Long_val(v_n);
  term_t res;
  if (n < 0) {
    _oy_invalid_argument("negative integer");
  }
  if (sizeof(intnat) > sizeof(int32_t) && n > UINT32_MAX) {
    _oy_invalid_argument("integer too large");
  }
  res = (*f)((uint32_t)n);
  if (res == NULL_TERM) {
    _oy_error();
  }
  return Val_term(res);
}

static inline value _oy_nullary(term_t (*f)()) {
  term_t res = (*f)();
  if (res == NULL_TERM) {
    _oy_error();
  }
  return Val_term(res);
}

static inline value _oy_unary(term_t (*f)(term_t), value v_arg) {
  term_t res = (*f)(Term_val(v_arg));
  if (res == NULL_TERM) {
    _oy_error();
  }
  return Val_term(res);
}

static inline value _oy_binary(term_t (*f)(term_t, term_t), value v_arg1,
                                value v_arg2) {
  term_t res = (*f)(Term_val(v_arg1), Term_val(v_arg2));
  if (res == NULL_TERM) {
    _oy_error();
  }
  return Val_term(res);
}

static inline value _oy_ternary(term_t (*f)(term_t, term_t, term_t),
                                 value v_arg1, value v_arg2, value v_arg3) {
  term_t res = (*f)(Term_val(v_arg1), Term_val(v_arg2), Term_val(v_arg3));
  if (res == NULL_TERM) {
    _oy_error();
  }
  return Val_term(res);
}

static inline intnat *_oy_nativeints_from_int_values(value v_arr, uint32_t n) {
  intnat *arr;
  uint32_t i;

  arr = (intnat *) malloc(sizeof(intnat[n]));
  if (arr) {
    for (i = 0; i < n; i++) {
      arr[i] = Long_val(Field(v_arr, i));
    }
  }

  return arr;
}

static inline intnat *_oy_nativeints_from_nativeint_values(value v_arr,
    uint32_t n) {
  intnat *arr;
  uint32_t i;

  arr = (intnat *) malloc(sizeof(intnat[n]));
  if (arr) {
    for (i = 0; i < n; i++) {
      arr[i] = Nativeint_val(Field(v_arr, i));
    }
  }

  return arr;
}

static inline int32_t *_oy_int32s_from_int32_values(value v_arr, uint32_t n) {
  int32_t *arr;
  uint32_t i;

  arr = (int32_t *) malloc(sizeof(int32_t[n]));
  if (arr) {
    for (i = 0; i < n; i++) {
      arr[i] = Int32_val(Field(v_arr, i));
    }
  }

  return arr;
}

static inline int64_t *_oy_int64s_from_int64_values(value v_arr, uint32_t n) {
  int64_t *arr;
  uint32_t i;

  arr = (int64_t *) malloc(sizeof(int64_t[n]));
  if (arr) {
    for (i = 0; i < n; i++) {
      arr[i] = Int64_val(Field(v_arr, i));
    }
  }

  return arr;
}


static inline value _oy_nary_const(term_t (*f)(uint32_t, const term_t *),
                                    value v_args) {
  CAMLparam1(v_args);
  term_t *args;
  term_t res;

  uint32_t n = check_Wosize_val(v_args);
  args = _oy_terms_from_values(v_args, n);
  if (args == NULL) {
    _oy_allocation_error();
  }

  res = (*f)(n, args);
  free(args);
  if (res == NULL_TERM) {
    _oy_error();
  }
  CAMLreturn(Val_term(res));
}

static inline value _oy_nary(term_t (*f)(uint32_t, term_t *), value v_args) {
  CAMLparam1(v_args);
  term_t *args;
  term_t res;

  uint32_t n = check_Wosize_val(v_args);
  args = _oy_terms_from_values(v_args, n);
  if (args == NULL) {
    _oy_allocation_error();
  }

  res = (*f)(n, args);
  free(args);
  if (res == NULL_TERM) {
    _oy_error();
  }
  CAMLreturn(Val_term(res));
}


static inline value _oy_unary_quantifier(term_t (*q) (uint32_t n, term_t *,
    term_t), value v_arg, value v_body) {
  term_t arg = Term_val(v_arg);
  term_t res = (*q)(1, &arg, Term_val(v_body));
  if (res == NULL_TERM) {
    _oy_error();
  }
  return Val_term(res);
}

static inline value _oy_nary_quantifier(term_t (*q) (uint32_t n, term_t *,
    term_t),
    value v_args, value v_body) {
  CAMLparam1(v_args);
  term_t res, *args;

  uint32_t n = check_Wosize_val(v_args);
  args = _oy_terms_from_values(v_args, n);
  if (args == NULL) {
    _oy_allocation_error();
  }

  res = (*q)(n, args, Term_val(v_body));
  free(args);
  if (res == NULL_TERM) {
    _oy_error();
  }
  CAMLreturn(Val_term(res));
}

static inline value _oy_parser(term_t (*f) (const char *), value v_arg) {
  CAMLparam1(v_arg);
  term_t res;
  const char *arg = String_val(v_arg);
  _oy_check_init();

  res = (*f)(arg);
  if (res == NULL_TERM) {
    _oy_error();
  }

  CAMLreturn(Val_term(res));
}

static inline value _oy_term_of_term_MLintCuint32(term_t (*f)(term_t, uint32_t),
    value v_t, value v_d,
    const char *negative, const char *toolarge) {
  term_t res;

  intnat d = Long_val(v_d);

  if (d < 0) {
    _oy_invalid_argument(negative);
  }
  if (sizeof(intnat) > sizeof(uint32_t) && d > UINT32_MAX) {
    _oy_invalid_argument(toolarge);
  }

  res = (*f)(Term_val(v_t), (uint32_t)d);
  if (res == NULL_TERM) {
    _oy_error();
  }

  return Val_term(res);
}

static inline value _oy_term_of_term_degree(term_t (*f)(term_t, uint32_t),
    value v_t, value v_d) {
  return _oy_term_of_term_MLintCuint32(f, v_t, v_d, "negative degree",
                                       "degree too large");
}

static inline value _oy_term_of_term_degree64(term_t (*f)(term_t, uint32_t),
    value v_t, value v_d) {
  CAMLparam1(v_d);
  term_t res;

  int64_t d = Int64_val(v_d);

  if (d < 0) {
    _oy_invalid_argument("negative degree");
  }
  if (d > UINT32_MAX) {
    _oy_invalid_argument("degree too large");
  }

  res = (*f)(Term_val(v_t), (uint32_t) d);
  if (res == NULL_TERM) {
    _oy_error();
  }

  CAMLreturn(Val_term(res));
}

static inline value _oy_term_of_term_shift(term_t (*f)(term_t, uint32_t),
    value v_t, value v_d) {
  return _oy_term_of_term_MLintCuint32(f, v_t, v_d, "negative shift",
                                       "shift too large");
}

static inline value _oy_term_of_term_index(term_t (*f)(term_t, uint32_t),
    value v_t, value v_d) {
  return _oy_term_of_term_MLintCuint32(f, v_t, v_d, "negative index",
                                       "index too large");
}


static inline value _oy_bool_of_term(int32_t (*f)(term_t), value v_t) {
  int32_t res = (*f)(Term_val(v_t));
  if (res == 0) {
    _oy_check_error();
  }
  return Val_bool(res);
}

