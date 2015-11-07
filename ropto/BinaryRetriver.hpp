//
//  BinaryRetriver.h
//  ropto
//
//  Created by Kimmy  on 11/7/15.
//  Copyright (c) 2015 TDCA. All rights reserved.
//

#ifndef ropto_BinaryRetriver_hpp
#define ropto_BinaryRetriver_hpp

#include <cstdint>

namespace ropto
{
    
    template<class T>
    union binary_retriver
    {
    public:
        T value;
        uint8_t bytes[sizeof(T)];
        binary_retriver() {}
        binary_retriver(const T& _value): value(_value) {}
    };
    

}

#endif
