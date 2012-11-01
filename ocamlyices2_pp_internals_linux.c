#define _GNU_SOURCE
#include <stdio.h>
#include "ocamlyices2.h"

#include <string.h>

/*
FILE *fopencookie(void *cookie, const char *mode,
  cookie_io_functions_t io_funcs);
*/

typedef struct { value cb; } ocamlyices_pp_cookie_t;

static value caml_copy_stringn(size_t size, const char* buf) {
  CAMLparam0();
  CAMLlocal1(v_buf);
  v_buf = caml_alloc_string(size);
  memcpy(&Byte(v_buf, 0), buf, size);
  CAMLreturn(v_buf);
}

static ssize_t ocamlyices_pp_write (void* cookie_, const char* buf, size_t size) {
  ocamlyices_pp_cookie_t* cookie = cookie_;
  caml_callback(cookie->cb, caml_copy_stringn(size, buf));
  return size;
}

int ocamlyices_pp_with_callback(value v_cb, int (*pp_fun)(FILE*, void*), void* arg) {
  ocamlyices_pp_cookie_t cookie = { v_cb };
  cookie_io_functions_t iofuns = { NULL, &ocamlyices_pp_write, NULL, NULL };

  FILE* output = fopencookie(&cookie, "w", iofuns);
  if (output == (void*)0) ocamlyices_failure(); // FIXME meaningfull error

  int res = pp_fun(output, arg);

  fclose(output);
  return res;
}
