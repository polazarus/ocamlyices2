#include "yices2_stubs.h"

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

OCAMLYICES_NULLARY_TYPE(bool_type, bool_type)
OCAMLYICES_NULLARY_TYPE(int_type, int_type)
OCAMLYICES_NULLARY_TYPE(real_type, real_type)
OCAMLYICES_TYPE_FROM_INT(bv_type, bv_type)
OCAMLYICES_TYPE_FROM_INT(new_scalar_type, new_scalar_type)
OCAMLYICES_NULLARY_TYPE(new_uninterpreted_type, new_uninterpreted_type)

value ocamlyices_tuple_type(value tau) {
  CAMLparam1(tau);
  type_t raw_res;
  type_t* raw_tau;
  size_t n, i;
 
  // Allocate array
  n = Wosize_val(tau);
  raw_tau = (type_t*) calloc(n, sizeof(type_t*));
  // Copy Ocaml array to C array
  for (i=0; i < n; i++) {
    raw_tau[i] = Int_val(Field(tau, i));
  }
  NOT_NEEDED_VALUE(tau);
  
  raw_res = yices_tuple_type(n, raw_tau); 
  free(raw_tau);
  if (raw_res == NULL_TYPE) ocamlyices_failure();
  
  CAMLreturn(Val_int(raw_res));
}

value ocamlyices_function_type(value dom, value range) {
  CAMLparam2(dom, range);
  type_t raw_res, raw_range;
  type_t* raw_dom;
  size_t n, i;

  raw_range = Int_val(range);
  NOT_NEEDED_VALUE(range);
  // Allocate array
  n = Wosize_val(dom);
  raw_dom = (type_t*) calloc(n, sizeof(type_t*));
  // Copy Ocaml array to C array
  for (i=0; i < n; i++) {
    raw_dom[i] = Int_val(Field(dom, i));
  }
  NOT_NEEDED_VALUE(dom);

  raw_res = yices_function_type(n, raw_dom, raw_range);
  free(raw_dom);
  if (raw_res == NULL_TYPE) ocamlyices_failure();

  CAMLreturn(Val_int(raw_res));
}
 
OCAMLYICES_NULLARY_TERM(true, true)
OCAMLYICES_NULLARY_TERM(false, false)

value ocamlyices_constant (value v_tau, value v_index) {
  CAMLparam2(v_tau, v_index);
  term_t res;

  type_t tau = Int_val(v_tau);
  int32_t index = Int_val(v_index);
  NOT_NEEDED_VALUE(v_tau);
  NOT_NEEDED_VALUE(v_index);

  res = yices_constant(tau, index);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_int(res));
}

OCAMLYICES_TERM_FROM_TYPE(new_uninterpreted_term)
OCAMLYICES_TERM_FROM_TYPE(new_variable)

value ocamlyices_application(value v_fun, value v_args) {
  CAMLparam2(v_fun, v_args);
  term_t* args;
  term_t fun, res;
  size_t n, i;
  
  fun = Int_val(v_fun);
  NOT_NEEDED_VALUE(v_fun);
  
  // Allocate array
  n = Wosize_val(v_args);
  args = (type_t*) calloc(n, sizeof(term_t*));
  // Copy Ocaml array to C array
  for (i=0; i < n; i++) {
    args[i] = Int_val(Field(v_args, i));
  }
  NOT_NEEDED_VALUE(v_args);

  res = yices_application(fun, n, args);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_int(res));
}

OCAMLYICES_TERNARY_TERM(ite)

OCAMLYICES_BINARY_TERM(eq)
OCAMLYICES_BINARY_TERM(neq)

OCAMLYICES_UNARY_TERM(not)


OCAMLYICES_NARY_TERM(and)
OCAMLYICES_NARY_TERM(or)
OCAMLYICES_NARY_TERM(xor)

OCAMLYICES_BINARY_TERM(and2)
OCAMLYICES_BINARY_TERM(or2)
OCAMLYICES_BINARY_TERM(xor2)

OCAMLYICES_TERNARY_TERM(and3)
OCAMLYICES_TERNARY_TERM(or3)
OCAMLYICES_TERNARY_TERM(xor3)
OCAMLYICES_BINARY_TERM(iff)
OCAMLYICES_BINARY_TERM(implies)

OCAMLYICES_NARY_TERM(tuple)

value ocamlyices_select(value v_index, value v_tuple) {
  CAMLparam2(v_index, v_tuple);
  term_t tuple, res;
  
  uint32_t index = Int_val(v_index);
  tuple = Int_val(v_tuple);
  
  res = yices_select(index, tuple);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_int(res));
}

