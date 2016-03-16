#/bin/sh

clang++ src/*.cpp -std=c++1z -stdlib=libc++ -dynamiclib -o libropto.dylib

clang++ test/*.cpp -std=c++1z -stdlib=libc++ -Isrc -L. -lropto -o run_test

