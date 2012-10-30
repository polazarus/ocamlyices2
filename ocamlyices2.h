#ifndef __OCAMLYICES2_H__
#define __OCAMLYICES2_H__

#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/threads.h>
#include <caml/custom.h>
#include <caml/fail.h>

#include <gmp.h>
#define __GMP_H__
#define __GMP_H
#include <yices.h>

#define CAML_MAX_INT ((1L << (8 * sizeof(value) - 2)) - 1)
#define CAML_MIN_INT (-(1L << (8 * sizeof(value) - 2)))

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

#define caml_alloc_context() caml_alloc_custom(&ocamlyices_context_ops, sizeof (context_t*), 0, 1)
#define Store_context_val(v, raw) do { *(context_t**)Data_custom_val(v) = raw; } while (0)
#define Context_val(v) *(context_t**)Data_custom_val(v)

#define caml_alloc_config() caml_alloc_custom(&ocamlyices_ctx_config_ops, sizeof (ctx_config_t*), 0, 1)
#define Store_config_val(v, raw) do { *(ctx_config_t**)Data_custom_val(v) = raw; } while (0)
#define Config_val(v) *(ctx_config_t**)Data_custom_val(v)

#define caml_alloc_param() caml_alloc_custom(&ocamlyices_param_ops, sizeof (param_t*), 0, 1)
#define Store_ocamlyices_param_val(v, raw) do { *(param_t**)Data_custom_val(v) = raw; } while (0)
#define Ocamlyices_param_val(v) *(param_t**)Data_custom_val(v)

#define caml_alloc_model() caml_alloc_custom(&ocamlyices_model_ops, sizeof (model_t*), 0, 1)
#define Store_model_val(v, raw) do { *(model_t**)Data_custom_val(v) = raw; } while (0)
#define Model_val(v) *(model_t**)Data_custom_val(v)

#define Term_val(v) ((term_t) Long_val(v))
#define Val_term(v) Val_long(v)
#define Type_val(v) ((type_t) Long_val(v))
#define Val_type(v) Val_long(v)
#define ML2C_COPY_ARRAY(v_arr, n, arr, X_t, X_val) \
  do {\
    size_t i;\
    n = Wosize_val(v_arr);\
    arr = (X_t*) calloc(n, sizeof(X_t));\
    if (arr != (void*)0)\
      for (i = 0; i < n; i++)\
        arr[i] = X_val(Field(v_arr, i));\
  } while (0)

#define ocamlyices_context_ops ocamlyices_internal_context_ops
#define ocamlyices_ctx_config_ops ocamlyices_internal_ctx_config_ops
#define ocamlyices_param_ops ocamlyices_internal_model_ops

#define ocamlyices_check_failure ocamlyices_internal_check_failure
#define ocamlyices_failure ocamlyices_internal_failure
#define ocamlyices_eval_binding_overflow ocamlyices_internal_eval_binding_overflow
#define ocamlyices_already_freed_model ocamlyices_internal_already_freed_model
#define ocamlyices_already_freed_config ocamlyices_internal_already_freed_config
#define ocamlyices_already_freed_context ocamlyices_internal_already_freed_context
#define ocamlyices_allocation_error ocamlyices_internal_allocation_error
#define ocamlyices_bad_array_sizes_error ocamlyices_internal_bad_array_sizes_error
#define ocamlyices_invalid_argument ocamlyices_internal_invalid_argument
#define ocamlyices_pp_with_callback ocamlyices_internal_pp_with_callback

struct custom_operations
  ocamlyices_context_ops __attribute__ ((visibility ("hidden"))),
  ocamlyices_ctx_config_ops __attribute__ ((visibility ("hidden"))),
  ocamlyices_param_ops __attribute__ ((visibility ("hidden"))),
  ocamlyices_model_ops __attribute__ ((visibility ("hidden")));

__attribute__ ((visibility ("hidden"))) void ocamlyices_check_failure();
__attribute__ ((visibility ("hidden"))) void ocamlyices_failure();
__attribute__ ((visibility ("hidden"))) void ocamlyices_eval_binding_overflow();
__attribute__ ((visibility ("hidden"))) void ocamlyices_already_freed_model();
__attribute__ ((visibility ("hidden"))) void ocamlyices_already_freed_config();
__attribute__ ((visibility ("hidden"))) void ocamlyices_already_freed_context();
__attribute__ ((visibility ("hidden"))) void ocamlyices_allocation_error();
__attribute__ ((visibility ("hidden"))) void ocamlyices_bad_array_sizes_error();
__attribute__ ((visibility ("hidden"))) void ocamlyices_invalid_argument(const char*);

__attribute__ ((visibility ("hidden"))) int ocamlyices_pp_with_callback(value v_cb, int (*pp_fun)(FILE*, void*), void* arg);

#endif
