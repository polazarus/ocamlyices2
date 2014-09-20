#include <yices.h>
#include <caml/mlvalues.h>
#include <caml/fail.h>

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#define NORETURN __attribute__((noreturn))
#else
#define UNUSED
#define NORETURN
#endif

#ifdef DEBUG
#define DEBUG_PRINT(s, ...) do {fprintf(stderr, "[DEBUG] " s, ##__VA_ARGS__); fflush(stderr); } while(0);
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif

void _oy_check_error();
void _oy_check_init();
int _oy_callback_print(value v_cb, int (*printfn)(FILE *, void *), void *arg);

CAMLprim value ocamlyices_info (value unit);
CAMLprim value ocamlyices_reset (value unit);



static inline NORETURN void _oy_error() {
  _oy_check_error();
  caml_failwith("Unknown error");
}

static inline NORETURN void _oy_binding_overflow_error() {
  caml_failwith("Output overflows (due to the binding, try another variant of the function)");
}

static inline NORETURN void _oy_freed_model_error() {
  caml_failwith("Illegal operation on freed model");
}

static inline NORETURN void _oy_freed_config_error() {
  caml_failwith("Illegal operation on freed config");
}

static inline NORETURN void _oy_freed_context_error() {
  caml_failwith("Illegal operation on freed context");
}

static inline NORETURN void _oy_allocation_error() {
  caml_failwith("Illegal operation on freed model");
}

static inline NORETURN void _oy_unsupported() {
  caml_failwith("Unsupported operation");
}

static inline NORETURN void _oy_invalid_array_sizes() {
  caml_invalid_argument("Arrays with different sizes");
}

static inline NORETURN void _oy_invalid_argument(const char *message) {
  caml_invalid_argument(message);
}

static inline intnat Long_option_val(value val, intnat def) {
  return Is_block(val) ? Long_val(Field(val, 0)) : def;
}

static inline uint32_t check_Wosize_val(value val) {
  mlsize_t n_raw = Wosize_val(val);
  if (Max_wosize > UINT32_MAX && n_raw > UINT32_MAX) {
    _oy_invalid_argument("array too large");
  }
  return (uint32_t) n_raw;
}


