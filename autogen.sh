#!/bin/sh

aclocal -I config
libtoolize --force --copy
automake --add-missing --foreign --copy
autoconf
