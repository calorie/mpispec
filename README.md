MPISpec
=======
A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.

## requirements

- MPICH >= 1.2.0

## install

```
$ git clone git@github.com:YuuShigetani/mpispec.git
$ cd mpispec
$ ./autogen.sh
$ ./configure
$ make
$ make install
```

if you have errors, you have to write in your .bashrc etc:

```
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/your/mpich/include/path
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/your/shared-object-file/path
```

## run sample

```
$ cd sample
$ mpispec -np 3
```

if you have errors of undefined reference, you probably need to install binutils-gold.

```
$ apt-get install binutils-gold
```

## run unity test

```
$ bundle install --path vendor/bundle --binstubs .bundle/bin
$ bundle exec rake
```

=======
LGPL Lisence

Author: Yuu Shigetani

Year:   2013, 2014

