mpispec
=======
## requirements

- MPICH >= 1.2.0

## install

```
$ git clone git@github.com:YuuShigetani/mpispec.git
$ cd mpispec
$ ./bootstrap
$ ./configure
$ make
$ sudo make install
```

if you had errors, you had to write in your .bashrc etc.

```
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/your/mpich/include/path
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/your/shared-object-file/path
```

if you had undefined error and use ubuntu, you probably need to install binutils-gold.

```
$ apt-get install binutils-gold
```

## run tests

```
$ cd sample
$ mpispec -np 3
```

## uninstall

```
$ cd /path/to/mpispec
$ sudo make uninstall
```

