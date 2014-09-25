type typ
type term
type config
type context
type params
type model

type status =
  | IDLE
  | SEARCHING
  | UNKNOWN
  | SAT
  | UNSAT
  | INTERRUPTED
  | ERROR

let string_of_status = function
  | IDLE -> "idle"
  | SEARCHING -> "searching"
  | UNKNOWN -> "unknown"
  | SAT -> "sat"
  | UNSAT -> "unsat"
  | INTERRUPTED -> "interrupted"
  | ERROR -> "error"

module Error = struct
  type code =
    (*<ERRORCODES>*)
    | NO_ERROR
    | INVALID_TYPE
    | INVALID_TERM
    | INVALID_CONSTANT_INDEX
    | INVALID_VAR_INDEX
    | INVALID_TUPLE_INDEX
    | INVALID_RATIONAL_FORMAT
    | INVALID_FLOAT_FORMAT
    | INVALID_BVBIN_FORMAT
    | INVALID_BVHEX_FORMAT
    | INVALID_BITSHIFT
    | INVALID_BVEXTRACT
    | INVALID_BITEXTRACT
    | TOO_MANY_ARGUMENTS
    | TOO_MANY_VARS
    | MAX_BVSIZE_EXCEEDED
    | DEGREE_OVERFLOW
    | DIVISION_BY_ZERO
    | POS_INT_REQUIRED
    | NONNEG_INT_REQUIRED
    | SCALAR_OR_UTYPE_REQUIRED
    | FUNCTION_REQUIRED
    | TUPLE_REQUIRED
    | VARIABLE_REQUIRED
    | ARITHTERM_REQUIRED
    | BITVECTOR_REQUIRED
    | SCALAR_TERM_REQUIRED
    | WRONG_NUMBER_OF_ARGUMENTS
    | TYPE_MISMATCH
    | INCOMPATIBLE_TYPES
    | DUPLICATE_VARIABLE
    | INCOMPATIBLE_BVSIZES
    | EMPTY_BITVECTOR
    | ARITHCONSTANT_REQUIRED
    | INVALID_MACRO
    | TOO_MANY_MACRO_PARAMS
    | TYPE_VAR_REQUIRED
    | DUPLICATE_TYPE_VAR
    | BVTYPE_REQUIRED
    | BAD_TERM_DECREF
    | BAD_TYPE_DECREF
    | INVALID_TOKEN
    | SYNTAX_ERROR
    | UNDEFINED_TYPE_NAME
    | UNDEFINED_TERM_NAME
    | REDEFINED_TYPE_NAME
    | REDEFINED_TERM_NAME
    | DUPLICATE_NAME_IN_SCALAR
    | DUPLICATE_VAR_NAME
    | INTEGER_OVERFLOW
    | INTEGER_REQUIRED
    | RATIONAL_REQUIRED
    | SYMBOL_REQUIRED
    | TYPE_REQUIRED
    | NON_CONSTANT_DIVISOR
    | NEGATIVE_BVSIZE
    | INVALID_BVCONSTANT
    | TYPE_MISMATCH_IN_DEF
    | ARITH_ERROR
    | BVARITH_ERROR
    | CTX_FREE_VAR_IN_FORMULA
    | CTX_LOGIC_NOT_SUPPORTED
    | CTX_UF_NOT_SUPPORTED
    | CTX_ARITH_NOT_SUPPORTED
    | CTX_BV_NOT_SUPPORTED
    | CTX_ARRAYS_NOT_SUPPORTED
    | CTX_QUANTIFIERS_NOT_SUPPORTED
    | CTX_LAMBDAS_NOT_SUPPORTED
    | CTX_NONLINEAR_ARITH_NOT_SUPPORTED
    | CTX_FORMULA_NOT_IDL
    | CTX_FORMULA_NOT_RDL
    | CTX_TOO_MANY_ARITH_VARS
    | CTX_TOO_MANY_ARITH_ATOMS
    | CTX_TOO_MANY_BV_VARS
    | CTX_TOO_MANY_BV_ATOMS
    | CTX_ARITH_SOLVER_EXCEPTION
    | CTX_BV_SOLVER_EXCEPTION
    | CTX_ARRAY_SOLVER_EXCEPTION
    | CTX_INVALID_OPERATION
    | CTX_OPERATION_NOT_SUPPORTED
    | CTX_INVALID_CONFIG
    | CTX_UNKNOWN_PARAMETER
    | CTX_INVALID_PARAMETER_VALUE
    | CTX_UNKNOWN_LOGIC
    | EVAL_UNKNOWN_TERM
    | EVAL_FREEVAR_IN_TERM
    | EVAL_QUANTIFIER
    | EVAL_LAMBDA
    | EVAL_OVERFLOW
    | EVAL_FAILED
    | EVAL_CONVERSION_FAILED
    | EVAL_NO_IMPLICANT
    | MDL_UNINT_REQUIRED
    | MDL_CONSTANT_REQUIRED
    | MDL_DUPLICATE_VAR
    | MDL_FTYPE_NOT_ALLOWED
    | MDL_CONSTRUCTION_FAILED
    | YVAL_INVALID_OP
    | YVAL_OVERFLOW
    | OUTPUT_ERROR
    | INTERNAL_EXCEPTION
    (*</ERRORCODES>*)

  type report = {
    name : string;
    line : int;
    column : int;
    term1 : term option;
    type1 : typ option;
    term2 : term option;
    type2 : typ option;
    badval : int64
  }
