#include "config.h"

#if defined(HAVE_FOPENCOOKIE) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <string.h>

#include <caml/callback.h>
#include <caml/memory.h> // CAMLparam & co

#include "ocamlyices2.h"

/* include automatically generated file */
#include "errors.h"

static _Bool _oy_initialized = 0;


CAMLprim value ocamlyices_info(value unit) {
  CAMLparam1 (unit);
  CAMLlocal1 (tuple);
  tuple = caml_alloc_tuple(5);
  Store_field(tuple, 0, caml_copy_string(yices_version));
  Store_field(tuple, 1, caml_copy_string(yices_build_arch));
  Store_field(tuple, 2, caml_copy_string(yices_build_mode));
  Store_field(tuple, 3, caml_copy_string(yices_build_date));
  CAMLreturn (tuple);
}

void _oy_check_init () {
  if (!_oy_initialized) {
    yices_init();
    _oy_initialized = 1;
  }
}

/*
 * yices_init, yices_exit, yices_reset does not make a lot of sense in the OCaml context
 *
 * so automatic initialization
 * and minimal support for reset
 */

CAMLprim value ocamlyices_reset (UNUSED value unit) {
  if (_oy_initialized) {
    yices_exit();
    _oy_initialized = 0;
  } else {
    yices_init();
    _oy_initialized = 1;
  }
  return Val_unit;
}

// check failure

/* precondition: val should be non-reclaimable (i.e. not a block) */
static inline value caml_alloc_set1(tag_t tag, value val) {
  value res = caml_alloc_small(1, tag);
  Field((res), 0) = val;
  return res;
}

static inline value term_option(term_t t) {
  return (t == NULL_TERM) ? Val_long(0) : caml_alloc_set1(0, Val_term(t));
}

static inline value type_option(type_t t) {
  return (t == NULL_TYPE) ? Val_long(0) : caml_alloc_set1(0, Val_type(t));
}

static value _oy__error_args[2];

void _oy__check_error() {
  error_code_t ec = yices_error_code();
  if (ec == NO_ERROR) {
    return;
  }
  DEBUG_PRINT("error %d\n", (int)ec);
  CAMLparam0();
  CAMLlocal1(temp);
  intnat lec = _oy__linear_error_code(ec);
  error_report_t *report = yices_error_report();
  DEBUG_PRINT("error %s (linear code %d))\n",_oy__linear_error_code_names[lec], lec);
  temp = caml_alloc_tuple(8);
  Store_field(temp, 0, caml_copy_string(_oy__linear_error_code_names[lec]));
  Store_field(temp, 1, Val_long(report->column));
  Store_field(temp, 2, Val_long(report->line));
  Store_field(temp, 3, term_option(report->term1));
  Store_field(temp, 4, type_option(report->type1));
  Store_field(temp, 5, term_option(report->term2));
  Store_field(temp, 6, type_option(report->type2));
  Store_field(temp, 7, caml_copy_int64(report->badval));
  _oy__error_args[0] = Val_long(lec);
  _oy__error_args[1] = temp;
  value* exc = caml_named_value("ocamlyices2.exception");
  if (exc == NULL) caml_failwith("cannot find exception");
  caml_raise_with_args(*exc, 2, _oy__error_args);
  CAMLreturn0;
}

#if defined(HAVE_FOPENCOOKIE)

static inline FILE* fopen_write_callback(void *cookie, ssize_t (*write)(void*, const char*, size_t)) {
  cookie_io_functions_t iofuns = { NULL, write, NULL, NULL };
  return fopencookie(cookie, "w", iofuns);
}

#elif defined(HAVE_FUNOPEN)

/* WARNING: from the manpages of FreeBSD and Apple, funopen is badled typed
 * (with ints everywhere instead of appropriate size_t and ssize_t)
 */

typedef struct {
  void *true_cookie;
  ssize_t (*true_write)(void*, const char*, size_t);
} cookie_wrap_t;

static int fopen_write_callback__write(void *cookie, const char *buf, int size) {
  cookie_wrap_t *cookie_wrap = (cookie_wrap_t*)cookie;
  return (int)cookie_wrap->true_write(cookie_wrap->true_cookie, buf, (size_t)size);
}

static int fopen_write_callback__close(void *cookie_wrap) {
  free(cookie_wrap);
  return 0;
}

static inline FILE* fopen_write_callback(void *cookie, ssize_t (*write)(void*, const char*, size_t)) {
  cookie_wrap_t* wrap = malloc(sizeof(cookie_wrap_t));
  wrap->true_cookie = cookie;
  wrap->true_write = write;
  return funopen((void*)wrap, NULL, fopen_write_callback__write, NULL, fopen_write_callback__close);
}

#else

