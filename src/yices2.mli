val version : string
val build_arch : string
val build_mode : string
val build_date : string

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

val string_of_status : status -> string

module Error : sig
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

(** [register_exn] must be called before any use of Yices2 functions.
    In most cases, the main function of the Yices2 package will call
    [register_exn]) by itself, so you won't have to do anything.
    But in some cases, the main wont be executed and will need to
    run [register_exn] yourself. *)
val register_exn : unit -> unit

(** On some systems, like mingw or alpine, fopen or fcookieopen
   are not available and thus the functions Type.print,
   Term.print and Model.print are not available.
   This function tells you if it is available. *)
external print_supported : unit -> bool
  = "ocamlyices_print_supported"

module Experimental : sig
  (** Reset Yices *)
  external reset : unit -> unit
    = "ocamlyices_reset"
end

(******************************************************************************)

(** Types

    Type constructors and type testing.
  *)
module Type : sig

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

  (** Get type of term *)
  external of_term: term -> typ
    = "ocamlyices_type_of_term"

  (** {2 Type naming} *)

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

  (** {2 Type testing} *)

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

  (** {2 Pretty printing} *)

  (** Print type *)
  external print : ?width:int -> ?height:int -> ?offset:int
    -> (string -> unit) -> typ -> unit
    = "ocamlyices_type_print"
end


(******************************************************************************)

module Term : sig

  (** Scalar of given type and index. *)
  external constant : typ -> int -> term
    = "ocamlyices_term_constant"

  (** Creates new uninterpreted of type. *)
  external new_uninterpreted : typ -> term
    = "ocamlyices_term_new_uninterpreted"

  (** Boolean terms *)
  module Bool : sig
    (** Boolean for true *)
    external true_ : unit -> term
      = "ocamlyices_term_true"

    (** Boolean for false *)
    external false_ : unit -> term
      = "ocamlyices_term_false"

    (** If-then-else [ite cond iftrue iffalse] *)
    external ite : term -> term -> term -> term
      = "ocamlyices_term_ite"

    (** Generic equality *)
    external eq : term -> term -> term
      = "ocamlyices_term_eq"

    (** Generic inequality *)
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
    val forall : typ -> (term -> term) -> term
    val foralln : typ array -> (term array -> term) -> term

    (** Existential quantifier *)
    val exists : typ -> (term -> term) -> term
    val existsn : typ array -> (term array -> term) -> term

  end

  (** Function application, update and lambda terms *)
  module Fun : sig
    (** Function application *)
    external application : term -> term array -> term
      = "ocamlyices_term_application"
    (** Function update *)
    external update : term -> term array -> term -> term
      = "ocamlyices_term_update"
    (** Lambda term *)
    val lambda : typ -> (term -> term) -> term
    val lambdan : typ array -> (term array -> term) -> term
  end

  (** Tuples *)
  module Tuple : sig
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

  (**
   Arithmetic terms

   All functions return arithmetic terms except for comparators.
   *)
  module Arith : sig

    (** {2 Constructors}

        Only generic arithmetic constructors. For more constructors, see:
        {ul
          {- {!Int}}
          {- {!Ratio}}
          {- {!Poly}}
          {- {!RatioPoly}}
        }
     *)

    (** Zero (generic) *)
    external zero : unit -> term
      = "ocamlyices_term_zero"

    (** Binary addition *)
    external add : term -> term -> term
      = "ocamlyices_term_add"

    (** N-ary addition *)
    external addn : term array -> term
      = "ocamlyices_term_addn"

    (** Substract *)
    external sub : term -> term -> term
      = "ocamlyices_term_sub"

    (** Negate *)
    external neg : term -> term
      = "ocamlyices_term_neg"

    (** Binary multiplication *)
    external mul : term -> term -> term
      = "ocamlyices_term_mul"

    (** N-ary multiplication *)
    external muln : term array -> term
      = "ocamlyices_term_muln"

    (** Power of two *)
    external square : term -> term
      = "ocamlyices_term_square"

    (** Power of fixed integer *)
    external power : term -> int -> term
      = "ocamlyices_term_power_int"

    (** Power of fixed integer, up to UINT32_MAX *)
    external power_int64 : term -> int64 -> term
      = "ocamlyices_term_power_int64"

    (** Division *)
    external div : term -> term -> term
      = "ocamlyices_term_div"

    (** {2 Comparators} *)

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

    (** {3 Comparators to zero} *)

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
  module Int : sig

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
  module Ratio : sig
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
  module Poly : sig
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
  module RatioPoly : sig
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
  module Bitvector : sig

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

  (** {2 Term substitution} *)

  external subst : vars:term array -> repls:term array -> term -> term
    = "ocamlyices_term_subst"
  external subst_array : vars:term array -> repls:term array
    -> term array -> unit
    = "ocamlyices_term_subst_array"

  (** {2 Term info and type testing} *)

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

  (** {2 Term naming} *)

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

  (** {2 Parsing and pretty printing} *)

  (** Parse a term in Yices format *)
  external parse : string -> term
    = "ocamlyices_term_parse"

  (** Print a term *)
  external print : ?width:int -> ?height:int -> ?offset:int
    -> (string -> unit) -> term -> unit
    = "ocamlyices_term_print"
end

(******************************************************************************)

