#!/bin/sh

# Time-stamp: <2007-11-14 22:30:03 shinya>

aclocal -I config
libtoolize --force --copy
automake --add-missing --foreign --copy
autoconf
