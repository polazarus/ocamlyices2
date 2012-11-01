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

type error_report = {
  code : int;
  line : int;
  column : int;
  term1 : term option;
  type1 : typ option;
  term2 : term option;
  type2 : typ option;
  badval : int64
}

exception YicesError of string * error_report


(** Initialize Yices *)
external init : unit -> unit = "ocamlyices_init"

(** Reset Yices *)
external reset : unit -> unit = "ocamlyices_reset"

(** Exit Yices *)
external exit : unit -> unit = "ocamlyices_exit"

(******************************************************************************)

external int_type : unit -> typ = "ocamlyices_int_type"
external bool_type : unit -> typ = "ocamlyices_bool_type"
external real_type : unit -> typ = "ocamlyices_real_type"

external bv_type : int -> typ = "ocamlyices_bv_type"
external new_scalar_type : int -> typ = "ocamlyices_new_scalar_type"

external new_uninterpreted_type : int -> typ = "ocamlyices_new_uninterpreted_type"
external tuple_type : typ array -> typ = "ocamlyices_tuple_type"
external function_type : typ array -> typ -> typ = "ocamlyices_function_type"

external parse_type : string -> term = "ocamlyices_parse_type"


(******************************************************************************)

external true_ : unit -> term = "ocamlyices_true"
external false_ : unit -> term = "ocamlyices_false"
external constant : typ -> int -> term = "ocamlyices_constant"
external new_uninterpreted_term : typ -> term = "ocamlyices_new_uninterpreted_term"
external new_variable : typ -> term = "ocamlyices_new_variable"
external application : term -> term array -> term = "ocamlyices_application"

external eq : term -> term -> term = "ocamlyices_eq"
external neq : term -> term -> term = "ocamlyices_neq"
external not : term -> term = "ocamlyices_not"

external and_ : term -> term -> term = "ocamlyices_and2"
external or_ : term -> term -> term = "ocamlyices_or2"
external xor : term -> term -> term = "ocamlyices_xor2"
external and3 : term -> term -> term -> term = "ocamlyices_and3"
external or3 : term -> term -> term -> term = "ocamlyices_or3"
external xor3 : term -> term -> term -> term = "ocamlyices_xor3"
external andn : term array -> term = "ocamlyices_and"
external orn : term array -> term = "ocamlyices_or"
external xorn : term array -> term = "ocamlyices_xor"

external iff : term -> term -> term = "ocamlyices_iff"
external implies : term -> term -> term = "ocamlyices_implies"
external tuple : term array -> term = "ocamlyices_tuple"
external select : int -> term -> term = "ocamlyices_select"
external tuple_update : term -> int -> term -> term = "ocamlyices_tuple_update"
external update : term -> term array -> term -> term = "ocamlyices_update"
external distinct : term array -> term = "ocamlyices_distinct"

external forall : term -> term -> term = "ocamlyices_forall"
external exists : term -> term -> term = "ocamlyices_exists"
external lambda : term -> term -> term = "ocamlyices_lambda"
external foralln : term array -> term -> term = "ocamlyices_foralln"
external existsn : term array -> term -> term = "ocamlyices_existsn"
external lambdan : term array-> term -> term = "ocamlyices_lambdan"

external zero : unit -> term = "ocamlyices_zero"
external int_ : int -> term = "ocamlyices_int"
external int32_ : int32 -> term = "ocamlyices_int32"
external int64_ : int64 -> term = "ocamlyices_int64"
external rational : int -> int -> term = "ocamlyices_rational"
external rational32 : int32 -> int32 -> term = "ocamlyices_rational32"
external rational64 : int64 -> int64 -> term = "ocamlyices_rational64"
external parse_rational : string -> term = "ocamlyices_parse_rational"
external parse_float : string -> term = "ocamlyices_parse_float"
external add : term -> term -> term = "ocamlyices_add"
external sub : term -> term -> term = "ocamlyices_sub"
external neg : term -> term = "ocamlyices_neg"
external mul : term -> term -> term = "ocamlyices_mul"
external square : term -> term = "ocamlyices_square"

external power : term -> int -> term = "ocamlyices_power"
external power32 : term -> int32 -> term = "ocamlyices_power32"

external poly_int : int array -> term array -> term = "ocamlyices_poly_int"
external poly_int32 : int32 array -> term array -> term = "ocamlyices_poly_int32"
external poly_int64 : int64 array -> term array -> term = "ocamlyices_poly_int64"

external poly_rational : int array -> int array -> term array -> term = "ocamlyices_poly_rational"
external poly_rational32 : int32 array -> int32 array -> term array -> term = "ocamlyices_poly_rational32"
external poly_rational64 : int64 array -> int64 array -> term array -> term = "ocamlyices_poly_rational64"