end

exception YicesError of Error.code * Error.report

let () =
  let report = let open Error in {
    name = "NO_ERROR";
    line = 0;
    column = 0;
    term1 = None;
    type1 = None;
    term2 = None;
    type2 = None;
    badval = 0L
  } in
  let exc = YicesError (Error.NO_ERROR, report) in
  Callback.register_exception "ocamlyices2.exception" exc

external info : unit -> string*string*string*string
  = "ocamlyices_info"
let version, build_arch, build_mode, build_date =
  info ()

module Experimental = struct
  (** Reset Yices *)
  external reset : unit -> unit
    = "ocamlyices_reset"
end

(******************************************************************************)

module Type = struct
  (** Get built-in type for whole numbers *)
  external int : unit -> typ
    = "ocamlyices_type_int"
  (** Get built-in type for Booleans *)
  external bool : unit -> typ
    = "ocamlyices_type_bool"
  (** Get build-in type for real numbers *)
  external real : unit -> typ
    = "ocamlyices_type_real"
  (** Get type for bitvector of given size (in bits) *)
  external bitvector : int -> typ
    = "ocamlyices_type_bitvector"
  (** Create new type for scalar of given cardinality *)
  external new_scalar : int -> typ
    = "ocamlyices_type_new_scalar"
  (** Create new uninterpreted type *)
  external new_uninterpreted : unit -> typ
    = "ocamlyices_type_new_uninterpreted"
  (** Get tuple type of provided types *)
  external tuple : typ array -> typ
    = "ocamlyices_type_tuple"
  external tuple1 : typ -> typ
    = "ocamlyices_type_tuple1"
  external tuple2 : typ -> typ -> typ
    = "ocamlyices_type_tuple2"
  external tuple3 : typ -> typ -> typ -> typ
    = "ocamlyices_type_tuple3"
  (** Get function type *)
  external func : typ array -> typ -> typ
    = "ocamlyices_type_function"
  external func1 : typ -> typ -> typ
    = "ocamlyices_type_function1"
  external func2 : typ -> typ -> typ -> typ
    = "ocamlyices_type_function2"
  external func3 : typ -> typ -> typ -> typ -> typ
    = "ocamlyices_type_function3"

  (** Parse type *)
  external parse : string -> typ
    = "ocamlyices_type_parse"

  (** Get type by name *)
  external by_name : string -> typ
    = "ocamlyices_type_by_name"
  (** Get name of type *)
  external get_name : typ -> string
    = "ocamlyices_type_get_name"
  (** Set name of type *)
  external set_name : typ -> string -> unit
    = "ocamlyices_type_set_name"
  (** Unbind type of its associated name, if any *)
  external clear_name : typ -> unit
    = "ocamlyices_type_clear_name"
  (** Unbind name of its associated type, if any *)
  external remove_name : string -> unit
    = "ocamlyices_type_remove_name"

  (** Get type of term *)
  external of_term: term -> typ
    = "ocamlyices_type_of_term"

  (** Get size of bit-vector type in bits *)
  external bitsize : typ -> int
    = "ocamlyices_type_bitsize"

  (** Check if type is boolean *)
  external is_bool : typ -> bool
    = "ocamlyices_type_is_bool"
  (** Check if type is integer *)
  external is_int : typ -> bool
    = "ocamlyices_type_is_int"
  (** Check if type is real *)
  external is_real : typ -> bool
    = "ocamlyices_type_is_real"
  (** Check if type is arithmetic *)
  external is_arithmetic : typ -> bool
    = "ocamlyices_type_is_arithmetic"
  (** Check if type is bit-vector *)
  external is_bitvector : typ -> bool
    = "ocamlyices_type_is_bitvector"
  (** Check if type is tuple *)
  external is_tuple : typ -> bool
    = "ocamlyices_type_is_tuple"
  (** Check if type is function *)
  external is_function : typ -> bool
    = "ocamlyices_type_is_function"
  (** Check if type is scalar *)
  external is_scalar : typ -> bool
    = "ocamlyices_type_is_scalar"
  (** Check if type is uninterpreted *)
  external is_uninterpreted : typ -> bool
    = "ocamlyices_type_is_uninterpreted"
  (** Check if first type is subtype of the second  *)
  external is_subtype : typ -> typ -> bool
    = "ocamlyices_type_is_subtype"

  (** Print type *)
  external print : ?width:int -> ?height:int -> ?offset:int
    -> (string -> unit) -> typ -> unit
    = "ocamlyices_type_print"
