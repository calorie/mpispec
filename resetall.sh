#!/bin/sh

sudo make uninstall
sudo make clean
./bootstrap
./configure --prefix=/usr/local
./run.sh
