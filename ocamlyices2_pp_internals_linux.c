#define _GNU_SOURCE
#include <stdio.h>
#include "ocamlyices2.h"

#include <string.h>

/*
FILE *fopencookie(void *cookie, const char *mode,
                 cookie_io_functions_t io_funcs);
*/

struct ocamlyices_internal_pp_cookie {
  value cb;
};

static ssize_t ocamlyices_internal_pp_write (void* cookie_, const char* buf, size_t size) {
  struct ocamlyices_internal_pp_cookie* cookie = cookie_;
  CAMLparam0();
  CAMLlocal1(v_buf);

  v_buf = caml_alloc_string(size);
  memcpy(&Byte(v_buf, 0), buf, size);
  caml_callback(cookie->cb, v_buf);

  CAMLreturnT(ssize_t, size);
}



int ocamlyices_internal_pp_with_callback(value v_cb, int (*pp_fun)(FILE*, void*), void* arg) {
  struct ocamlyices_internal_pp_cookie cookie = {v_cb};
  cookie_io_functions_t iofuns = {
    NULL, &ocamlyices_internal_pp_write, NULL, NULL
  };
  FILE* output = fopencookie(&cookie, "w", iofuns);
  int res = pp_fun(output, arg);
  fclose(output);
  return res;
}