end


(******************************************************************************)

module Term = struct

  (** Scalar of given type and index. *)
  external constant : typ -> int -> term
    = "ocamlyices_term_constant"

  (** Creates new uninterpreted of type. *)
  external new_uninterpreted : typ -> term
    = "ocamlyices_term_new_uninterpreted"


  module Raw = struct
    external new_variable : typ -> term
      = "ocamlyices_term_new_variable"
    external new_variable : typ -> term
      = "ocamlyices_term_new_variable"
    external exists : term -> term -> term
      = "ocamlyices_term_exists"
    external existsn : term array -> term -> term
      = "ocamlyices_term_existsn"
    external forall : term -> term -> term
      = "ocamlyices_term_forall"
    external foralln : term array -> term -> term
      = "ocamlyices_term_foralln"
    external lambda : term -> term -> term
      = "ocamlyices_term_lambda"
    external lambdan : term array-> term -> term
      = "ocamlyices_term_lambdan"
  end

  let mk_simple raw = fun typ f ->
    let v = Raw.new_variable typ in
    raw v (f v)
  let mk_simplen raw = fun typs f ->
    let v = Array.map Raw.new_variable typs in
    raw v (f v)


  module Bool = struct
    (** Boolean for true *)
    external true_ : unit -> term
      = "ocamlyices_term_true"
    (** Boolean for false *)
    external false_ : unit -> term
      = "ocamlyices_term_false"
    (** If-then-else [ite cond iftrue iffalse] *)
    external ite : term -> term -> term -> term
      = "ocamlyices_term_ite"
    (** Equality *)
    external eq : term -> term -> term
      = "ocamlyices_term_eq"
    (** Inequality *)
    external neq : term -> term -> term
      = "ocamlyices_term_neq"
    (** Distinct, all-diff. *)
    external distinct : term array -> term
      = "ocamlyices_term_distinct"
    (** Negation *)
    external not : term -> term
      = "ocamlyices_term_not"
    (** Conjunction *)
    external and2 : term -> term -> term
      = "ocamlyices_term_and2"
    external and3 : term -> term -> term -> term
      = "ocamlyices_term_and3"
    external andn : term array -> term
      = "ocamlyices_term_andn"
    (** Disjunction *)
    external or2 : term -> term -> term
      = "ocamlyices_term_or2"
    external or3 : term -> term -> term -> term
      = "ocamlyices_term_or3"
    external orn : term array -> term
      = "ocamlyices_term_orn"
    (** Exclusive disjunction *)
    external xor2 : term -> term -> term
      = "ocamlyices_term_xor2"
    external xor3 : term -> term -> term -> term
      = "ocamlyices_term_xor3"
    external xorn : term array -> term
      = "ocamlyices_term_xorn"
    (** Logical equivalence *)
    external iff : term -> term -> term
      = "ocamlyices_term_iff"
    (** Logical implication *)
    external implies : term -> term -> term
      = "ocamlyices_term_implies"
    (** Universal quantifier *)
    let forall = mk_simple Raw.forall
    let foralln = mk_simplen Raw.foralln
    (** Existential quantifier *)
    let exists = mk_simple Raw.exists
    let existsn = mk_simplen Raw.existsn

  end

  (** Function application, update and lambda terms *)
  module Fun = struct
    (** Function application *)
    external application : term -> term array -> term
      = "ocamlyices_term_application"
    (** Function update *)
    external update : term -> term array -> term -> term
      = "ocamlyices_term_update"
    (** Lambda term *)
    let lambda = mk_simple Raw.lambda
    let lambdan = mk_simplen Raw.lambdan
  end

  (** Tuples *)
  module Tuple = struct
    (** Tuple *)
    external create : term array -> term
      = "ocamlyices_term_tuple"
    (** Tuple select *)
    external select : int -> term -> term
      = "ocamlyices_term_select"
    (** Tuple update *)
    external update : term -> int -> term -> term
      = "ocamlyices_term_tuple_update"
  end

  (** Arithmetic terms *)
  module Arith = struct
    type t = term

    (** Zero *)
    external zero : unit -> term
      = "ocamlyices_term_zero"

    external add : term -> term -> term
      = "ocamlyices_term_add"
    external addn : term array -> term
      = "ocamlyices_term_addn"
    external sub : term -> term -> term
      = "ocamlyices_term_sub"
    external neg : term -> term
      = "ocamlyices_term_neg"
    external mul : term -> term -> term
      = "ocamlyices_term_mul"
    external muln : term array -> term
      = "ocamlyices_term_muln"
    external square : term -> term
      = "ocamlyices_term_square"
    external power : term -> int -> term
      = "ocamlyices_term_power_int"
    (** Warning only upto UINT32_MAX *)
    external power_int64 : term -> int64 -> term
      = "ocamlyices_term_power_int64"
    external div : term -> term -> term
      = "ocamlyices_term_div"

    external eq : term -> term -> term
      = "ocamlyices_term_arith_eq"
    external neq : term -> term -> term
      = "ocamlyices_term_arith_neq"
    external geq : term -> term -> term
      = "ocamlyices_term_arith_geq"
    external leq : term -> term -> term
      = "ocamlyices_term_arith_leq"
    external gt : term -> term -> term
      = "ocamlyices_term_arith_gt"
    external lt : term -> term -> term
      = "ocamlyices_term_arith_lt"

    external eq0 : term -> term
      = "ocamlyices_term_arith_eq0"
    external neq0 : term -> term
      = "ocamlyices_term_arith_neq0"
    external geq0 : term -> term
      = "ocamlyices_term_arith_geq0"
    external leq0 : term -> term
      = "ocamlyices_term_arith_leq0"
    external gt0 : term -> term
      = "ocamlyices_term_arith_gt0"
    external lt0 : term -> term
      = "ocamlyices_term_arith_lt0"

  end

  (** Integer terms *)
  module Int = struct
    external of_int : int -> term
      = "ocamlyices_term_int"
    external of_int32 : int32 -> term
      = "ocamlyices_term_int32"
    external of_int64 : int64 -> term
      = "ocamlyices_term_int64"
    external of_nativeint : nativeint -> term
      = "ocamlyices_term_nativeint"
    external of_z : Z.t -> term
      = "ocamlyices_term_z"
  end

  (** Rational terms *)
  module Ratio = struct
    external of_int : int -> int -> term
      = "ocamlyices_term_rational_int"
    external of_int32 : int32 -> int32 -> term
      = "ocamlyices_term_rational_int32"
    external of_int64 : int64 -> int64 -> term
      = "ocamlyices_term_rational_int64"
    external of_nativeint : nativeint -> nativeint -> term
      = "ocamlyices_term_rational_nativeint"
    external of_q : Q.t -> term
      = "ocamlyices_term_q"

    (** Parse rational *)
    external parse_rational : string -> term
      = "ocamlyices_term_parse_rational"
    (** Parse float *)
    external parse_float : string -> term
      = "ocamlyices_term_parse_float"
  end

  (** Integer polynomial terms *)
  module Poly = struct
    external of_int : int array -> term array -> term
      = "ocamlyices_term_poly_int"
    external of_int32 : int32 array -> term array -> term
      = "ocamlyices_term_poly_int32"
    external of_int64 : int64 array -> term array -> term
      = "ocamlyices_term_poly_int64"
    external of_nativeint : nativeint array -> term array -> term
      = "ocamlyices_term_poly_nativeint"
    external of_z : Z.t array -> term array -> term
      = "ocamlyices_term_poly_z"
  end

  (** Rational polynomial terms *)
  module RatioPoly = struct
    external of_int : int array -> int array -> term array -> term
      = "ocamlyices_term_poly_rational_int"
    external of_int32 : int32 array -> int32 array -> term array -> term
      = "ocamlyices_term_poly_rational_int32"
    external of_int64 : int64 array -> int64 array -> term array -> term
      = "ocamlyices_term_poly_rational_int64"
    external of_nativeint : nativeint array -> nativeint array -> term array -> term
      = "ocamlyices_term_poly_rational_nativeint"
    external of_q : Q.t array -> term array -> term
      = "ocamlyices_term_poly_q"
  end

  (** Bit-vectors *)
  module Bitvector = struct

    external of_int : int -> int -> term
      = "ocamlyices_term_bitvector_of_int"
    external of_int32 : int -> int32 ->  term
      = "ocamlyices_term_bitvector_of_int32"
    external of_int64 : int -> int64 -> term
      = "ocamlyices_term_bitvector_of_int64"
    external of_nativeint : int -> nativeint -> term
      = "ocamlyices_term_bitvector_of_nativeint"
    external of_bools : bool array -> term
      = "ocamlyices_term_bitvector_of_bools"
    external of_terms : term array -> term
      = "ocamlyices_term_bitvector_of_terms"
    external of_bin : string -> term
      = "ocamlyices_term_bitvector_of_bin"
    external of_hex : string -> term
      = "ocamlyices_term_bitvector_of_hex"
    external of_z : int -> Z.t -> term
      = "ocamlyices_term_bitvector_of_z"

    external zero : int -> term
      = "ocamlyices_term_bitvector_zero"
    external one : int -> term
      = "ocamlyices_term_bitvector_one"
    external minus_one : int -> term
      = "ocamlyices_term_bitvector_minus_one"

    external add : term -> term -> term
      = "ocamlyices_term_bitvector_add"
    external sub : term -> term -> term
      = "ocamlyices_term_bitvector_sub"
    external neg : term -> term
      = "ocamlyices_term_bitvector_neg"
    external mul : term -> term -> term
      = "ocamlyices_term_bitvector_mul"
    external square : term -> term
      = "ocamlyices_term_bitvector_square"
    external power : term -> int -> term
      = "ocamlyices_term_bitvector_power_int"
    external power_int64 : term -> int64 -> term
      = "ocamlyices_term_bitvector_power_int64"

    external div : term -> term -> term
      = "ocamlyices_term_bitvector_div"
    external rem : term -> term -> term
      = "ocamlyices_term_bitvector_rem"
    external sdiv : term -> term -> term
      = "ocamlyices_term_bitvector_sdiv"
    external srem : term -> term -> term
      = "ocamlyices_term_bitvector_srem"
    external smod : term -> term -> term
      = "ocamlyices_term_bitvector_smod"

    external not : term -> term
      = "ocamlyices_term_bitvector_not"
    external and_ : term -> term -> term
      = "ocamlyices_term_bitvector_and"
    external or_ : term -> term -> term
      = "ocamlyices_term_bitvector_or"
    external xor : term -> term -> term
      = "ocamlyices_term_bitvector_xor"
    external nand : term -> term -> term
      = "ocamlyices_term_bitvector_nand"
    external nor : term -> term -> term
      = "ocamlyices_term_bitvector_nor"
    external xnor : term -> term -> term
      = "ocamlyices_term_bitvector_xnor"
    external shl : term -> term -> term
      = "ocamlyices_term_bitvector_shl"
    external lshr : term -> term -> term
      = "ocamlyices_term_bitvector_lshr"
    external ashr : term -> term -> term
      = "ocamlyices_term_bitvector_ashr"

    external shift_left0 : term -> int -> term
      = "ocamlyices_term_bitvector_shift_left0"
    external shift_left1 : term -> int -> term
      = "ocamlyices_term_bitvector_shift_left1"
    external shift_right0 : term -> int -> term
      = "ocamlyices_term_bitvector_shift_right0"
    external shift_right1 : term -> int -> term
      = "ocamlyices_term_bitvector_shift_right1"
    external ashift_right : term -> int -> term
      = "ocamlyices_term_bitvector_ashift_right"
    external rotate_left : term -> int -> term
      = "ocamlyices_term_bitvector_rotate_left"
    external rotate_right : term -> int -> term
      = "ocamlyices_term_bitvector_rotate_right"

    external extract : term -> int -> int -> term
      = "ocamlyices_term_bitvector_extract"

    external concat : term -> term -> term
      = "ocamlyices_term_bitvector_concat"

    external repeat : term -> int -> term
      = "ocamlyices_term_bitvector_repeat"
    external sign_extend : term -> int -> term
      = "ocamlyices_term_bitvector_sign_extend"
    external zero_extend : term -> int -> term
      = "ocamlyices_term_bitvector_zero_extend"

    external redand : term -> term
      = "ocamlyices_term_bitvector_redand"
    external redor : term -> term
      = "ocamlyices_term_bitvector_redor"
    external redcomp : term -> term -> term
      = "ocamlyices_term_bitvector_redcomp"

    external bitextract : term -> int -> term
      = "ocamlyices_term_bitvector_bitextract"

    external eq : term -> term -> term
      = "ocamlyices_term_bitvector_eq"
    external neq : term -> term -> term
      = "ocamlyices_term_bitvector_neq"
    external ge : term -> term -> term
      = "ocamlyices_term_bitvector_ge"
    external gt : term -> term -> term
      = "ocamlyices_term_bitvector_gt"
    external le : term -> term -> term
      = "ocamlyices_term_bitvector_le"
    external lt : term -> term -> term
      = "ocamlyices_term_bitvector_lt"
    external sge : term -> term -> term
      = "ocamlyices_term_bitvector_sge"
    external sgt : term -> term -> term
      = "ocamlyices_term_bitvector_sgt"
    external sle : term -> term -> term
      = "ocamlyices_term_bitvector_sle"
    external slt : term -> term -> term
      = "ocamlyices_term_bitvector_slt"

  end

  external subst : vars:term array -> repls:term array -> term -> term
    = "ocamlyices_term_subst"
  external subst_array : vars:term array -> repls:term array -> term array -> unit
    = "ocamlyices_term_subst_array"

  external parse : string -> term
    = "ocamlyices_term_parse"

  (* Term info *)

  external type_of : term -> typ
    = "ocamlyices_type_of_term"
  external is_bool : term -> bool
    = "ocamlyices_term_is_bool"
  external is_int : term -> bool
    = "ocamlyices_term_is_int"
  external is_real : term -> bool
    = "ocamlyices_term_is_real"
  external is_arithmetic : term -> bool
    = "ocamlyices_term_is_arithmetic"
  external is_bitvector : term -> bool
    = "ocamlyices_term_is_bitvector"
  external is_tuple : term -> bool
    = "ocamlyices_term_is_tuple"
  external is_function : term -> bool
    = "ocamlyices_term_is_function"
  external is_scalar : term -> bool
    = "ocamlyices_term_is_scalar"
  external bitsize : term -> int
    = "ocamlyices_term_bitsize"
  external is_ground : term -> bool
    = "ocamlyices_term_is_ground"

  external by_name : string -> term
    = "ocamlyices_term_by_name"
  external get_name : term -> string
    = "ocamlyices_term_get_name"
  external set_name : term -> string -> unit
    = "ocamlyices_term_set_name"
  external clear_name : term -> unit
    = "ocamlyices_term_clear_name"
  external remove_name : string -> unit
    = "ocamlyices_term_remove_name"

  external print : ?width:int -> ?height:int -> ?offset:int
    -> (string -> unit) -> term -> unit
    = "ocamlyices_term_print"
