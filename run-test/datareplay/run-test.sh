#!/bin/sh

CC="mpicc"
LOGLIBS="-L$HOME/local/lib -ldatalog"
REPLAYLIBS="-L$HOME/local/lib -ldatareplay"
CUFLAGS="-I$HOME/local/include"
CULIBS="-L$HOME/local/lib -lcue -lm"

gmake
gmake install
$CC $LOGLIBS test/datareplay/pre_test/pre_test.c -o pre_test
mpirun -np 1 ./pre_test
rm -f *.o ./pre_test
$CC $CUFLAGS $CULIBS $REPLAYLIBS test/datareplay/*.c -o test_main
./test_main rank0.log
rm -f *.o ./test_main rank0.log
