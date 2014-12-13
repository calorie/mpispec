SUBDIRS = src script share

runtest:	FORCE
	make all
	sudo make install
	make sample

reinstall:	FORCE
	sudo make uninstall clean
	./autogen.sh
	./configure --prefix=/usr/local
	make runtest

sample:	FORCE
	cd sample ; mpispec -np 3

format:	FORCE
	find src/mpispec sample/src -type f -name '*.c' -o -name '*.h' -a ! -name 'mpispec.h' | xargs clang-format -i -style=file

deps:	FORCE
	clib install -o src

memcheck:	FORCE
	cd sample ; mpispec -np 3 valgrind -v --leak-check=full --show-reachable=yes --log-file=valgrind.%p.log --suppressions=/usr/local/share/openmpi/openmpi-valgrind.supp

rmdust:	FORCE
	rm -f sample/rank*.xml sample/rank*.result sample/valgrind*.log sample/exec_test

FORCE:
