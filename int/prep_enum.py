#!/usr/bin/env python

import re
import sys
import shutil

re_comments_and_directives = re.compile(r"""
// .*[\r\n]*
| /\*(?:[^*]|\*[^/])*\*/
| \# (?:[^\\\r\n]|\\(?:.|\r\n|\n|\r))* (?:\\\Z)? [\r\n]*
""", re.X)

re_error_code_enum = re.compile (r"""
\b enum \s+ error_code \s+ \{
(.*?)
\} \s+ error_code_t\b[^;]*;
""", re.X | re.DOTALL)

re_error_code = re.compile(r'\b[A-Z_][A-Z_0-9]*\b');

def extract_codes(filename):
    with open(filename) as f:
        contents = f.read()
    contents = re_comments_and_directives.sub("", contents)
    m = re_error_code_enum.search(contents)
    if m:
        codes = re_error_code.findall(m.group(1))
    else:
        raise Exception("cannot find error code enum")
    return codes

re_codes = re.compile(r"(\(\*<ERRORCODES>\*\)).*?([ \t]*\(\*</ERRORCODES>\*\))", re.DOTALL)
def gen_ml(codes, filenameml, filenamemli):

    with open(filenameml, "r") as ml, open(filenamemli, "r") as mli:
        contentsml = ml.read()
        contentsmli = mli.read()

    repl = "\\1\n"+"".join("  | %s\n" % code for code in codes)+"\\2"

    if not re_codes.search(contentsml):
        print ("cannot find <ERRORCODES>")
        sys.exit(1)

    contentsml = re_codes.sub(repl, contentsml)
    contentsmli = re_codes.sub(repl, contentsmli)

    shutil.move(filenameml, filenameml+".orig")
    shutil.move(filenamemli, filenamemli+".orig")

    with open(filenameml, "w") as ml, open(filenamemli, "w") as mli:
        ml.write(contentsml)
        mli.write(contentsmli)

stub_header = r"""/* automatically generated file */
static const char *_oy_linear_error_code_names[] = {
"""
stub_middle = r"""
};
static inline intnat _oy_linear_error_code (error_code_t err) {
  switch (err) {
"""
stub_footer = r"""
  }
}
"""

def gen_stub(codes, filename):
    with open(filename, "w") as f:
        f.write(stub_header)
        for code in codes:
            f.write("  \"%s\",\n" % code)
        f.write(stub_middle)
        for i, code in enumerate(codes):
            if code == "INTERNAL_EXCEPTION":
                f.write("  default:\n")
            f.write("  case %s:\n    return %d;\n" % (code,i))
        f.write(stub_footer)

import sys
if len(sys.argv) != 5:
    print ("usage : prep_enum.py yices_types.h yices2.ml yices2.mli errors.h")
else:
    codes = extract_codes(sys.argv[1]) # yices_types.h
    gen_ml(codes, sys.argv[2],sys.argv[3]) # yices2_error.mli
    gen_stub(codes, sys.argv[4]) # errors.h
