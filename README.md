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

## run sample

```
$ cd sample
$ mpispec -np 3
```

if you had undefined reference and use ubuntu, you probably need to install binutils-gold.

```
$ apt-get install binutils-gold
```

## run unity test

```
$ bundle install --path vendor/bundle --binstubs .bundle/bin
$ rake
```

