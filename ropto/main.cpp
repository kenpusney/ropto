#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <cassert>

#include "ropto.hpp"

using namespace ropto;

struct blob {
    int x;
    short y;
    char z;
};

int main(int argc, const char * argv[]) {
    
    std::string hello { "hello world" };
    
    byte_stream stream;
    blob x {1, 2, '3'};
    write(x, stream);
    
    auto base64 = base64_encode(stream.iterate());
    
    byte_stream decoded;
    
    decoded.iterate() = base64_decode(base64);
    
    blob y = read<blob>(decoded);
    
    std::cout << y.x << y.y << y.z;
}
