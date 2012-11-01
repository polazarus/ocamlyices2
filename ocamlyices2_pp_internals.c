#include "ocamlyices2.h"
#include <unistd.h>
#include <pthread.h>

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
