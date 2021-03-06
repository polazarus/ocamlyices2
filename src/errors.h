/* automatically generated file */
static const char *_oy_linear_error_code_names[] = {
  "NO_ERROR",
  "INVALID_TYPE",
  "INVALID_TERM",
  "INVALID_CONSTANT_INDEX",
  "INVALID_VAR_INDEX",
  "INVALID_TUPLE_INDEX",
  "INVALID_RATIONAL_FORMAT",
  "INVALID_FLOAT_FORMAT",
  "INVALID_BVBIN_FORMAT",
  "INVALID_BVHEX_FORMAT",
  "INVALID_BITSHIFT",
  "INVALID_BVEXTRACT",
  "INVALID_BITEXTRACT",
  "TOO_MANY_ARGUMENTS",
  "TOO_MANY_VARS",
  "MAX_BVSIZE_EXCEEDED",
  "DEGREE_OVERFLOW",
  "DIVISION_BY_ZERO",
  "POS_INT_REQUIRED",
  "NONNEG_INT_REQUIRED",
  "SCALAR_OR_UTYPE_REQUIRED",
  "FUNCTION_REQUIRED",
  "TUPLE_REQUIRED",
  "VARIABLE_REQUIRED",
  "ARITHTERM_REQUIRED",
  "BITVECTOR_REQUIRED",
  "SCALAR_TERM_REQUIRED",
  "WRONG_NUMBER_OF_ARGUMENTS",
  "TYPE_MISMATCH",
  "INCOMPATIBLE_TYPES",
  "DUPLICATE_VARIABLE",
  "INCOMPATIBLE_BVSIZES",
  "EMPTY_BITVECTOR",
  "ARITHCONSTANT_REQUIRED",
  "INVALID_MACRO",
  "TOO_MANY_MACRO_PARAMS",
  "TYPE_VAR_REQUIRED",
  "DUPLICATE_TYPE_VAR",
  "BVTYPE_REQUIRED",
  "BAD_TERM_DECREF",
  "BAD_TYPE_DECREF",
  "INVALID_TOKEN",
  "SYNTAX_ERROR",
  "UNDEFINED_TYPE_NAME",
  "UNDEFINED_TERM_NAME",
  "REDEFINED_TYPE_NAME",
  "REDEFINED_TERM_NAME",
  "DUPLICATE_NAME_IN_SCALAR",
  "DUPLICATE_VAR_NAME",
  "INTEGER_OVERFLOW",
  "INTEGER_REQUIRED",
  "RATIONAL_REQUIRED",
  "SYMBOL_REQUIRED",
  "TYPE_REQUIRED",
  "NON_CONSTANT_DIVISOR",
  "NEGATIVE_BVSIZE",
  "INVALID_BVCONSTANT",
  "TYPE_MISMATCH_IN_DEF",
  "ARITH_ERROR",
  "BVARITH_ERROR",
  "CTX_FREE_VAR_IN_FORMULA",
  "CTX_LOGIC_NOT_SUPPORTED",
  "CTX_UF_NOT_SUPPORTED",
  "CTX_ARITH_NOT_SUPPORTED",
  "CTX_BV_NOT_SUPPORTED",
  "CTX_ARRAYS_NOT_SUPPORTED",
  "CTX_QUANTIFIERS_NOT_SUPPORTED",
  "CTX_LAMBDAS_NOT_SUPPORTED",
  "CTX_NONLINEAR_ARITH_NOT_SUPPORTED",
  "CTX_FORMULA_NOT_IDL",
  "CTX_FORMULA_NOT_RDL",
  "CTX_TOO_MANY_ARITH_VARS",
  "CTX_TOO_MANY_ARITH_ATOMS",
  "CTX_TOO_MANY_BV_VARS",
  "CTX_TOO_MANY_BV_ATOMS",
  "CTX_ARITH_SOLVER_EXCEPTION",
  "CTX_BV_SOLVER_EXCEPTION",
  "CTX_ARRAY_SOLVER_EXCEPTION",
  "CTX_INVALID_OPERATION",
  "CTX_OPERATION_NOT_SUPPORTED",
  "CTX_INVALID_CONFIG",
  "CTX_UNKNOWN_PARAMETER",
  "CTX_INVALID_PARAMETER_VALUE",
  "CTX_UNKNOWN_LOGIC",
  "EVAL_UNKNOWN_TERM",
  "EVAL_FREEVAR_IN_TERM",
  "EVAL_QUANTIFIER",
  "EVAL_LAMBDA",
  "EVAL_OVERFLOW",
  "EVAL_FAILED",
  "EVAL_CONVERSION_FAILED",
  "EVAL_NO_IMPLICANT",
  "MDL_UNINT_REQUIRED",
  "MDL_CONSTANT_REQUIRED",
  "MDL_DUPLICATE_VAR",
  "MDL_FTYPE_NOT_ALLOWED",
  "MDL_CONSTRUCTION_FAILED",
  "YVAL_INVALID_OP",
  "YVAL_OVERFLOW",
  "OUTPUT_ERROR",
  "INTERNAL_EXCEPTION",
};
static inline intnat _oy_linear_error_code (error_code_t err) {
  switch (err) {
  case NO_ERROR:
    return 0;
  case INVALID_TYPE:
    return 1;
  case INVALID_TERM:
    return 2;
  case INVALID_CONSTANT_INDEX:
    return 3;
  case INVALID_VAR_INDEX:
    return 4;
  case INVALID_TUPLE_INDEX:
    return 5;
  case INVALID_RATIONAL_FORMAT:
    return 6;
  case INVALID_FLOAT_FORMAT:
    return 7;
  case INVALID_BVBIN_FORMAT:
    return 8;
  case INVALID_BVHEX_FORMAT:
    return 9;
  case INVALID_BITSHIFT:
    return 10;
  case INVALID_BVEXTRACT:
    return 11;
  case INVALID_BITEXTRACT:
    return 12;
  case TOO_MANY_ARGUMENTS:
    return 13;
  case TOO_MANY_VARS:
    return 14;
  case MAX_BVSIZE_EXCEEDED:
    return 15;
  case DEGREE_OVERFLOW:
    return 16;
  case DIVISION_BY_ZERO:
    return 17;
  case POS_INT_REQUIRED:
    return 18;
  case NONNEG_INT_REQUIRED:
    return 19;
  case SCALAR_OR_UTYPE_REQUIRED:
    return 20;
  case FUNCTION_REQUIRED:
    return 21;
  case TUPLE_REQUIRED:
    return 22;
  case VARIABLE_REQUIRED:
    return 23;
  case ARITHTERM_REQUIRED:
    return 24;
  case BITVECTOR_REQUIRED:
    return 25;
  case SCALAR_TERM_REQUIRED:
    return 26;
  case WRONG_NUMBER_OF_ARGUMENTS:
    return 27;
  case TYPE_MISMATCH:
    return 28;
  case INCOMPATIBLE_TYPES:
    return 29;
  case DUPLICATE_VARIABLE:
    return 30;
  case INCOMPATIBLE_BVSIZES:
    return 31;
  case EMPTY_BITVECTOR:
    return 32;
  case ARITHCONSTANT_REQUIRED:
    return 33;
  case INVALID_MACRO:
    return 34;
  case TOO_MANY_MACRO_PARAMS:
    return 35;
  case TYPE_VAR_REQUIRED:
    return 36;
  case DUPLICATE_TYPE_VAR:
    return 37;
  case BVTYPE_REQUIRED:
    return 38;
  case BAD_TERM_DECREF:
    return 39;
  case BAD_TYPE_DECREF:
    return 40;
  case INVALID_TOKEN:
    return 41;
  case SYNTAX_ERROR:
    return 42;
  case UNDEFINED_TYPE_NAME:
    return 43;
  case UNDEFINED_TERM_NAME:
    return 44;
  case REDEFINED_TYPE_NAME:
    return 45;
  case REDEFINED_TERM_NAME:
    return 46;
  case DUPLICATE_NAME_IN_SCALAR:
    return 47;
  case DUPLICATE_VAR_NAME:
    return 48;
  case INTEGER_OVERFLOW:
    return 49;
  case INTEGER_REQUIRED:
    return 50;
  case RATIONAL_REQUIRED:
    return 51;
  case SYMBOL_REQUIRED:
    return 52;
  case TYPE_REQUIRED:
    return 53;
  case NON_CONSTANT_DIVISOR:
    return 54;
  case NEGATIVE_BVSIZE:
    return 55;
  case INVALID_BVCONSTANT:
    return 56;
  case TYPE_MISMATCH_IN_DEF:
    return 57;
  case ARITH_ERROR:
    return 58;
  case BVARITH_ERROR:
    return 59;
  case CTX_FREE_VAR_IN_FORMULA:
    return 60;
  case CTX_LOGIC_NOT_SUPPORTED:
    return 61;
  case CTX_UF_NOT_SUPPORTED:
    return 62;
  case CTX_ARITH_NOT_SUPPORTED:
    return 63;
  case CTX_BV_NOT_SUPPORTED:
    return 64;
  case CTX_ARRAYS_NOT_SUPPORTED:
    return 65;
  case CTX_QUANTIFIERS_NOT_SUPPORTED:
    return 66;
  case CTX_LAMBDAS_NOT_SUPPORTED:
    return 67;
  case CTX_NONLINEAR_ARITH_NOT_SUPPORTED:
    return 68;
  case CTX_FORMULA_NOT_IDL:
    return 69;
  case CTX_FORMULA_NOT_RDL:
    return 70;
  case CTX_TOO_MANY_ARITH_VARS:
    return 71;
  case CTX_TOO_MANY_ARITH_ATOMS:
    return 72;
  case CTX_TOO_MANY_BV_VARS:
    return 73;
  case CTX_TOO_MANY_BV_ATOMS:
    return 74;
  case CTX_ARITH_SOLVER_EXCEPTION:
    return 75;
  case CTX_BV_SOLVER_EXCEPTION:
    return 76;
  case CTX_ARRAY_SOLVER_EXCEPTION:
    return 77;
  case CTX_INVALID_OPERATION:
    return 78;
  case CTX_OPERATION_NOT_SUPPORTED:
    return 79;
  case CTX_INVALID_CONFIG:
    return 80;
  case CTX_UNKNOWN_PARAMETER:
    return 81;
  case CTX_INVALID_PARAMETER_VALUE:
    return 82;
  case CTX_UNKNOWN_LOGIC:
    return 83;
  case EVAL_UNKNOWN_TERM:
    return 84;
  case EVAL_FREEVAR_IN_TERM:
    return 85;
  case EVAL_QUANTIFIER:
    return 86;
  case EVAL_LAMBDA:
    return 87;
  case EVAL_OVERFLOW:
    return 88;
  case EVAL_FAILED:
    return 89;
  case EVAL_CONVERSION_FAILED:
    return 90;
  case EVAL_NO_IMPLICANT:
    return 91;
  case MDL_UNINT_REQUIRED:
    return 92;
  case MDL_CONSTANT_REQUIRED:
    return 93;
  case MDL_DUPLICATE_VAR:
    return 94;
  case MDL_FTYPE_NOT_ALLOWED:
    return 95;
  case MDL_CONSTRUCTION_FAILED:
    return 96;
  case YVAL_INVALID_OP:
    return 97;
  case YVAL_OVERFLOW:
    return 98;
  case OUTPUT_ERROR:
    return 99;
  default:
  case INTERNAL_EXCEPTION:
    return 100;

  }
}
