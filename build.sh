#!/bin/sh


flex  -o dbase.lex.cc -l  dbase.lpp
bison -o dbase.tab.cc \
    -Wno-conflicts-sr \
    -Wno-conflicts-rr \
    -Wno-other        \
    -vd dbase.ypp

g++ -O2 -std=c++20 -o dbase2fpc dbase.lex.cc dbase.tab.cc -lole32
strip dbase2fpc.exe
