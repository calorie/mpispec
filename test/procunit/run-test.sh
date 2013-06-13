#!/bin/sh

gmake
gmake install
cd sample/
proclog -np 2
procut rank0.log
rm -f *.log
cd ..
