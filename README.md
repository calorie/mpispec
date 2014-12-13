MPISpec
=======
A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.

## Requirements

- [OpenMPI](http://www.open-mpi.org)

## Install

```
$ git clone https://github.com/calorie/mpispec.git
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

### [clang-format](http://clang.llvm.org/docs/ClangFormat.html)

```
$ make format
```

### [clib](https://github.com/clibs/clib)

```
$ clib install -o src
```

### [valgrind](http://valgrind.org)

```
$ make memcheck
```

=======
LGPL Lisence

Author: Yuu Shigetani

Year:   2013, 2014
