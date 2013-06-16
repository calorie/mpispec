#!/bin/sh

make
sudo make install
cd ~/work/hpcss/mpispec_sample/
mpispec -np 3
cd ..
