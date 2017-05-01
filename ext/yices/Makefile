#########################################################################
#  The Yices SMT Solver. Copyright 2014 SRI International.              #
#                                                                       #
#  This program may only be used subject to the noncommercial end user  #
#  license agreement which is downloadable along with this program.     #
#########################################################################

SHELL=/bin/sh
builddir = build
.PHONY: default build bin lib install clean arch-clean all-clean show-details \
        source-distribution binary-distribution dist tarfile \
        doc documentation manual regress check \
		test tests compile-tests-dynamic compile-tests-static

#
# Default target: build binaries/libraries
# Parallel considerations: using 'make -j' can greatly decrease the build time.
#
build: bin lib
	@echo "Build done. Run 'make dist' to see the results in $(builddir)/dist."

# Be helpful if not configured
summary make.include:
	@echo Please ./configure first && exit 1
-include make.include

#
#   top_builddir = top-level directory for Yices
#   YICES_MODE = build mode
#   PACKAGE_VERSION = full version
#   MAJOR = major version number
#   MINOR = minor version number
#   PATCH_LEVEL = patch level
#   host = architecture (e.g, i686-pc-linux-gnu)
#   host_os = OS (e.g., linux)
#   builddir = target build director
#

objdir := $(builddir)/obj
srcdir := src

#
# Distribution subdirectory: tarfiles are constructed
# via make binary-distribution or make smt-distribution
#
# In this makefile, we just copy what needs to be included
# in the distribution tarfiles in $(builddir)/dist or $(builddir)/static_dist
# WARNING: we must give absolute paths because the 'install' program needs
# absolute destination paths.
#
distdir := $(abspath $(top_builddir))/$(builddir)/dist
smt_distdir := $(abspath $(top_builddir))/$(builddir)/smt_dist

#
# SOURCE FILES
#

#
# Source files for the shared library (libyices.so)
#
base_src_c := \
	src/api/yices_version.c \
	src/api/context_config.c \
	src/api/search_parameters.c \
	src/api/smt_logic_codes.c \
	src/api/yices_api.c \
	src/api/yices_error.c \
	src/api/yval.c \
	src/context/common_conjuncts.c \
	src/context/conditional_definitions.c \
	src/context/context.c \
	src/context/context_simplifier.c \
	src/context/context_solver.c \
	src/context/context_statistics.c \
	src/context/context_utils.c \
	src/context/divmod_table.c \
	src/context/eq_abstraction.c \
	src/context/eq_learner.c \
	src/context/internalization_table.c \
	src/context/ite_flattener.c \
	src/context/pseudo_subst.c \
	src/context/shared_terms.c \
	src/context/symmetry_breaking.c \
	src/exists_forall/ef_client.c \
	src/exists_forall/ef_analyze.c \
	src/exists_forall/ef_parameters.c \
	src/exists_forall/ef_problem.c \
	src/exists_forall/efsolver.c \
	src/frontend/smt2/attribute_values.c \
	src/frontend/yices/yices_lexer.c \
	src/frontend/yices/yices_parser.c \
	src/io/concrete_value_printer.c \
	src/io/model_printer.c \
	src/io/pretty_printer.c \
	src/io/reader.c \
	src/io/term_printer.c \
	src/io/tracer.c \
	src/io/type_printer.c \
	src/io/yices_pp.c \
	src/io/writer.c \
	src/model/abstract_values.c \
	src/model/arith_projection.c \
	src/model/concrete_values.c \
	src/model/fresh_value_maker.c \
	src/model/fun_maps.c \
	src/model/fun_trees.c \
	src/model/generalization.c \
	src/model/literal_collector.c \
	src/model/map_to_model.c \
	src/model/model_eval.c \
	src/model/model_queries.c \
	src/model/models.c \
	src/model/presburger.c \
	src/model/projection.c \
	src/model/term_to_val.c \
	src/model/val_to_term.c \
	src/parser_utils/lexer.c \
	src/parser_utils/parser.c \
	src/parser_utils/term_stack2.c \
	src/parser_utils/term_stack_error.c \
	src/solvers/bv/bit_blaster.c \
	src/solvers/bv/bv64_intervals.c \
	src/solvers/bv/bv_atomtable.c \
	src/solvers/bv/bvconst_hmap.c \
	src/solvers/bv/bvexp_table.c \
	src/solvers/bv/bv_intervals.c \
	src/solvers/bv/bvpoly_compiler.c \
	src/solvers/bv/bvpoly_dag.c \
	src/solvers/bv/bvsolver.c \
	src/solvers/bv/bv_vartable.c \
	src/solvers/bv/merge_table.c \
	src/solvers/bv/remap_table.c \
	src/solvers/cdcl/gates_hash_table.c \
	src/solvers/cdcl/gates_manager.c \
	src/solvers/cdcl/smt_core.c \
	src/solvers/egraph/composites.c \
	src/solvers/egraph/diseq_stacks.c \
	src/solvers/egraph/egraph_assertion_queues.c \
	src/solvers/egraph/egraph.c \
	src/solvers/egraph/egraph_explanations.c \
	src/solvers/egraph/egraph_utils.c \
	src/solvers/egraph/theory_explanations.c \
	src/solvers/floyd_warshall/dl_vartable.c \
	src/solvers/floyd_warshall/idl_floyd_warshall.c \
	src/solvers/floyd_warshall/rdl_floyd_warshall.c \
	src/solvers/funs/fun_solver.c \
	src/solvers/simplex/arith_atomtable.c \
	src/solvers/simplex/arith_vartable.c \
	src/solvers/simplex/diophantine_systems.c \
	src/solvers/simplex/integrality_constraints.c \
	src/solvers/simplex/matrices.c \
	src/solvers/simplex/offset_equalities.c \
	src/solvers/simplex/simplex.c \
	src/terms/balanced_arith_buffers.c \
	src/terms/bit_expr.c \
	src/terms/bit_term_conversion.c \
	src/terms/bv64_interval_abstraction.c \
	src/terms/bv64_constants.c \
	src/terms/bv64_polynomials.c \
	src/terms/bvarith64_buffers.c \
	src/terms/bvarith64_buffer_terms.c \
	src/terms/bvarith_buffers.c \
	src/terms/bvarith_buffer_terms.c \
	src/terms/bv_constants.c \
	src/terms/bvlogic_buffers.c \
	src/terms/bvpoly_buffers.c \
	src/terms/bv_polynomials.c \
	src/terms/bv_slices.c \
	src/terms/conditionals.c \
	src/terms/elim_subst.c \
	src/terms/extended_rationals.c \
	src/terms/free_var_collector.c \
	src/terms/full_subst.c \
	src/terms/ite_stack.c \
	src/terms/mpq_aux.c \
	src/terms/poly_buffer.c \
	src/terms/poly_buffer_terms.c \
	src/terms/polynomials.c \
	src/terms/power_products.c \
	src/terms/pprod_table.c \
	src/terms/rational_hash_maps.c \
	src/terms/rationals.c \
	src/terms/rba_buffer_terms.c \
	src/terms/renaming_context.c \
	src/terms/subst_cache.c \
	src/terms/subst_context.c \
	src/terms/term_explorer.c \
	src/terms/term_manager.c \
	src/terms/terms.c \
	src/terms/term_sets.c \
	src/terms/term_substitution.c \
	src/terms/term_utils.c \
	src/terms/types.c \
	src/terms/variable_renaming.c \
	src/utils/arena.c \
	src/utils/backtrack_arrays.c \
	src/utils/cache.c \
	src/utils/csets.c \
	src/utils/dep_tables.c \
	src/utils/gcd.c \
	src/utils/generic_heap.c \
	src/utils/hash_functions.c \
	src/utils/index_vectors.c \
	src/utils/int_array_hsets.c \
	src/utils/int_array_sort2.c \
	src/utils/int_array_sort.c \
	src/utils/int_bags.c \
	src/utils/int_bv_sets.c \
	src/utils/int_hash_classes.c \
	src/utils/int_hash_map2.c \
	src/utils/int_hash_map.c \
	src/utils/int_hash_sets.c \
	src/utils/int_hash_tables.c \
	src/utils/int_heap2.c \
	src/utils/int_heap.c \
	src/utils/int_partitions.c \
	src/utils/int_powers.c \
	src/utils/int_queues.c \
	src/utils/int_stack.c \
	src/utils/int_vectors.c \
	src/utils/mark_vectors.c \
	src/utils/memalloc.c \
	src/utils/object_stores.c \
	src/utils/pair_hash_map2.c \
	src/utils/pointer_vectors.c \
	src/utils/ptr_array_sort2.c \
	src/utils/ptr_array_sort.c \
	src/utils/ptr_hash_classes.c \
	src/utils/ptr_hash_map.c \
	src/utils/ptr_heap.c \
	src/utils/ptr_partitions.c \
	src/utils/ptr_queues.c \
	src/utils/ptr_sets.c \
	src/utils/ptr_sets2.c \
	src/utils/ptr_stack.c \
	src/utils/ptr_vectors.c \
	src/utils/refcount_int_arrays.c \
	src/utils/refcount_strings.c \
	src/utils/simple_cache.c \
	src/utils/sparse_arrays.c \
	src/utils/stable_sort.c \
	src/utils/string_buffers.c \
	src/utils/string_utils.c \
	src/utils/symbol_tables.c \
	src/utils/tuple_hash_map.c \
	src/utils/uint_rbtrees.c \
	src/utils/use_vectors.c


#
# Optional: mcsat solver
#
mcsat_src_c := \
	src/mcsat/tracing.c \
	src/mcsat/solver.c \
	src/mcsat/variable_db.c \
	src/mcsat/variable_queue.c \
	src/mcsat/value.c \
	src/mcsat/model.c \
	src/mcsat/trail.c \
	src/mcsat/conflict.c \
	src/mcsat/gc.c \
	src/mcsat/utils/int_mset.c \
	src/mcsat/utils/int_lset.c \
	src/mcsat/utils/scope_holder.c \
	src/mcsat/utils/statistics.c \
	src/mcsat/uf/uf_plugin.c \
	src/mcsat/uf/app_reps.c \
	src/mcsat/uf/uf_feasible_set_db.c \
	src/mcsat/bool/clause_db.c \
	src/mcsat/bool/cnf.c \
	src/mcsat/bool/bcp_watch_manager.c \
	src/mcsat/bool/bool_plugin.c \
	src/mcsat/nra/nra_plugin.c \
	src/mcsat/nra/nra_plugin_internal.c \
	src/mcsat/nra/nra_plugin_explain.c \
	src/mcsat/nra/libpoly_utils.c \
	src/mcsat/nra/poly_constraint.c \
	src/mcsat/nra/feasible_set_db.c \
	src/mcsat/ite/ite_plugin.c \
	src/mcsat/watch_list_manager.c \
	src/mcsat/preprocessor.c \
	src/mcsat/options.c

#
# Fake mcsat module: do nothing. Used to make the compilation go through.
#
no_mcsat_src_c := \
	src/mcsat/no_mcsat.c \
	src/mcsat/options.c

#
# base: core + mcsat if MCSAT is enabled or core + fake mcsat
#
ifeq ($(ENABLE_MCSAT),yes)
base_src_c += $(mcsat_src_c)
else
base_src_c += $(no_mcsat_src_c)
endif

#
# Other source files for libyices_extra.a
# That's all the code needed by the main binaries
# + experimental/unfinished modules
# + functions for printing/debugging
# + old stuff not needed anymore (except for tests)
extra_src_c := \
	src/context/context_printer.c \
	src/context/dump_context.c \
	src/context/internalization_printer.c \
	src/frontend/common.c \
	src/frontend/smt1/smt_lexer.c \
	src/frontend/smt1/smt_parser.c \
	src/frontend/smt1/smt_term_stack.c \
	src/frontend/smt2/parenthesized_expr.c \
	src/frontend/smt2/smt2_commands.c \
	src/frontend/smt2/smt2_expressions.c \
	src/frontend/smt2/smt2_lexer.c \
	src/frontend/smt2/smt2_model_printer.c \
	src/frontend/smt2/smt2_parser.c \
	src/frontend/smt2/smt2_printer.c \
	src/frontend/smt2/smt2_term_stack.c \
	src/frontend/yices/arith_solver_codes.c \
	src/frontend/yices/yices_help.c \
	src/frontend/yices/yices_reval.c \
	src/model/large_bvsets.c \
	src/model/rb_bvsets.c \
	src/model/small_bvsets.c \
	src/scratch/booleq_table.c \
	src/scratch/bool_vartable.c \
	src/scratch/update_graph.c \
	src/solvers/bv/bvsolver_printer.c \
	src/solvers/bv/dimacs_printer.c \
	src/solvers/cdcl/gates_printer.c \
	src/solvers/cdcl/sat_solver.c \
	src/solvers/cdcl/smt_core_printer.c \
	src/solvers/egraph/egraph_printer.c \
	src/solvers/floyd_warshall/idl_fw_printer.c \
	src/solvers/floyd_warshall/rdl_fw_printer.c \
	src/solvers/funs/fun_solver_printer.c \
	src/solvers/simplex/dsolver_printer.c \
	src/solvers/simplex/int_constraint_printer.c \
	src/solvers/simplex/simplex_printer.c \
	src/solvers/simplex/simplex_prop_table.c \
	src/terms/arith_buffers.c \
	src/utils/command_line.c \
	src/utils/cputime.c \
	src/utils/memsize.c \
	src/utils/pair_hash_map.c \
	src/utils/pair_hash_sets.c \
	src/utils/string_hash_map.c \
	src/utils/tag_map.c \
	src/utils/timeout.c \
	src/utils/union_find.c

#
# additional source files for the binaries
#
bin_src_c := \
	src/frontend/yices_main.c \
	src/frontend/yices_sat.c \
	src/frontend/yices_smt.c \
	src/frontend/yices_smt2.c \
	src/frontend/yices_smtcomp.c \

#
# All auto-generated files (this list if just for cleaning purpose)
#
# The hash tables are generated 'globally' when building .lo files. This is
# not the best option but it avoids having to build all .d (dependencies) just
# to be sure that the hash.h will be generated.
#
# The version file should be updated whenever we run ./configure.
src/api/yices_version.c: config.status
	$(MKVERSION) $(template) src/api/yices_version.c $(PACKAGE_VERSION) $(YICES_MODE) $(host)
gperf_generated := \
	src/frontend/yices/yices_hash_keywords.h \
	src/frontend/smt1/smt_hash_keywords.h \
	src/frontend/smt2/smt2_hash_tokens.h \
	src/frontend/smt2/smt2_hash_keywords.h \
	src/frontend/smt2/smt2_hash_symbols.h

#
# Dependencies and object files
#
base_obj := $(base_src_c:%.c=$(objdir)/%.lo)
base_dep := $(base_src_c:%.c=$(objdir)/%.d)

extra_obj := $(extra_src_c:%.c=$(objdir)/%.lo)
extra_dep := $(extra_src_c:%.c=$(objdir)/%.d)

bin_obj := $(bin_src_c:%.c=$(objdir)/%.lo)
bin_dep := $(bin_src_c:%.c=$(objdir)/%.d)
binaries := $(addprefix $(builddir)/bin/,$(notdir $(bin_src_c:%.c=%$(EXEEXT))))

tests_src_dir := tests/unit
test_bin_dir := $(builddir)/testbin
tests_src_c := $(wildcard $(tests_src_dir)/*.c)
tests_obj := $(tests_src_c:%.c=$(objdir)/%.lo)
tests_dep := $(tests_src_c:%.c=$(objdir)/%.d)
tests_binaries := $(tests_src_c:$(tests_src_dir)/%.c=$(test_bin_dir)/%$(EXEEXT))
#tests_binaries_dynamic := $(tests_src_c:$(tests_src_dir)/%.c=$(test_bin_dir)/dynamic/%$(EXEEXT))

#
# Gperf generated tables
# - we need to give different names to the yices and smt lookup functions
#

ifneq ($(GPERF),)
src/frontend/yices/yices_hash_keywords.h: src/frontend/yices/yices_keywords.txt
	$(GPERF) -C -L ANSI-C -W yices_kw --output-file=$@ --lookup-function-name=in_yices_kw $^

src/frontend/smt1/smt_hash_keywords.h: src/frontend/smt1/smt_keywords.txt
	$(GPERF) -C -L ANSI-C -W smt_kw --output-file=$@ --lookup-function-name=in_smt_kw $^

#
# Tables for SMT2:
# - we want to include three hash functions in the same file (smt2_lexer.c)
# - for this to work,
#   we need to give different names to the hash function (option -H ...)
#   we can't use -G and we must give -E
#
src/frontend/smt2/smt2_hash_tokens.h: src/frontend/smt2/smt2_tokens.txt
	$(GPERF) -C -L ANSI-C -W smt2_tk -H hash_tk -E --output-file=$@ --lookup-function-name=in_smt2_tk $^

src/frontend/smt2/smt2_hash_keywords.h: src/frontend/smt2/smt2_keywords.txt
	$(GPERF) -C -L ANSI-C -W smt2_kw -H hash_kw -E --output-file=$@ --lookup-function-name=in_smt2_kw $^

src/frontend/smt2/smt2_hash_symbols.h: src/frontend/smt2/smt2_symbols.txt
	$(GPERF) -C -L ANSI-C -W smt2_sym -H hash_sym -E --output-file=$@ --lookup-function-name=in_smt2_sym $^
endif

#
# Object files.
#
$(objdir)/%.lo: %.c $(gperf_generated)
	@$(MKDIR_P) $(dir $@)
	$(COMPILE.c)
	@$(POSTCOMPILE)
# COMPILE.c: remeber that -fPIC, -DPIC and any shared/static platform-dependent
# library detail is automatically passed by libtool.
# The -Isrc is needed because the root directory for the .h files is src/.
# The -Isrc/include is needed because of the library 'include' headers
# (yices.h, ...) that are included without the include/ relative path.
# The LT_STATIC_CFLAGS and LT_SHARED_CFLAGS are a dirty hack in ltmain.sh.
# It simply gives the adds the content of these variables to the commands that
# build (respectively) the static and shared object. This is useful for passing
# the -D__GMP_LIBGMP_DLL=0 in static objects and -D__GMP_LIBGMP_DLL=1 in shared
# objects.
COMPILE.c = LT_STATIC_CFLAGS="$(STATIC_CPPFLAGS)" LT_SHARED_CFLAGS="$(SHARED_CPPFLAGS)" \
	$(LIBTOOL) $(LTFLAGS) --mode=compile $(CC) -I$(srcdir) -I$(srcdir)/include \
	$(DEPFLAGS) -c $< -o $@ $(CPPFLAGS) $(CFLAGS)
# The dependencies *.d are produced during compilation and are included at the
# end of the makefile if they have already been produced. Their only
# purpose is to re-build the .lo whenever one of the dependent .c or .h is
# modified; dependencies are only useful for yices developers, not for end-users
# who want to simply build the project. Because they are only needed
# for knowing which files should be re-compiled, they are not necessary at the
# first compilation, when no .d exists.
#
# The gcc will produce a build/obj/example.d where it has:
# 		src/utils/cputime.o: src/utils/cputime.c utils/cputime.h
# We want:
# 		build/obj/src/utils/cputime.lo build/obj/src/utils/cputime.d:
#           src/utils/cputime.c src/utils/cputime.h
# Three things we need to change using sed:
# - we must add src/utils/cputime.d to the target list (before ':'), so that
#   whenever a .c or .h dependency is changed, the .d will be recompiled.
# - the src/%.o must be build/obj/src/%.lo
# - the %.h must be src/%.h. In this case, I had to fix two bugs:
#   * do not replace %.h to src/% if %.h already begins with src;
#   * do not replace %.h to src/% if %.h is an absolute path.
# When compiling in release mode, we consider that the end-user is not going
# to change the code and re-compile. Then dependendy tasks are useless.
ifneq ($(YICES_MODE),release)
DEPFLAGS = -MMD -MP -MT $@ -MF $(@:%.lo=%.d)
POSTCOMPILE = \
	$(SED) 's,$@[ :]*,$@ $(@:%.lo=%.d): ,g' $(@:%.lo=%.d) | \
	$(SED) 's,\([^ \t\n:]*\.h\),$(srcdir)/\1,g' | \
	$(SED) 's,src/src,src,g' | \
	$(SED) 's,src//,/,g' > $(@:%.lo=%.d).$$$$ \
	&& mv $(@:%.lo=%.d).$$$$ $(@:%.lo=%.d)
endif

#
# Libraries.
#
# - libyices.a is the static library
# - libyices.la is the shared library name in the libtool vocabulary. When
#   installing, e.g.:
#       libtool --mode=install ./install-sh libyices.la /usr/lib/libyices.la',
#   libtool will take care of giving the right name (.dylib or .so or .dll...)
#   with the right versionning and everything. It will also handle stripping
#   during installation through the use of '-s' (see the STRIP variable).

ifeq ($(enable_static),yes)
lib: $(builddir)/lib/$(libyices_a)
endif
ifeq ($(enable_shared),yes)
lib: $(builddir)/lib/libyices.la
endif
# Table of possibilities:
# 1) static library
#   a) if libgmp.a has been found, create a libyices.a with gmp statically
#      linked. This uses partial linking offered by 'ld -r'.
#      Note: I think PIC should be disabled on libgmp.a for embeding it.
#   b) if libgmp.a has not been found, default to -lgmp. Most of the time,
#      this means that the gmp library will be dynamically linked.
# 2) shared library
#   a) if libgmp.a has been found and contains PIC (position-independant code),
#      create a shared library that embeds the gmp library statically.
#   b) if libgmp.a has not been found, default to using -lgmp which means that
#      libyices.so (or .dylib or .dll) will also depend on the GMP shared library.
#
# Static library.
#
# Precondition: STATIC_GMP and -lgmp most not be set simultanously. This
# precondition is enforced in configure.ac.
$(builddir)/lib/$(libyices_a): $(base_obj)
	$(MKDIR_P) $(dir $@)
	$(LD) -r -o $(builddir)/obj/libyices.o $(^:%.lo=%.o) $(LDFLAGS) $(STATIC_GMP) $(STATIC_LIBS) $(LIBS)
	$(AR) cru $@ $(builddir)/obj/libyices.o
# NOTE: We use the variable libyices_a because on Windows, static and shared
# both produce libyices.a. In this case, set libyices_a to something different
# than libyices.a from the shared version.
# NOTE: we cannot use libtool --mode=link instead of ld -r + ar cru because:
# 1) for partially linking with 'ld -r', libtool --mode=link discards the arg
#    'libgmp.a' for some reason, even with the -Wl,libgmp.a trick.
# 2) for archiving with 'ar cru', libtool --mode=link discards the *.o created
#    in the previous stage. Some answers on these linker flags stripped:
# https://www.gnu.org/software/libtool/manual/html_node/Stripped-link-flags.html
#
# Shared library.
#
# For GMP, we want to have either -lgmp or libgmp.a but not both.
# NOTE: you can set your own RPATH when launching make.
# Precondition: PIC_GMP and -lgmp most not be set simultanously
RPATH ?= $(libdir)
$(builddir)/lib/libyices.la: $(base_obj)
	$(MKDIR_P) $(dir $@)
	$(LIBTOOL) $(LTFLAGS) --mode=link $(CC) -shared -o $@ $^ -rpath $(RPATH) \
	-version-number $(MAJOR):$(MINOR):$(PATCH_LEVEL) -no-undefined \
	$(if $(PIC_GMP),-Wl$(,)$(PIC_GMP)) $(SHARED_LIBS) $(LIBS)
# NOTE: argument -Wl,$(STATIC_GMP) must be passed instead of simply $(STATIC_GMP)
# because without, libtool would discard any .a library with a message that says
# that it is not a libtool library. Here is the stackoverflow question that helped me:
# http://stackoverflow.com/questions/12714869/
# NOTE: The libtool argument '-no-undefined' (which translates to --no-undefined when
# ld is called) is necessary for shared library build on Mingw (and maybe others).

# Trick that allows to 'escape' the , character in $(if ...) using $(,)
, := ,

#
# Executables.
#
# Executables and tests cannot link to our newly created static or shared
# libraries because they are using non-exported symbols. To be able to
# compile them, we need to create a specific static library that does not
# enforce visibility ('ld' is the one who does that).
# A workaround is to pass the CFLAG -fvisibility=default instead of hidden when
# building *.o.
$(builddir)/libtemp/libyices_with_extras_without_gmp.la: $(base_obj) $(extra_obj)
	$(MKDIR_P) $(dir $@)
	$(LIBTOOL) $(LTFLAGS) --mode=link $(CC) -static $^ -o $@

bin: $(binaries)
# WARNING: because these programs depend on functions are do not belong to
# the yices library (= functions 'extern' in yices.h but not in yices_api.c),
# we must re-build a new library without the visibility=hidden limitation.
# For yices_sat: we don't need gmp.
$(builddir)/bin/yices_sat$(EXEEXT): $(objdir)/src/frontend/yices_sat.lo $(builddir)/libtemp/libyices_with_extras_without_gmp.la
	$(LIBTOOL) $(LTFLAGS) --mode=link $(CC) -static -o $@ $^ $(LDFLAGS) $(STATIC_LIBS) $(LIBS)

# For all other executables, use either -lgmp (in LIBS) or libgmp.a (in STATIC_GMP).
$(builddir)/bin/%$(EXEEXT): $(objdir)/src/frontend/%.lo $(builddir)/libtemp/libyices_with_extras_without_gmp.la
	$(LIBTOOL) $(LTFLAGS) --mode=link $(CC) -static -o $@ $^ $(LDFLAGS) $(STATIC_GMP) $(STATIC_LIBS) $(LIBS)

# Tests linked against static non-PIC library.
# Although we already linked libyices.a to libgmp.a, we still need to add
# 'STATIC_GMP' because libyices_extra has not been linked to libgmp.a.
# Note that .a archive does not enforce the hidden visibility ('t'=hidden and
# 'T'=visible that you can see when inspecting libyices.a with nm), so many
# function that should not be usable are actually used by the tests.
compile-tests compile-tests-static: $(tests_binaries)
#compile-tests-dynamic: $(tests_dep) $(tests_binaries_dynamic)
COMPILE_TEST = \
	$(LIBTOOL) $(LTFLAGS) --mode=link $(CC) -static -o $@ $^ $(LDFLAGS) \
	$(STATIC_LIBS) $(STATIC_GMP) $(LIBS) -no-fast-install
$(test_bin_dir)/%$(EXEEXT): $(objdir)/$(tests_src_dir)/%.lo $(builddir)/libtemp/libyices_with_extras_without_gmp.la
	$(COMPILE_TEST)

test: $(tests_binaries)
	@$(RUNTESTS)

test_%: $(test_bin_dir)/test_%
	@$(RUNTESTS)

RUNTESTS = ok=0; nok=0; \
	for p in $^; do \
		printf "%50s " $$p; \
		if ./$$p>/dev/null; \
		then echo OK; ok=$$(expr $$ok + 1); \
		else echo NOK; nok=$$(expr $$nok + 1); \
		fi; \
	done; echo "Passed: $$ok\nFailed: $$nok"; \
	test $$nok -eq 0

# Tests linked against shared library.
# Not possible to use libyices.so because some symbols that are used in tests
# are not exported (example: delete_pstore) because they are not in yices_api.c.
#$(test_bin_dir)/dynamic/%$(EXEEXT): $(builddir)/libtemp/libyices_extra.la $(objdir)/$(tests_src_dir)/%.lo ./$(builddir)/lib/libyices.la
#	$(LIBTOOL) $(LTFLAGS) --mode=link $(CC) -shared -o $@ $^ -no-fast-install
#$(LDFLAGS) $(LIBS)

##########################
#  BINARY DISTRIBUTIONS  #
##########################

ifeq ($(MAKE_RESTARTS),)
#
# Rules to keep going if .h or .c files have been deleted.  This
# allows Make to rebuild an out-of-date dependency file 'source.d'
# that refers to an 'oldstuff.h' that has been deleted.  We want to
# fail on the next restart if 'oldstuff.h' is still referenced in some
# existing 'source.c'.
#
%.h:
	@ echo "*** Warning: header file $@ is missing ***"

%.c:
	@ echo "*** Warning: source file $@ is missing ***"

%:
	@ echo "*** Warning: file $@ is missing ***"
endif


#
# ldconfig is used by the install rules.
# We don't check for ldconfig in configure.ac (since that doesn't
# make much sense). We use /sbin/ldconfig as default here. If that's
# wrong, change it on the command line:
#
#   make install LDCONFIG=...
#
LDCONFIG=/sbin/ldconfig

#
# Script/command to build a README file from a template
#
arm_android=$(findstring arm-linux-android,$(host))
ifeq ($(arm_android),arm-linux-android)
MKREADME=./utils/mkreadme-android
else
MKREADME=./utils/mkreadme
endif

#
# Template + script to construct yices_version.c
#
template=./src/api/yices_version_template.txt
MKVERSION=./utils/make_source_version

#
# Binary distributions:
#
# build_dir/dist: binaries + libraries with distribution not linked with GMP
# build_dir/static_dist: includes GMP (statically linked)
#
dist_dir := $(realpath $(top_builddir))/$(builddir)/dist

#
# Directories and temporary file to build tarfiles
# - we do this in two steps: the includes, binaries, and library files
#   to be distributed are constructed by invoking make -C src
#   and copied into $(dist_dir)
#   (what's required is defined in src/Makefile and may depend
#    on the OS + compilation mode).
# - the full tar file is built from $(dist_dir) + the generic files
#   that are independent of OS and compilation modes
# - the name of the tar file depends on ARCH + MODE
#
# All tarfiles are stored in ./distributions
#
distributions = ./distributions
bindist_dir = yices-$(PACKAGE_VERSION)

ifneq ($(YICES_MODE),release)
  bindist_tarfile = $(bindist_dir)-$(host)-$(MODE).tar.gz
else
  bindist_tarfile = $(bindist_dir)-$(host).tar.gz
endif


#
# Regressions
#
regressdir := tests/regress
check regress:
	$(regressdir)/check.sh $(regressdir) $(builddir)/bin

#
# Documentation: for now we just build manual.pdf
# this assumes latexmk is available
#
# We must have doc as an explicit target since we have a ./doc directory.
# Without it 'make doc' does nothing. To be safe, I've also added bin,
# lib, obj, etc. so that the Makefile will work event if directories
# or files with these names are present.
#
#
doc:
	@ echo "=== Building the documentation ==="
	@ (cd doc/manual ; \
          (latexmk -pdf manual || \
            (echo "===================================="; \
             echo "  WARNING: can't run latexmk"; \
             echo "   manual.pdf may be out-of-date"; \
             echo "===================================="; )))


documentation manual: doc

#
# Binary distribution:
#
# On mingw, there's a manual step involved. So build the
# distribution as follows:
# 1) make dist (or make static-dist)
# 2) from a Visual Studio console,
#    go to ./build/i686-pc-mingw32-release/dist/lib
#    then type 'lib /machine:x86 /def:libyices.def
# 3) clean up: remove libyices.def and libyices.exp
#    from ./build/i686-pc-mingw32-release/dist/lib
# 4) make tarfile (or make static-tarfile)
#
# On other platforms: make binary-distribution or make
# static-distribution should work.
#
# On MacOS X, the tar command adds files of the form ._xxxx to store
# metadata/extended file attributes. This gets ugly if we untar on
# some other platform. To prevent generation of these ._xxxx files, it
# is enough to set the environment variable COPYFILE_DISABLE to 1. We
# do this systematically (even on other systems). Hopefully,
# COPYFILE_DISABLE should not matter on other systems than MacOS X.
#
# We use a shell script ./util/mkreadme to make sure
# the README file in the tarfile is up to date (probably
# not foolproof)
#

binary-distribution: dist tarfile

tarfile: $(distributions) doc
	@ echo "=== Building $(bindist_tarfile) ==="
	rm -f -r $(bindist_dir)
	$(MKDIR_P) $(bindist_dir)
	cp -r $(dist_dir)/* $(bindist_dir)
	cp etc/LICENSE etc/NOTICES $(bindist_dir)
	$(MKREADME) $(dist_dir) etc/README.$(host_os) > $(bindist_dir)/README
	if test -f etc/install-yices.$(host_os) ; then \
	  cp etc/install-yices.$(host_os) $(bindist_dir)/install-yices ; \
	  chmod ug+x $(bindist_dir)/install-yices ; \
	fi
	$(MKDIR_P) $(bindist_dir)/etc
	cp etc/pstdint.h $(bindist_dir)/etc
	$(MKDIR_P) $(bindist_dir)/examples
	for file in `ls examples | $(EGREP) '\.ys$$|\.smt$$|\.c$$|\.smt2$$' ` ; do \
	  cp examples/$$file $(bindist_dir)/examples ; \
	done || true
	$(MKDIR_P) $(bindist_dir)/doc
	cp doc/YICES-LANGUAGE $(bindist_dir)/doc
	cp doc/manual/manual.pdf $(bindist_dir)/doc
	chmod -R og+rX $(bindist_dir)
	COPYFILE_DISABLE=1 tar -czf $(distributions)/$(bindist_tarfile) $(bindist_dir)
	chmod -R og+rX $(distributions)
	rm -f -r $(bindist_dir)


#
# Install from the source.
#
# The target directories are determined by ./configure:
#   bindir = where to put binaries
#   libdir = where to put the libraries
#   includedir = where to put the include files
#
# We assume 'make binary-distribution' was done first,
# then we copy the $(dist_dir) content:
#   $(dist_dir)/bin     into $(bindir)
#   $(dist_dir)/lib     into $(libdir)
#   $(dist_dir)/include into $(includedir)
#
# The reason we don't want to force compilation in 'make install' is
# that we typically need to do 'sudo make install' but we don't want
# to compile with 'sudo make binary-distribution'.  That can easily
# fail if the source and build directory or file permissions are
# wrong, or lead to other problems.
#

# When calling $(LIBTOOL) --mode=install cp..., libtool will take care of
# ranlib and strip. For enabling the stripping, we just have to pass the
# option '-s'.
ifneq ($(YICES_MODE),debug)
STRIP=-s
endif

# If DESTDIR is already set, does nothing. If DESTDIR has not been given,
# default to $(prefix).
# You can install in a staging area using 'make install DESTDIR=/path
# that will mimick the installation using $prefix but in DESTDIR.
installed_bin = yices_{main,sat,smt,smt2,smtcomp}
installed_lib = $(libyices_a) libyices.la
installed_include = yices{,_exit_codes,_limits,_types}.h
dist:
	rm -rf $(distdir) && $(MKDIR_P) $(distdir)/include $(distdir)/lib $(distdir)/bin
	$(LIBTOOL) $(LTFLAGS) --mode=install $(INSTALL_DATA) $(srcdir)/include/*.h $(dist_dir)/include
	($(LIBTOOL) $(LTFLAGS) --mode=install $(INSTALL_PROGRAM) $(STRIP) $(builddir)/bin/yices_*$(EXEEXT) $(dist_dir)/bin) || true
	($(LIBTOOL) $(LTFLAGS) --mode=install $(INSTALL) $(STRIP) $(builddir)/lib/libyices.* $(dist_dir)/lib) || true
install:
	$(MKDIR_P) $(DESTDIR)$(includedir) $(DESTDIR)$(libdir) $(DESTDIR)$(bindir)
	$(LIBTOOL) $(LTFLAGS) --mode=install $(INSTALL_DATA) $(srcdir)/include/*.h $(abspath $(DESTDIR))$(includedir)
	($(LIBTOOL) $(LTFLAGS) --mode=install $(INSTALL_PROGRAM) $(STRIP) $(builddir)/bin/yices_*$(EXEEXT) $(abspath $(DESTDIR))$(bindir)) || true
	($(LIBTOOL) $(LTFLAGS) --mode=install $(INSTALL) $(STRIP) $(builddir)/lib/libyices.* $(abspath $(DESTDIR))$(libdir)) || true
uninstall:
	$(LIBTOOL) $(LTFLAGS) --mode=uninstall rm -f $(DESTDIR)$(includedir)/yices_*.h
	($(LIBTOOL) $(LTFLAGS) --mode=uninstall rm -f $(DESTDIR)$(bindir)/yices_*) || true
	($(LIBTOOL) $(LTFLAGS) --mode=uninstall rm -f $(DESTDIR)$(libdir)/libyices*) || true

#
# SOURCE DISTRIBUTION
#
srcdist_dir = yices-$(PACKAGE_VERSION)
srcdist_tarfile= yices-$(PACKAGE_VERSION)-src.tar.gz

#
# Build the source tar file
#
# NOTE: chmod -R .. may fail on some Solaris versions
# - a workaround is to use a GNU/fileutils version of chmod
#   instead of SUN's own version
# - on CSL ungoliant: make sure /csl/bin is in the path before /bin
#   and /usr/bin
#
# For tests: we copy all unit tests and the regression tests in
# arrays/bv/efsmt/mantis/wd.
#
source-distribution: $(distributions) doc
	@ echo "=== Building $(srcdist_tarfile) ==="
	rm -f -r $(srcdist_dir)
	$(MKDIR_P) $(srcdist_dir)
	cp install-sh config.guess configure configure.ac config.sub $(srcdist_dir)
	cp Makefile make.include.in $(srcdist_dir)
	$(MKDIR_P) $(srcdist_dir)/autoconf
	cp autoconf/* $(srcdist_dir)/autoconf
	$(MKDIR_P) $(srcdist_dir)/configs
	$(MKDIR_P) $(srcdist_dir)/etc
	cp etc/README.source $(srcdist_dir)/README
	cp etc/LICENSE etc/NOTICES $(srcdist_dir)
	cp etc/* $(srcdist_dir)/etc
	$(MKDIR_P) $(srcdist_dir)/src
	rsync -R `find src/ -name "Makefile" -or -name "*.c" -or -name "*.h" -or -name "*.txt"` $(srcdist_dir)
	$(MKDIR_P) $(srcdist_dir)/doc
	$(MKDIR_P) $(srcdist_dir)/doc/manual
	cp doc/COMPILING doc/NOTES doc/YICES-LANGUAGE doc/GMP doc/manual/manual.pdf \
           $(srcdist_dir)/doc
	cp doc/manual/*.pdf doc/manual/*.tex doc/manual/*.bib \
           doc/manual/*.cls doc/manual/*.sty doc/manual/*.eps doc/manual/*.fig \
	   $(srcdist_dir)/doc/manual
	$(MKDIR_P) $(srcdist_dir)/utils
	cp utils/gmp-hack.h utils/mingw-build utils/setup-vs2010 utils/setup-vs2013 \
	   utils/yices_parser.txt utils/yices_input_tables.h \
	   utils/smt_parser.txt utils/smt_input_tables.h \
	   utils/smt2_parser.txt utils/smt2_input_tables.h \
	   utils/table_builder.c utils/truth_table_compiler.c \
	   utils/mkreadme utils/mkreadme-android utils/make_source_version \
	   utils/lib_name utils/gmp_version utils/yices_version \
           utils/remove_trailing_whitespaces \
           $(srcdist_dir)/utils
	$(MKDIR_P) $(srcdist_dir)/examples
	for file in `ls examples | $(EGREP) '\.ys$$|\.c$$|\.smt2$$' ` ; do \
	  cp examples/$$file $(srcdist_dir)/examples ; \
	done || true
	$(MKDIR_P) $(srcdist_dir)/tests
	cp -R tests/unit $(srcdist_dir)/tests
	$(MKDIR_P) $(srcdist_dir)/tests/regress
	for t in regress/arrays regress/bv regress/efsmt regress/mantis regress/wd ; do \
	   cp -R tests/$$t $(srcdist_dir)/tests/regress ; \
	done
	cp tests/regress/check.sh $(srcdist_dir)/tests/regress
	chmod -R og+rX $(srcdist_dir)
	COPYFILE_DISABLE=1 tar -czf $(distributions)/$(srcdist_tarfile) $(srcdist_dir)
	chmod -R og+rX $(distributions)
	rm -f -r $(srcdist_dir)


#
# CLEANUP
#

# The gperf-generated files will be cleaned ONLY if GPERF has been found.
clean:
	rm -f src/api/yices_version.c
	rm -rf $(builddir)
ifneq ($(GPERF),)
	rm -rf $(gperf_generated)
endif

distclean: clean
	rm -rf summary libtool config.* make.include autom4te.cache


# If we are using GNU make, the 'include' will try to rebuild every target
# (= every .d included) and after having rebuilt those targets, make will
# re-include them. This mecanism is inspired from the GNU make manual:
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
# This target is used when the .d are not present; the '-include' will try
# to rebuild the missing .d and this 'empty' target will be used. This is
# because the actual .d are created during the compilation step.
$(objdir)/%.d: ;
# With no '.PRECIOUS', the .d are removed because they are considered as
# 'intermediate' files.
.PRECIOUS: $(base_dep) $(extra_dep) $(bin_dep) $(tests_dep) \
		   $(base_obj) $(extra_obj) $(bin_obj) $(tests_obj)

-include $(base_dep)
-include $(extra_dep)
-include $(bin_dep)
-include $(tests_dep)


#
# For debugging of Makefile and configuration:
# print the options as set by this Makefile
#

info config configuration show-details: summary
	cat summary
