//
//  Serializer.cpp
//  ropto
//
//  Created by Kimmy  on 11/7/15.
//  Copyright (c) 2015 TDCA. All rights reserved.
//

#ifndef ropto_SerializerSupport_hpp
#define ropto_SerializerSupport_hpp

#include "BinaryRetriver.hpp"
#include "Serializer.hpp"

namespace ropto
{
    template<class T>
    class serializer<T, true>
    {
    public:
        static T from_bytes(byte_stream& stream)
        {
            binary_retriver<T> retriver;
            for(int i = 0; i < sizeof(retriver); i++)
                retriver.bytes[i] = stream.fetch();
            return retriver.value;
        }
        static void to_bytes(const T& value, byte_stream& stream)
        {
            binary_retriver<T> retriver{value};
            for (auto byte: retriver.bytes)
                stream.append(byte);
        }
    };
    
    template<>
    class serializer<int>
    {
    public:
        static int from_bytes(byte_stream& stream)
        {
            int value = 0;
            for(int i = 3; i >= 0; i--)
            {
                value += stream.fetch() << (8*i);
            }
            return value;
        }
        
        static void to_bytes(const int& value, byte_stream& stream)
        {
            for(int i = 3; i >= 0; i--)
            {
                uint8_t byte = (value >> (8*i)) & 0xFF;
                stream.append(byte);
            }
        }
    };
}

#endif