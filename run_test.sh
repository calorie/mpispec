#!/bin/sh

make
sudo make install
# rake test:all
cd sample/
mpispec -np 3
cd ..
