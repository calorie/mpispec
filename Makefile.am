
# Time-stamp: <2007-11-14 22:29:40 shinya>

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
	clang-format-3.5 -i -style="{BasedOnStyle: Google, IndentWidth: 4}" src/mpispec/*.c src/mpispec/*.h sample/src/*.c sample/src/*.h

deps:	FORCE
	clib install -o src

FORCE:
