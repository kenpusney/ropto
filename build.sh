#!/bin/sh

cd ropto
clang++ *.cpp -o ropto_test -lz -std=c++11
