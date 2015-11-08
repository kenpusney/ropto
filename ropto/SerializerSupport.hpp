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
#include "Optional.hpp"


namespace ropto
{
    template<class T>
    class serializer<T, true, false, false>
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
    
    template<class Integral>
    class serializer<Integral, true, true, false>
    {
    public:
        static Integral from_bytes(byte_stream& stream)
        {
            Integral value = 0;
            for(size_t i = sizeof(Integral); i > 0; i--)
            {
                value += static_cast<Integral>(stream.fetch()) << (8*(i-1));
            }
            return value;
        }
        
        static void to_bytes(const Integral& value, byte_stream& stream)
        {
            for(size_t i = sizeof(Integral); i > 0; i--)
            {
                uint8_t byte = (value >> (8*(i-1))) & 0xFF;
                stream.append(byte);
            }
        }
    };
    
    template<class T, int size=sizeof(T)>
    union Integral;
    
    template<class T>
    union Integral<T, 1> {int8_t i; T t;};
    
    template<class T>
    union Integral<T, 2> { int16_t i; T t; };
    
    template<class T>
    union Integral<T, 4> { int32_t i; T t; };
    
    template<class T>
    union Integral<T, 8> { int64_t i; T t; };
    
    template<class T>
    union Integral<T, 16> { __int128_t i; T t; };
    
    template<class Floating>
    class serializer<Floating, true, false, true>
    {
        typedef Integral<Floating> Int;
    public:
        static Floating from_bytes(byte_stream& stream)
        {
            Int value {};
            read(value.i, stream);
            return value.t;
        }
        static void to_bytes(const Floating& value, byte_stream& stream)
        {
            Int ivalue {};
            ivalue.t = value;
            write(ivalue.i, stream);
        }
    };
    
    template<class T>
    class serializer<std::vector<T>, false, false, false>
    {
    public:
        static std::vector<T> from_bytes(byte_stream& stream)
        {
            size_t count = read<size_t>(stream);
            std::vector<T> value{};
            value.reserve(count);
            for (int i = 0; i < count; i++)
            {
                value.push_back(read<T>(stream));
            }
            return value;
        }
        
        static void to_bytes(const std::vector<T>& value, byte_stream& stream)
        {
            write(value.size(), stream);
            for (auto item: value)
                write(item, stream);
        }
    };
    
    template<class T>
    class serializer<std::basic_string<T>, false, false, false>
    {
    public:
        static std::basic_string<T> from_bytes(byte_stream& stream)
        {
            size_t count = read<size_t>(stream);
            std::basic_string<T> value;
            value.reserve(count);
            for (int i = 0; i < count; i++)
            {
                value.append(1, read<T>(stream));
            }
            return value;
        }
        
        static void to_bytes(const std::basic_string<T>& value, byte_stream& stream)
        {
            write(value.size(), stream);
            for (T character: value)
                write(character, stream);
        }
    };
    
    template<class T>
    class serializer<optional<T>, false, false, false>
    {
    public:
        static optional<T> from_bytes(byte_stream& stream)
        {
            T value;
            bool valid = false;
            read(stream, valid);
            if (valid)
                read(stream, value);
            return valid ? optional<T>{value} : optional<T> {};
        }
        
        static void to_bytes(const optional<T>& value, byte_stream& stream)
        {
            write(static_cast<bool>(value), stream);
            if(value)
                write(value.value(), stream);
        }
    };
}

#endif