
# Time-stamp: <2007-11-14 22:29:40 shinya>

SUBDIRS = src script share

runtest:	FORCE
	make all
	sudo make install
	make sample

reinstall:	FORCE
	sudo make uninstall clean
	./bootstrap
	./configure --prefix=/usr/local
	make runtest

sample:	FORCE
	cd sample ; mpispec -np 3

FORCE:
