all: errors todos
	
	

clean:
	$(RM) *.txt

distclean:
	$(RM) ../src/errors.h ../src/yices2.mli

###############################################################################

errors: ../src/errors.h

../src/errors.h: ../ext/yices/src/include/yices_types.h prep_enum.py
	python prep_enum.py $< ../src/yices2.ml ../src/yices2.mli ../src/errors.h

###############################################################################

todos: stubs_todo.txt intf_todo.txt

intf_all.txt: ../src/*.c
	grep '\<ocamlyices_\w*' $^ -oh | sort -u > $@

intf_done.txt: ../src/*.mli
	grep '\<ocamlyices_\w*' $^ -oh | sort -u > $@

stubs_all.txt: ../ext/yices/src/include/yices.h
	grep '\<yices_\w*\(\.h\)\?' $< -oh | grep '\.h$$' -v| sort -u > $@

stubs_done.txt: ../src/*.c
	grep '\<yices_\w*' $^ -oh | sort -u > $@

%_todo.txt: %_all.txt %_done.txt
	comm -23 $^ > $@

.PHONY:
	all clean errors todos
