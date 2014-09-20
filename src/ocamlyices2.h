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
#define MTFLAG_NAMING 0
#define MTFLAG_PP 1

#endif