end

(******************************************************************************)

(** Context *)
module Context = struct

  (** Context configuration *)
  module Config = struct
    external create : unit -> config
      = "ocamlyices_config_create"
    external set : config -> string -> string -> unit
      = "ocamlyices_config_set"
    external default_for_logic : config -> string -> unit
      = "ocamlyices_config_default_for_logic"
  end


  external create : ?config:config -> unit -> context
    = "ocamlyices_context_create"

  external status : context -> status
    = "ocamlyices_context_status"
  external reset : context -> unit
    = "ocamlyices_context_reset"
  external push : context -> unit
    = "ocamlyices_context_push"
  external pop : context -> unit
    = "ocamlyices_context_pop"
  external enable_option : context -> string -> unit
    = "ocamlyices_context_enable_option"
  external disable_option : context -> string -> unit
    = "ocamlyices_context_disable_option"
  external assert_formula : context -> term -> unit
    = "ocamlyices_context_assert_formula"
  external assert_formulas : context -> term array -> unit
    = "ocamlyices_context_assert_formulas"

  (** Check parameters *)
  module Params = struct
    external create : unit -> params
      = "ocamlyices_params_create"
    external set : params -> string -> string -> unit
      = "ocamlyices_params_set"
  end

  external check : ?params:params -> context -> status
    = "ocamlyices_context_check"

  external assert_blocking_clause : context -> unit
    = "ocamlyices_context_assert_blocking_clause"
  external stop_search : context -> unit
    = "ocamlyices_context_stop_search"

  external get_model : ?keepsubst:bool -> context -> model
    = "ocamlyices_context_get_model"

