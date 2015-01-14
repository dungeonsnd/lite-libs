#!/bin/sh


cpprepos=~/.src/git_src/cppmaven/cpprepos/

d_libevent=$cpprepos/libevent-2.0.21-stable
d_cppfastdevelop=$cpprepos/cppfastdevelop


echo 'Using path dir_libevent:  '$dir_libevent
echo 'Using path dir_cppfastdevelop:  '$dir_cppfastdevelop

cd src

g++ -Wall -g -O0 server_main.cpp my_io_cb.cpp -o server_main  \
-I. -I../../include  \
-I $d_libevent/include  \
-I $d_cppfastdevelop/cppfoundation/include  \
-L../../lib/  \
-L $d_libevent/lib/  \
-L $d_cppfastdevelop/cppfoundation/bin/  \
-levent_core  \
-lcfclass_d  \
-lcfstub_d  \
-Wl,-rpath=../lib/

mv server_main ../

cd ../

