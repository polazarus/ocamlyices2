#include "config.h"
#ifdef HAVE_FOPENCOOKIE

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "ocamlyices2.h"
#include <caml/callback.h>

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

#else

#include <unistd.h>
#include <pthread.h>
#include "ocamlyices2.h"
#include <caml/callback.h>

typedef struct {
  int fd;
  int (*pp_fun)(FILE*, void*);
  void* arg;
} pp_thread_data_t;

void* ocamlyices_internal_pp_thread(void* data_) {
  pp_thread_data_t* data = data_;
  FILE* output = fdopen(data->fd, "w");
  int res = (*data->pp_fun)(output, data->arg);
  fclose(output);
  return (void*) res;
}

int ocamlyices_internal_pp_with_callback(value v_cb, int (*pp_fun)(FILE*, void*), void* arg) {
  int pipefd[2], pipe_res, ptc_res;
  pthread_t thread;
  pp_thread_data_t data;
  char buffer[256];
  size_t nread;

  COND_MT_START(MTFLAG_PP);

  pipe_res = pipe(pipefd);
  if (pipe_res != 0) {
    COND_MT_START(MTFLAG_PP);
    ocamlyices_failure(); // cannot pipe
  }

  data.fd = pipefd[1];
  data.pp_fun = pp_fun;
  data.arg = arg;
  ptc_res = pthread_create(&thread, NULL, &ocamlyices_internal_pp_thread, &data);
  if (ptc_res != 0) {
    close(pipefd[0]); close(pipefd[1]);
    ocamlyices_failure(); // cannot create thread
  }

  while(1) {
    nread = read(pipefd[0], buffer, 255);
    if (nread > 0) {
      buffer[nread] = '\0';
      COND_MT_END(MTFLAG_PP);
      caml_callback(v_cb, caml_copy_string(buffer));
      COND_MT_START(MTFLAG_PP);
    } else {
      break;
    }
  }
  if (nread < 0) {
    COND_MT_START(MTFLAG_PP);
    ocamlyices_failure(); // io error
  }
  void* join_retval;
  pthread_join(thread, &join_retval);
  COND_MT_END(MTFLAG_PP);
  
  return (int) join_retval;
}

#endif
