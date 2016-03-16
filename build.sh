#/bin/sh

clang++ src/*.cpp -std=c++1z -stdlib=libc++ -Iinclude -dynamiclib -o libropto.dylib

clang++ test/*.cpp -std=c++1z -stdlib=libc++ -Iinclude -L. -lropto -o run_test

