
#ifndef ropto_Compress_hpp
#define ropto_Compress_hpp

#include <cstdint>
#include <vector>
#include <array>

namespace ropto {
    class compressed
    {
        typedef uint8_t byte_t;
        typedef std::vector<byte_t> bytes_t;
        static constexpr size_t chunk_size = 131072;
        bytes_t cdata;
        byte_t in[chunk_size];
        byte_t out[chunk_size];
    public:
        compressed(): cdata{}, in{}, out {}
        {}
        compressed(const compressed& compressed) = delete;
        
        
        bytes_t extract();
        
        void compress(const bytes_t& bytes, int level = -1);
        
        const bytes_t& data() const {return cdata;}
        
        void data(const bytes_t& data) {
            bytes_t{}.swap(cdata);
            cdata.insert(begin(cdata), begin(data), end(data));
        }
        
        enum class level
        {
            default_compress = -1,
            no_compress = 0,
            best_speed = 1,
            super_speed, // 2
            fast_speed,  // 3
            good_speed,  // 4
            balanced,    // 5
            good_comression,    // 6
            better_compression, // 7
            super_compression,  // 8
            best_compression = 9
        };
    };
}

#endif