value ocamlyices_tuple_update(value v_tuple, value v_index, value v_newv) {
  CAMLparam3(v_tuple, v_index, v_newv);
  term_t tuple, newv, res;
  uint32_t index;
  
  index = Int_val(v_index);
  tuple = Int_val(v_tuple);
  v_newv = Int_val(v_newv);
  
  res = yices_tuple_update(tuple, index, newv);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_int(res));
}

value ocamlyices_update(value v_fun, value v_args, value v_newv) {
  CAMLparam3(v_fun, v_args, v_newv);
  term_t fun, newv, res;
  term_t* args;
  size_t n, i ;
  
  fun = Int_val(v_fun);
  newv = Int_val(v_newv);

  n = Wosize_val(v_args);
  args = (type_t*) calloc(n, sizeof(term_t*));
  for (i=0; i < n; i++) {
    args[i] = Int_val(Field(v_args, i));
  }
  NOT_NEEDED_VALUE(v_args);
  
  res = yices_update(fun, n, args, newv);
  if (res == NULL_TERM) ocamlyices_failure();

  CAMLreturn(Val_int(res));
}

OCAMLYICES_NARY_TERM(distinct)

OCAMLYICES_ARGSNBODY_TERM(forall)
OCAMLYICES_ARGSNBODY_TERM(exists)
OCAMLYICES_ARGSNBODY_TERM(lambda)

OCAMLYICES_NULLARY_TERM(zero, zero)

OCAMLYICES_TERM_FROM_INT(int32, int)
OCAMLYICES_TERM_FROM_INT32(int32, int32)
OCAMLYICES_TERM_FROM_INT64(int64, int64)

OCAMLYICES_TERM_FROM_INT_INT(rational32, rational)
OCAMLYICES_TERM_FROM_INT32_INT32(rational32, rational32)
OCAMLYICES_TERM_FROM_INT64_INT64(rational64, rational64)

OCAMLYICES_PARSE_TERM(parse_rational)
OCAMLYICES_PARSE_TERM(parse_float)

OCAMLYICES_BINARY_TERM(add)
OCAMLYICES_BINARY_TERM(sub)
OCAMLYICES_UNARY_TERM(neg)
OCAMLYICES_BINARY_TERM(mul)
OCAMLYICES_UNARY_TERM(square)
OCAMLYICES_TERM_FROM_TERM_AND_INT(power)
OCAMLYICES_TERM_FROM_TERM_AND_INT32(power,power32)

//OCAMLYICES_TERM_FROM_INTS_TERMS(poly_int32, poly_int)
//OCAMLYICES_TERM_FROM_INT32S_TERMS(poly_int32, poly_int32)
//OCAMLYICES_TERM_FROM_INT64S_TERMS(poly_int64, poly_int64)

//OCAMLYICES_TERM_FROM_INTS_INTS_TERMS(poly_rational32, poly_rational)
//OCAMLYICES_TERM_FROM_INT32S_INT32S_TERMS(poly_rational32, poly_rational32)
//OCAMLYICES_TERM_FROM_INT64S_INT64S_TERMS(poly_rational64, poly_rational64)


OCAMLYICES_BINARY_TERM(arith_eq_atom)
OCAMLYICES_BINARY_TERM(arith_neq_atom)
OCAMLYICES_BINARY_TERM(arith_geq_atom)
OCAMLYICES_BINARY_TERM(arith_leq_atom)
OCAMLYICES_BINARY_TERM(arith_gt_atom)
OCAMLYICES_BINARY_TERM(arith_lt_atom)

OCAMLYICES_UNARY_TERM(arith_eq0_atom)
OCAMLYICES_UNARY_TERM(arith_neq0_atom)
OCAMLYICES_UNARY_TERM(arith_geq0_atom)
OCAMLYICES_UNARY_TERM(arith_leq0_atom)
OCAMLYICES_UNARY_TERM(arith_gt0_atom)
OCAMLYICES_UNARY_TERM(arith_lt0_atom)

//__YICES_DLLSPEC__ extern term_t yices_bvconst_uint32(uint32_t n, uint32_t x);
//__YICES_DLLSPEC__ extern term_t yices_bvconst_uint64(uint32_t n, uint64_t x);

OCAMLYICES_TERM_FROM_INT(bvconst_zero, bvconst_zero)
OCAMLYICES_TERM_FROM_INT(bvconst_one, bvconst_one)
OCAMLYICES_TERM_FROM_INT(bvconst_minus_one, bvconst_minus_one)

// bvconst_from_array
OCAMLYICES_PARSE_TERM(parse_bvbin)
OCAMLYICES_PARSE_TERM(parse_bvhex)

OCAMLYICES_BINARY_TERM(bvadd)
OCAMLYICES_BINARY_TERM(bvsub)
OCAMLYICES_UNARY_TERM(bvneg)
OCAMLYICES_BINARY_TERM(bvmul)
OCAMLYICES_UNARY_TERM(bvsquare)

OCAMLYICES_TERM_FROM_TERM_AND_INT(bvpower)
OCAMLYICES_TERM_FROM_TERM_AND_INT32(bvpower,bvpower32)

OCAMLYICES_BINARY_TERM(bvdiv)
OCAMLYICES_BINARY_TERM(bvrem)
OCAMLYICES_BINARY_TERM(bvsdiv)
OCAMLYICES_BINARY_TERM(bvsrem)
OCAMLYICES_BINARY_TERM(bvsmod)

OCAMLYICES_UNARY_TERM(bvnot)
OCAMLYICES_BINARY_TERM(bvand)
OCAMLYICES_BINARY_TERM(bvor)
OCAMLYICES_BINARY_TERM(bvxor)
OCAMLYICES_BINARY_TERM(bvnand)
OCAMLYICES_BINARY_TERM(bvnor)
OCAMLYICES_BINARY_TERM(bvxnor)
OCAMLYICES_BINARY_TERM(bvshl)
OCAMLYICES_BINARY_TERM(bvlshr)
OCAMLYICES_BINARY_TERM(bvashr)

OCAMLYICES_TERM_FROM_TERM_UINT(shift_left0, shift_left0)
OCAMLYICES_TERM_FROM_TERM_UINT(shift_left1, shift_left1)
OCAMLYICES_TERM_FROM_TERM_UINT(shift_right0, shift_right0)
OCAMLYICES_TERM_FROM_TERM_UINT(shift_right1, shift_right1)
OCAMLYICES_TERM_FROM_TERM_UINT(ashift_right, ashift_right)
OCAMLYICES_TERM_FROM_TERM_UINT(rotate_left, rotate_left)
OCAMLYICES_TERM_FROM_TERM_UINT(rotate_right, rotate_right)

OCAMLYICES_TERM_FROM_TERM_INT_INT(bvextract, bvextract)

OCAMLYICES_BINARY_TERM(bvconcat)

OCAMLYICES_TERM_FROM_TERM_UINT(bvrepeat, bvrepeat)
OCAMLYICES_TERM_FROM_TERM_UINT(sign_extend, sign_extend)
OCAMLYICES_TERM_FROM_TERM_UINT(zero_extend, zero_extend)

OCAMLYICES_UNARY_TERM(redand)
OCAMLYICES_UNARY_TERM(redor)
OCAMLYICES_BINARY_TERM(redcomp)

// bvarray

OCAMLYICES_TERM_FROM_TERM_UINT(bitextract, bitextract)

OCAMLYICES_BINARY_TERM(bveq_atom)
OCAMLYICES_BINARY_TERM(bvneq_atom)
OCAMLYICES_BINARY_TERM(bvge_atom)
OCAMLYICES_BINARY_TERM(bvgt_atom)
OCAMLYICES_BINARY_TERM(bvle_atom)
OCAMLYICES_BINARY_TERM(bvlt_atom)
OCAMLYICES_BINARY_TERM(bvsge_atom)
OCAMLYICES_BINARY_TERM(bvsgt_atom)
OCAMLYICES_BINARY_TERM(bvsle_atom)
OCAMLYICES_BINARY_TERM(bvslt_atom)

// parse_type
OCAMLYICES_PARSE_TERM(parse_term)



value ocamlyices_type_of_term(value v_t) {
  CAMLparam1(v_t);
  type_t res;
  term_t t = Int_val(v_t);
  
  res = yices_type_of_term(t);
  if (res == NULL_TYPE) ocamlyices_failure();

  CAMLreturn(res);
}

OCAMLYICES_TERM_CHECK(term_is_bool, term_is_bool)
OCAMLYICES_TERM_CHECK(term_is_int, term_is_int)
OCAMLYICES_TERM_CHECK(term_is_real, term_is_real)
OCAMLYICES_TERM_CHECK(term_is_arithmetic, term_is_arithmetic)
OCAMLYICES_TERM_CHECK(term_is_bitvector, term_is_bitvector)
OCAMLYICES_TERM_CHECK(term_is_tuple, term_is_tuple)
OCAMLYICES_TERM_CHECK(term_is_function, term_is_function)
OCAMLYICES_TERM_CHECK(term_is_scalar, term_is_scalar)

value ocamlyices_term_bitsize(value v_t) {
  CAMLparam1(v_t);
  uint32_t res;
  term_t t = Int_val(v_t);
  
  res = yices_type_of_term(t);
  if (res == 0 /* FIXME check error code */) ocamlyices_failure();

  CAMLreturn(Val_int(res));
}

