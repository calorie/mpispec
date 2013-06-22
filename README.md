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