#warning "Pretty printing not supported (missing fopencookie/funopen)"
static inline FILE* fopen_write_callback(UNUSED void* cookie, ssize_t (*UNUSED write)(void*, const char*, size_t)) NORETURN {
  return _oy__unsupported();
}

#endif

static value caml_copy_stringn(size_t size, const char *buf) {
  value v_buf = caml_alloc_string(size);
  memcpy(&Byte(v_buf, 0), buf, size);
  return v_buf;
}

static ssize_t _oy_pp_write (void *cookie, const char *buf, size_t size) {
  value *v_cb = (value *)cookie;
  value v_buf = caml_copy_stringn(size, buf);
  DEBUG_PRINT("call caml write with %d bytes", size);
  caml_callback(*v_cb, v_buf); // ignore output
  return (ssize_t)size;
}

// Precondition: the callback should be a root
int _oy_callback_print(value v_cb, int (*printfn)(FILE *, void *),
                                void *arg) {
  FILE *output = fopen_write_callback((void*)&v_cb, &_oy_pp_write);
  if (output == NULL) {
    _oy__error();  // FIXME meaningfull error
  }
  DEBUG_PRINT("print with callback initialized\n");
  int res = printfn(output, arg);
  fclose(output);
  return res;
}

struct pp_term_arg {
  term_t t;
  uint32_t width, height, offset;
};
struct pp_type_arg {
  type_t t;
  uint32_t width, height, offset;
};
struct pp_model_arg {
  model_t *mdl;
  uint32_t width, height, offset;
};

static inline intnat Long_option_val(value val, intnat def) {
  return Is_block(val) ? Long_val(Field(val, 0)) : def;
}

static int _oy_pp_term(FILE *output, void *arg_) {
  struct pp_term_arg *arg = (struct pp_term_arg *)arg_;
  return yices_pp_term(output, arg->t, arg->width, arg->height, arg->offset);
}
static int _oy_pp_type(FILE *output, void *arg_) {
  struct pp_type_arg *arg = (struct pp_type_arg *)arg_;
  return yices_pp_type(output, arg->t, arg->width, arg->height, arg->offset);
}
static int _oy_pp_model(FILE *output, void *arg_) {
  struct pp_model_arg *arg = (struct pp_model_arg *)arg_;
  return yices_pp_model(output, arg->mdl, arg->width, arg->height, arg->offset);
}

CAMLprim value ocamlyices_pp_term(value v_width_opt, value v_height_opt,
                         value v_offset_opt, value v_cb, value v_t) {
  CAMLparam1(v_cb);
  term_t t = Term_val(v_t);
  uint32_t width = (uint32_t)Long_option_val(v_width_opt, UINT32_MAX);
  uint32_t height = (uint32_t)Long_option_val(v_height_opt, 1);
  uint32_t offset = (uint32_t)Long_option_val(v_offset_opt, 0);
  DEBUG_PRINT("pp_term %ld %ld %ld\n", width, height, offset);
  struct pp_term_arg arg = { t, width, height, offset };
  int res = _oy_callback_print(v_cb,
            &_oy_pp_term, &arg);
  if (res != 0) {
    _oy__error();
  }
  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_pp_type(value v_width_opt, value v_height_opt,
                         value v_offset_opt, value v_cb, value v_t) {
  CAMLparam4(v_width_opt, v_height_opt, v_offset_opt, v_cb);
  type_t t = Type_val(v_t);
  uint32_t width = (uint32_t)Long_option_val(v_width_opt, UINT32_MAX);
  uint32_t height = (uint32_t)Long_option_val(v_height_opt, 1);
  uint32_t offset = (uint32_t)Long_option_val(v_offset_opt, 0);
  struct pp_type_arg arg = { t, width, height, offset };
  int res = _oy_callback_print(v_cb,
            &_oy_pp_type, &arg);
  if (res != 0) {
    _oy__error();
  }
  CAMLreturn(Val_unit);
}

CAMLprim value ocamlyices_pp_model(value v_width_opt, value v_height_opt,
                          value v_offset_opt, value v_cb, value v_mdl) {
  CAMLparam4(v_width_opt, v_height_opt, v_offset_opt, v_cb);
  model_t *mdl = Model_val(v_mdl);
  uint32_t width = (uint32_t)Long_option_val(v_width_opt, UINT32_MAX);
  uint32_t height = (uint32_t)Long_option_val(v_height_opt, 1);
  uint32_t offset = (uint32_t)Long_option_val(v_offset_opt, 0);
  struct pp_model_arg arg = { mdl, width, height, offset };
  int res = _oy_callback_print(v_cb,
            &_oy_pp_model, &arg);
  if (res != 0) {
    _oy__error();
  }
  CAMLreturn(Val_unit);
}
