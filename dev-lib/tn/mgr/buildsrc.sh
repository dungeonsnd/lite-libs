#!/bin/sh

cpprepos=~/.src/git_src/cppmaven/cpprepos/

d_est=$cpprepos/est-0.1
d_libevent=$cpprepos/libevent-2.0.21-stable
d_cppfastdevelop=$cpprepos/cppfastdevelop
d_hiredis=$cpprepos/hiredis-master
cd src
:x
g++ -Wall -g -O0 server_main.cpp my_io_cb.cpp cache_cmd.cpp -o server_main  \
-I $d_est/include  \
-I $d_libevent/include  \
-I $d_cppfastdevelop/cppfoundation/include  \
-I $d_hiredis  \
-L../lib/  -levent_core -lcfclass_d -lcfstub_d -lhiredis  \
-Wl,-rpath=../lib/

mv server_main ../bin/
cd ../

