MPISpec
=======
A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.

## Requirements

- OpenMPI

## Install

```
$ git clone git@github.com:YuuShigetani/mpispec.git
$ cd mpispec
$ VAR=`locate ompi/request/request.h` | export C_INCLUDE_PATH=$C_INCLUDE_PATH:${VAR%/ompi/request/request.h}
$ ./autogen.sh
$ ./configure
$ make
$ make install
```

if you have errors, you have to write in your .bashrc etc:

```
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/your/openmpi/include/path
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/your/shared-object-file/path
```

## Sample

```
$ cd sample
$ mpispec -np 3
```

if you have errors of undefined reference, you probably need to install binutils.

```
$ apt-get install binutils
```

## Development

### clang-format

```
$ clang-format-3.5 -i -style="{BasedOnStyle: Google, IndentWidth: 4}" src/mpispec/*.c src/mpispec/*.h sample/src/*.c sample/src/*.h
```

### clib

```
$ clib install -o src
```

=======
LGPL Lisence

Author: Yuu Shigetani

Year:   2013, 2014