external arith_eq : term -> term -> term = "ocamlyices_arith_eq"
external arith_neq : term -> term -> term = "ocamlyices_arith_neq"
external arith_geq : term -> term -> term = "ocamlyices_arith_geq"
external arith_leq : term -> term -> term = "ocamlyices_arith_leq"
external arith_gt : term -> term -> term = "ocamlyices_arith_gt"
external arith_lt : term -> term -> term = "ocamlyices_arith_lt"

external arith_eq0 : term -> term = "ocamlyices_arith_eq0"
external arith_neq0 : term -> term = "ocamlyices_arith_neq0"
external arith_geq0 : term -> term = "ocamlyices_arith_geq0"
external arith_leq0 : term -> term = "ocamlyices_arith_leq0"
external arith_gt0 : term -> term = "ocamlyices_arith_gt0"
external arith_lt0 : term -> term = "ocamlyices_arith_lt0"

external bvconst_int : int -> int -> term = "ocamlyices_bvconst_int"
external bvconst_int32 : int -> int32 ->  term = "ocamlyices_bvconst_int32"
external bvconst_int64 : int -> int64 -> term = "ocamlyices_bvconst_int64"

external bvconst_zero : int -> term = "ocamlyices_bvconst_zero"
external bvconst_one : int -> term = "ocamlyices_bvconst_one"
external bvconst_minus_one : int -> term = "ocamlyices_bvconst_minus_one"

external bvconst_from_array : bool array -> term = "ocamlyices_bvconst_from_array"
external bvconst_from_bin : string -> term = "ocamlyices_parse_bvbin"
external bvconst_from_hex : string -> term = "ocamlyices_parse_bvhex"

external bvadd : term -> term -> term = "ocamlyices_bvadd"
external bvsub : term -> term -> term = "ocamlyices_bvsub"
external bvneg : term -> term = "ocamlyices_bvneg"
external bvmul : term -> term -> term = "ocamlyices_bvmul"
external bvsquare : term -> term = "ocamlyices_bvsquare"
external bvpower : term -> int -> term = "ocamlyices_bvpower"
external bvpower32 : term -> int32 -> term = "ocamlyices_bvpower32"

external bvdiv : term -> term -> term = "ocamlyices_bvdiv"
external bvrem : term -> term -> term = "ocamlyices_bvrem"
external bvsdiv : term -> term -> term = "ocamlyices_bvsdiv"
external bvsrem : term -> term -> term = "ocamlyices_bvsrem"
external bvsmod : term -> term -> term = "ocamlyices_bvsmod"

external bvnot : term -> term = "ocamlyices_bvnot"
external bvand : term -> term -> term = "ocamlyices_bvand"
external bvor : term -> term -> term = "ocamlyices_bvor"
external bvxor : term -> term -> term = "ocamlyices_bvxor"
external bvnand : term -> term -> term = "ocamlyices_bvnand"
external bvnor : term -> term -> term = "ocamlyices_bvnor"
external bvxnor : term -> term -> term = "ocamlyices_bvxnor"
external bvshl : term -> term -> term = "ocamlyices_bvshl"
external bvlshr : term -> term -> term = "ocamlyices_bvlshr"
external bvashr : term -> term -> term = "ocamlyices_bvashr"

external shift_left0 : term -> int -> term = "ocamlyices_shift_left0"
external shift_left1 : term -> int -> term = "ocamlyices_shift_left1"
external shift_right0 : term -> int -> term = "ocamlyices_shift_right0"
external shift_right1 : term -> int -> term = "ocamlyices_shift_right1"
external ashift_right : term -> int -> term = "ocamlyices_ashift_right"
external rotate_left : term -> int -> term = "ocamlyices_rotate_left"
external rotate_right : term -> int -> term = "ocamlyices_rotate_right"

external bvextract : term -> int -> int -> term = "ocamlyices_bvextract"

external bvconcat : term -> term -> term = "ocamlyices_bvconcat"

external bvrepeat : term -> int -> term = "ocamlyices_bvrepeat"
external sign_extend : term -> int -> term = "ocamlyices_sign_extend"
external zero_extend : term -> int -> term = "ocamlyices_zero_extend"

external redand : term -> term = "ocamlyices_redand"
external redor : term -> term = "ocamlyices_redor"
external redcomp : term -> term -> term = "ocamlyices_redcomp"

external bvarray : term array -> term = "ocamlyices_bvarray"
external bitextract : term -> int -> term = "ocamlyices_bitextract"

external bveq : term -> term -> term = "ocamlyices_bveq"
external bvneq : term -> term -> term = "ocamlyices_bvneq"
external bvge : term -> term -> term = "ocamlyices_bvge"
external bvgt : term -> term -> term = "ocamlyices_bvgt"
external bvle : term -> term -> term = "ocamlyices_bvle"
external bvlt : term -> term -> term = "ocamlyices_bvlt"
external bvsge : term -> term -> term = "ocamlyices_bvsge"
external bvsgt : term -> term -> term = "ocamlyices_bvsgt"
external bvsle : term -> term -> term = "ocamlyices_bvsle"
external bvslt : term -> term -> term = "ocamlyices_bvslt"

external parse_term : string -> term = "ocamlyices_parse_term"

(******************************************************************************)

external set_type_name : typ -> string -> unit = "ocamlyices_set_type_name"
external remove_type_name : string -> unit = "ocamlyices_remove_type_name"
external get_type_by_name : string -> typ = "ocamlyices_get_type_by_name"
external clear_type_name : string -> unit = "ocamlyices_clear_type_name"

external set_term_name : term -> string -> unit = "ocamlyices_set_term_name"
external remove_term_name : string -> unit = "ocamlyices_remove_term_name"
external get_term_by_name : string -> term = "ocamlyices_get_term_by_name"
external clear_term_name : string -> unit = "ocamlyices_clear_term_name"

(******************************************************************************)

external pp_term : ?width:int -> ?height:int -> ?offset:int
  -> (string -> unit) -> term -> unit = "ocamlyices_pp_term"
external pp_type : ?width:int -> ?height:int -> ?offset:int
  -> (string -> unit) -> typ -> unit = "ocamlyices_pp_type"
external pp_model : ?width:int -> ?height:int -> ?offset:int
  -> (string -> unit) -> model -> unit = "ocamlyices_pp_model"

(******************************************************************************)

(* Term info *)

external type_of_term : term -> typ = "ocamlyices_type_of_term"
external term_is_bool : term -> bool = "ocamlyices_term_is_bool"
external term_is_int : term -> bool = "ocamlyices_term_is_int"
external term_is_real : term -> bool = "ocamlyices_term_is_real"
external term_is_arithmetic : term -> bool = "ocamlyices_term_is_arithmetic"
external term_is_bitvector : term -> bool = "ocamlyices_term_is_bitvector"
external term_is_tuple : term -> bool = "ocamlyices_term_is_tuple"
external term_is_function : term -> bool = "ocamlyices_term_is_function"
external term_is_scalar : term -> bool = "ocamlyices_term_is_scalar"
external term_bitsize : term -> int = "ocamlyices_term_bitsize"

external new_config : unit -> config = "ocamlyices_new_config"
external free_config : config -> unit = "ocamlyices_free_config"
external set_config : config -> string -> string -> unit = "ocamlyices_set_config"
external default_config_for_logic : config -> string -> unit
  = "ocamlyices_default_config_for_logic"

external new_context : ?config:config -> unit -> context = "ocamlyices_new_context"
external free_context : context -> unit = "ocamlyices_free_context"

external context_status : context -> status = "ocamlyices_context_status"
external reset_context : context -> unit = "ocamlyices_reset_context"
external push : context -> unit = "ocamlyices_push"
external pop : context -> unit = "ocamlyices_pop"
external context_enable_option : context -> string -> unit
  = "ocamlyices_context_enable_option"
external context_disable_option : context -> string -> unit
  = "ocamlyices_context_disable_option"

external assert_formula : context -> term -> unit
  = "ocamlyices_assert_formula"
external assert_formulas : context -> term array -> unit
  = "ocamlyices_assert_formulas"

external check : ?params:params -> context -> status = "ocamlyices_check"

external assert_blocking_clause : context -> unit
  = "ocamlyices_assert_blocking_clause"
external stop_search : context -> unit = "ocamlyices_stop_search"

external new_params : unit -> params = "ocamlyices_new_param_record"
external set_param : params -> string -> string -> unit = "ocamlyices_set_param"
external free_params : params -> unit = "ocamlyices_free_param_record"

external get_model : ?keepsubst:bool -> context -> model = "ocamlyices_get_model"
external free_model : model -> unit = "ocamlyices_free_model"

external get_bool_value : model -> term -> bool = "ocamlyices_get_bool_value"
external get_int_value : model -> term -> int = "ocamlyices_get_int_value"
external get_int32_value : model -> term -> int32 = "ocamlyices_get_int32_value"
external get_int64_value : model -> term -> int64 = "ocamlyices_get_int64_value"
external get_int_value_as_string : model -> term -> string = "ocamlyices_get_int_value_as_string"
external get_rational_value_as_string : model -> term -> string
  = "ocamlyices_get_rational_value_as_string"

external get_int_value_as_string : model -> term -> string = "ocamlyices_get_int_value_as_string"
external get_rational_value_as_string : model -> term -> string
  = "ocamlyices_get_rational_value_as_string"
external get_real_value_as_float : model -> term -> float = "ocamlyices_get_double_value"
