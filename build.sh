#!/bin/sh

flex  -o dbase.lex.cc -l  dbase.lpp
bison -o dbase.tab.cc -vd dbase.ypp

g++ -D_GLIBCXX_USE_CXX11_ABI=0 -std=c++14 -o dbase2delphi dbase.lex.cc dbase.tab.cc -lfl -ly
