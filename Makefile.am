SUBDIRS = src script share

runtest:	FORCE
	make format
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

FORCE:
