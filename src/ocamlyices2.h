#ifndef __OCAMLYICES2_H__
#define __OCAMLYICES2_H__

#include <yices.h>
#include <caml/mlvalues.h>
#include <caml/custom.h> // for custom blocks
#include <caml/threads.h> // for caml_release/acquire_runtime_system
#include <caml/alloc.h> // caml_alloc, copy_string, etc.
#include <caml/fail.h> // calml_failwith

#include <stdlib.h> // for malloc, free
#include <stdint.h> // for (u)int32_t etc.


#ifdef __cplusplus
#undef CAMLprim
#define CAMLprim extern "C"
#endif

#define COND_MT_START(cond) do { if (cond) caml_release_runtime_system(); } while (0)
#define COND_MT_END(cond) do { if (cond) caml_acquire_runtime_system(); } while (0)

#define NOT_NEEDED_VALUE(v) do {v = 0;} while(0)

#define MTFLAG_ASSERT 1
#define MTFLAG_RESET 1
#define MTFLAG_EXIT 1
#define MTFLAG_INIT 1
#define MTFLAG_STOP_SEARCH 1
#define MTFLAG_STACKOP 1
#define MTFLAG_CHECK 1
#define MTFLAG_GET_MODEL 1
#define MTFLAG_FREE_MODEL 1
#define MTFLAG_NAMING 0
#define MTFLAG_PP 1

static inline context_t *Context_val(value v) {
  return (*((context_t **)Data_custom_val(v)));
}
static inline model_t *Model_val(value v) {
  return (*((model_t **)Data_custom_val(v)));
}
static inline model_t *Mdlctx_val_model(value v) {
  return Model_val(Field(v,0));
}
static inline context_t *Mdlctx_val_context(value v) {
  return Context_val(Field(v,1));
}

#define Term_val(v) (term_t)Long_val(v)
#define Val_term(v) Val_long((intnat)v)
#define Type_val(v) (type_t)Long_val(v)
#define Val_type(v) Val_long((intnat)v)

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#define NORETURN __attribute__((noreturn))
#else
#define UNUSED
#define NORETURN
#endif

void _oy__check_error();

static inline NORETURN void _oy__error() {
  _oy__check_error();
  caml_failwith("Unknown error");
}
static inline NORETURN void _oy__binding_overflow_error() {
  caml_failwith("Output overflows (due to the binding, try another variant of the function)");
}
static inline NORETURN void _oy__freed_model_error() {
  caml_failwith("Illegal operation on freed model");
}
static inline NORETURN void _oy__freed_config_error() {
  caml_failwith("Illegal operation on freed config");
}
static inline NORETURN void _oy__freed_context_error() {
  caml_failwith("Illegal operation on freed context");
}
static inline NORETURN void _oy__allocation_error() {
  caml_failwith("Illegal operation on freed model");
}
static inline NORETURN void _oy__unsupported() {
  caml_failwith("Unsupported operation");
}
static inline NORETURN void _oy__invalid_array_sizes() {
  caml_invalid_argument("Arrays with different sizes");
}
static inline NORETURN void _oy__invalid_argument(const char *message) {
  caml_invalid_argument(message);
}

#ifdef DEBUG
#define DEBUG_PRINT(s, ...) do {fprintf(stderr, "[DEBUG] " s, ##__VA_ARGS__); fflush(stderr); } while(0);
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif

static inline uint32_t check_Wosize_val(value val) {
  mlsize_t n_raw = Wosize_val(val);
  if (Max_wosize > UINT32_MAX && n_raw > UINT32_MAX) {
    _oy__invalid_argument("array too large");
  }
  return (uint32_t) n_raw;
}

static inline term_t *_oy__terms_from_values(value v_arr, uint32_t n) {
  term_t *arr;
  uint32_t i;

  arr = (term_t *) malloc(sizeof(term_t[n]));
  if (arr) {
    for (i = 0; i < n; i++) {
      arr[i] = Term_val(Field(v_arr, i));
    }
  }

  return arr;
}

void _oy_check_init();

#endif
