#!/bin/sh

dir_libevent=~/.src/libevent/libevent-2.0.21-stable
dir_cppfastdevelop=~/.src/cppfastdevelop

echo 'Using path dir_libevent:  '$dir_libevent
echo 'Using path dir_cppfastdevelop:  '$dir_cppfastdevelop

cd src

g++ -Wall -g -O0 server_main.cpp my_io_cb.cpp -o server_main  \
-I. -I../../include  \
-I $dir_libevent/include  \
-I $dir_cppfastdevelop/cppfoundation/include  \
-L../../lib/  \
-L $dir_libevent/lib/  \
-L $dir_cppfastdevelop/cppfoundation/bin/  \
-levent_core  \
-lcfclass_d  \
-lcfstub_d  \
-Wl,-rpath=../lib/

mv server_main ../

cd ../

