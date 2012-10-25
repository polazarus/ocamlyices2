#include <yices.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/threads.h>
#include <caml/custom.h>

#define COND_MT_START(cond) do { if (cond) caml_release_runtime_system(); } while (0)
#define COND_MT_END(cond) do { if (cond) caml_acquire_runtime_system(); } while (0)

#define NOT_NEEDED_VALUE(v) do {v = 0;} while(0)

int mt_level = 0;

value ocamlyices_info(value unit)
{
  CAMLparam1 (unit);
  CAMLlocal1 (tuple);
  tuple = caml_alloc_tuple(5);
  Store_field(tuple, 0, caml_copy_string(yices_version));
  Store_field(tuple, 1, caml_copy_string(yices_build_arch));
  Store_field(tuple, 2, caml_copy_string(yices_build_mode));
  Store_field(tuple, 3, caml_copy_string(yices_build_date));
  CAMLreturn (tuple);
}

value ocamlyices_init(value unit)
{
  CAMLparam1 (unit);
  yices_init();
  CAMLreturn (unit);
}

value ocamlyices_exit(value unit)
{
  CAMLparam1 (unit);
  COND_MT_START(mt_level > 1);
  yices_exit();
  COND_MT_END(mt_level > 1);
  caml_acquire_runtime_system();
  CAMLreturn (unit);
}

value ocamlyices_reset(value unit)
{
  CAMLparam1 (unit);
  COND_MT_START(mt_level > 1);
  yices_reset();
  COND_MT_START(mt_level > 1);
  CAMLreturn (unit);
}

void ocamlyices_failure() {
  
}

static struct custom_operations ocamlyices_type_ops = {
  "ocamlyices.type",
  custom_finalize_default,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

#define caml_alloc_ocamlyices_type() caml_alloc_custom(&ocamlyices_type_ops, sizeof (type_t), 0, 1)
#define Store_ocamlyices_type_val(v, raw) *(type_t*)Data_custom_val(v) = raw; 
#define Ocamlyices_type_val(v) *(type_t*)Data_custom_val(v); 


#define OCAMLYICES_NULLARY_TYPE_DEF(name) \
value ocamlyices_ ## name ## _type(value unit) { \
  CAMLparam1(unit); \
  CAMLlocal1(res); \
  type_t raw_res; \
  raw_res = yices_ ## name ## _type(); \
  if (raw_res == NULL_TYPE) ocamlyices_failure();\
  res = caml_alloc_ocamlyices_type(); \
  Store_ocamlyices_type_val(res, raw_res); \
  CAMLreturn(res); \
}
OCAMLYICES_NULLARY_TYPE_DEF(bool)
OCAMLYICES_NULLARY_TYPE_DEF(int)
OCAMLYICES_NULLARY_TYPE_DEF(real)

value ocamlyices_bv_type(value size) {
  CAMLparam1(size);
  CAMLlocal1(res);
  type_t raw_res;
  uint32_t raw_size;
  
  raw_size = (uint32_t)Int_val(size);
  NOT_NEEDED_VALUE(size);

  raw_res = yices_bv_type(raw_size);
  if (raw_res == NULL_TYPE) ocamlyices_failure();  
  
  res = caml_alloc_ocamlyices_type();
  Store_ocamlyices_type_val(res, raw_res);
  CAMLreturn(res);
}

value ocamlyices_new_scalar_type(value card) {
  CAMLparam1(card);
  CAMLlocal1(res);
  type_t raw_res;
  uint32_t raw_card;

  raw_card = (uint32_t)Int_val(card);
  NOT_NEEDED_VALUE(card);

  raw_res = yices_new_scalar_type(raw_card);
  if (raw_res == NULL_TYPE) ocamlyices_failure();
  
  res = caml_alloc_ocamlyices_type(); 
  Store_ocamlyices_type_val(res, raw_res);
  CAMLreturn(res);
}

OCAMLYICES_NULLARY_TYPE_DEF(new_uninterpreted)

value ocamlyices_tuple_type(value tau) {
  CAMLparam1(tau);
  CAMLlocal1(res);
  type_t raw_res;
  type_t* raw_tau;
  unsigned int n, i;
 
  // Allocate array
  n = Wosize_val(tau);
  raw_tau = (type_t*) calloc(n, sizeof(type_t*));
  // Copy Ocaml array to C array
  for (i=0; i < n; i++) {
    raw_tau[i] = Ocamlyices_type_val(Field(tau, i));
  }
  NOT_NEEDED_VALUE(tau);
  
  raw_res = yices_tuple_type(n, raw_tau); 
  free(raw_tau);
  if (raw_res == NULL_TYPE) ocamlyices_failure();
  
  res = caml_alloc_ocamlyices_type();
  Store_ocamlyices_type_val(res, raw_res);
  CAMLreturn(res);
}

value ocamlyices_function_type(value dom, value range) {
  CAMLparam2(dom, range);
  CAMLlocal1(res);
  type_t raw_res, raw_range;
  type_t* raw_dom;
  unsigned int n, i;

  raw_range = Ocamlyices_type_val(range);
  NOT_NEEDED_VALUE(range);
   // Allocate array
  n = Wosize_val(dom);
  raw_dom = (type_t*) calloc(n, sizeof(type_t*));
  // Copy Ocaml array to C array
  for (i=0; i < n; i++) {
    raw_dom[i] = Ocamlyices_type_val(Field(dom, i));
  }
  NOT_NEEDED_VALUE(dom);

  raw_res = yices_function_type(n, raw_dom, raw_range);
  free(raw_dom);
  if (raw_res == NULL_TYPE) ocamlyices_failure();

  res = caml_alloc_ocamlyices_type();
  Store_ocamlyices_type_val(res, raw_res);
  CAMLreturn(res);
}


static struct custom_operations ocamlyices_term_ops = {
  "ocamlyices.term",
  custom_finalize_default,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

#define caml_alloc_ocamlyices_term() caml_alloc_custom(&ocamlyices_term_ops, sizeof (term_t), 0, 1)
#define Store_ocamlyices_term_val(v, raw) do { *(term_t*)Data_custom_val(v) = raw; } while (0)
#define Ocamlyices_term_val(v) *(term_t*)Data_custom_val(v); 

#define OCAMLYICES_NULLARY_TERM_DEF(name) \
value ocamlyices_ ## name (value unit) { \
  CAMLparam1(unit); \
  CAMLlocal1(res); \
  term_t raw_res; \
  raw_res = yices_ ## name (); \
  if (raw_res == NULL_TERM) ocamlyices_failure();\
  res = caml_alloc_ocamlyices_term(); \
  Store_ocamlyices_term_val(res, raw_res); \
  CAMLreturn(res); \
}

OCAMLYICES_NULLARY_TERM_DEF(true)
OCAMLYICES_NULLARY_TERM_DEF(false)

