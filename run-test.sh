#!/bin/sh

make
sudo make install
cd sample/
mpispec -np 3
cd ..
