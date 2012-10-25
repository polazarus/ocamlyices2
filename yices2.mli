val version : string
val build_arch : string
val build_mode : string
val build_date : string

external init : unit -> unit = "ocamlyices_init"
external reset : unit -> unit = "ocamlyices_reset"
external exit : unit -> unit = "ocamlyices_exit"

type typ
external int_type : unit -> typ = "ocamlyices_int_type"
external bool_type : unit -> typ = "ocamlyices_bool_type"
external real_type : unit -> typ = "ocamlyices_real_type"
external new_bv_type : int -> typ = "ocamlyices_new_bv_type"
external new_scalar_type : int -> typ = "ocamlyices_new_scalar_type"
external new_uninterpreted_type : int -> typ = "ocamlyices_new_uninterpreted_type"
external tuple_type : typ array -> typ = "ocamlyices_tuple_type"


type term
external true_ : unit -> term = "ocamlyices_true"
external false_ : unit -> term = "ocamlyices_true"