(** Context *)
module Context : sig

  (** Context configuration

  {[
         name    |    value            |      meaning
----------------------------------------------------------------------------------------
         "mode"  | "one-shot"          |  only one call to check is supported
                 |                     |
                 | "multi-checks"      |  several calls to assert and check are
                 |                     |  possible
                 |                     |
                 | "push-pop"          |  like multi-check and with support for
                 |                     |  retracting assertions (via push/pop)
                 |                     |
                 | "interactive"       |  like push-pop, but with automatic context clean
                 |                     |  up when search is interrupted.
----------------------------------------------------------------------------------------
 "uf-solver"     | "default"           |  the uf-solver is included (i.e., the egraph)
                 | "none"              |  no uf-solver
----------------------------------------------------------------------------------------
 "bv-solver"     | "default"           |  the bitvector solver is included
                 | "none"              |  no bitvector solver
----------------------------------------------------------------------------------------
 "array-solver"  | "default"           |  the array solver is included
                 | "none"              |  no array solver
----------------------------------------------------------------------------------------
 "arith-solver"  | "ifw"               |  solver for IDL, based on the Floyd-Warshall
                 |                     |  algorithm
                 |                     |
                 | "rfw"               |  solver for RDL, based on Floyd-Warshall
                 |                     |
                 | "simplex"           |  solver for linear arithmetic, based on Simplex
                 |                     |
                 | "default"           |  same as "simplex"
                 |                     |
                 | "auto"              |  same as "simplex" unless mode="one-shot" and
                 |                     |  logic is QF_IDL or QF_RDL, in which case the
                 |                     |  solver is determined after the first call to
                 |                     |  yices_assert_formula(s).
                 |                     |
                 | "none"              |  no arithmetic solver
----------------------------------------------------------------------------------------
"arith-fragment" | "IDL"               |  integer difference logic
                 | "RDL"               |  real difference logic
                 | "LIA"               |  linear integer arithmetic
                 | "LRA"               |  linear real arithmetic
                 | "LIRA"              |  mixed linear arithmetic (real + integer variables)
]}
   *)
  module Config : sig
    external create : unit -> config
      = "ocamlyices_config_create"
    external set : config -> string -> string -> unit
      = "ocamlyices_config_set"
    external default_for_logic : config -> string -> unit
      = "ocamlyices_config_default_for_logic"
  end


  (** Create a context *)
  external create : ?config:config -> unit -> context
    = "ocamlyices_context_create"

  (** Get status. *)
  external status : context -> status
    = "ocamlyices_context_status"


  (** {2 Options}
    {ul
      {- [var-elim]: whether to eliminate variables by substitution}
      {- [arith-elim]: more variable elimination for arithmetic (Gaussian elimination)}
      {- [bvarith-elim]: more variable elimination for bitvector arithmetic}
      {- [eager-arith-lemmas]: if enabled and the simplex solver is used, the simplex solver will eagerly generate lemmas such as [(x >= 1) => (x >= 0)] (i.e., the lemmas that involve two inequalities on the same variable [x]).}
      {- [flatten]: whether to flatten nested [(or ...)]. If this is enabled the term [(or (or a b) (or c d) ]) is flattened to [(or a b c d)].}
      {- [learn-eq]: enable/disable heuristics to learn implied equalities}
      {- [keep-ite]: whether to eliminate term if-then-else or keep them as terms - this requires the context to include the egraph}
      {- [break-symmetries]: attempt to detect symmetries and add constraints to remove them (this can be used only if the context is created for QF_UF)}
      {- [assert-ite-bounds]: try to determine upper and lower bound on if-then-else terms and assert these bounds. For example, if term [t] is defined as [(ite c 10 (ite d 3 20))], then the context with include the assertion [3 <= t <= 20].}
    }
  *)

  (** Enable option *)
  external enable_option : context -> string -> unit
    = "ocamlyices_context_enable_option"

  (** Disable option *)
  external disable_option : context -> string -> unit
    = "ocamlyices_context_disable_option"

  (** {2 Formula assertion} *)

  (** Assert a single formula *)
  external assert_formula : context -> term -> unit
    = "ocamlyices_context_assert_formula"

  (** Assert multiple formulae *)
  external assert_formulas : context -> term array -> unit
    = "ocamlyices_context_assert_formulas"


  (** {2 Assertion stack operations} *)

  (** Reset context, i.e. remove all assertions. *)
  external reset : context -> unit
    = "ocamlyices_context_reset"

  (** Push, i.e. mark a backtrack point. *)
  external push : context -> unit
    = "ocamlyices_context_push"

  (** Pop, i.e. remove all assertions up to last backtrack point. *)
  external pop : context -> unit
    = "ocamlyices_context_pop"


  (** {2 Satisfiability checking } *)
  (** Check parameters

   *)
  module Params : sig

    (** Create a record of parameters *)
    external create : unit -> params
      = "ocamlyices_params_create"

    (** Set a parameter *)
    external set : params -> string -> string -> unit
      = "ocamlyices_params_set"
  end

  (** Check satisfiability. *)
  external check : ?params:params -> context -> status
    = "ocamlyices_context_check"

  (** Add a blocking clause, i.e. a clause that ask for another model. *)
  external assert_blocking_clause : context -> unit
    = "ocamlyices_context_assert_blocking_clause"

  (** Interrupt a search. *)
  external stop_search : context -> unit
    = "ocamlyices_context_stop_search"

  (** Get model (should be called after {!check}). *)
  external get_model : ?keepsubst:bool -> context -> model
    = "ocamlyices_context_get_model"

end

(** Model *)
module Model : sig

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

  (** {2 Pretty printing} *)

  external print : ?width:int -> ?height:int -> ?offset:int
    -> (string -> unit) -> model -> unit
    = "ocamlyices_model_print"
end
