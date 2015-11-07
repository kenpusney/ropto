//
//  Serializer.hpp
//  ropto
//
//  Created by Kimmy  on 11/7/15.
//  Copyright (c) 2015 TDCA. All rights reserved.
//

#ifndef ropto_Serializer_hpp
#define ropto_Serializer_hpp

#include "ByteStream.hpp"

#include <type_traits>

namespace ropto
{
    
    template<class T, bool = std::is_pod<T>::value>
    class serializer;
    
    template<class T>
    T read(byte_stream& stream)
    {
        return serializer<T>::from_bytes(stream);
    }
    
    template<class T>
    void write(const T& value, byte_stream& stream)
    {
        return serializer<T>::to_bytes(value, stream);
    }

}

#endif