end

(** Model *)
module Model = struct

  external get_bool : model -> term -> bool
    = "ocamlyices_model_get_bool"
  external get_int : model -> term -> int
    = "ocamlyices_model_get_int"
  external get_nativeint : model -> term -> nativeint
    = "ocamlyices_model_get_nativeint"
  external get_int32 : model -> term -> int32
    = "ocamlyices_model_get_int32"
  external get_int64 : model -> term -> int64
    = "ocamlyices_model_get_int64"
  external get_rational : model -> term -> int*int
    = "ocamlyices_model_get_rational_int"
  external get_rational_int32 : model -> term -> int32*int32
    = "ocamlyices_model_get_rational_int32"
  external get_rational_int64 : model -> term -> int64*int64
    = "ocamlyices_model_get_rational_int64"
  external get_rational_nativeint : model -> term -> nativeint*nativeint
    = "ocamlyices_model_get_rational_nativeint"
  external get_float : model -> term -> float
    = "ocamlyices_model_get_float"
  external get_bitvector : model -> term -> bool array
    = "ocamlyices_model_get_bitvector"
  external get_z : model -> term -> Z.t
    = "ocamlyices_model_get_z"
  external get_q : model -> term -> Q.t
    = "ocamlyices_model_get_q"
  external get_scalar : model -> term -> int
    = "ocamlyices_model_get_scalar"

  external get_as_term : model -> term -> term
    = "ocamlyices_model_get_as_term"
  external get_as_terms : model -> term array -> term array
    = "ocamlyices_model_get_as_terms"

  external print : ?width:int -> ?height:int -> ?offset:int
    -> (string -> unit) -> model -> unit
    = "ocamlyices_model_print"
end
