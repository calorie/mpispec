#!/bin/sh

make
sudo make install
cd sample/
mpiut -np 2
cd ..
