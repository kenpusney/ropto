
#include "Compress.hpp"

#include <zlib.h>
#include <cassert>
#include <algorithm>
#include <iterator>

using namespace std;

namespace ropto {
    void compressed::compress(const vector<byte_t>& bytes, int level)
    {
        int ret, flush;
        unsigned have;
        z_stream strm;
        
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        ret = deflateInit(&strm, level);
        assert(ret == Z_OK);
        size_t size = bytes.size();
        int read_offset = 0;
        while (size > 0) {
            auto start = begin(bytes) + read_offset;
            size_t read_cnt = size > chunk_size ? chunk_size : size;
            copy(start, start+read_cnt, begin(in));
            strm.avail_in = static_cast<unsigned>(read_cnt);
            strm.next_in = in;
            
            flush = (size > chunk_size) ? Z_NO_FLUSH : Z_FINISH;
            
            do {
                strm.avail_out = chunk_size;
                strm.next_out = out;
                
                ret = deflate(&strm, flush);
                assert(ret != Z_STREAM_ERROR);
                have = chunk_size - strm.avail_out;
                copy(begin(out), begin(out)+have, back_inserter(cdata));
            } while (strm.avail_out == 0);
            assert(strm.avail_in == 0);
            size -= read_cnt;
        }
        deflateEnd(&strm);
    }
    
    vector<uint8_t> compressed::extract()
    {
        int ret;
        unsigned have;
        z_stream strm;
        bytes_t bytes {};
       
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = nullptr;
        
        ret = inflateInit(&strm);
        
        assert(ret == Z_OK);
        
        size_t size = cdata.size();
        size_t read_offset = 0;
        while (size) {
            auto start = begin(cdata) + read_offset;
            size_t read_cnt = size > chunk_size ? chunk_size : size;
            copy(start, start+read_cnt, begin(in));
            strm.avail_in = static_cast<unsigned>(read_cnt);
            strm.next_in = in;
            
            do {
                strm.avail_out = chunk_size;
                strm.next_out = out;
                
                ret = inflate(&strm, Z_NO_FLUSH);
                assert(ret != Z_STREAM_ERROR);
                
                have = chunk_size - strm.avail_out;
                std::copy(out, out+have, back_inserter(bytes));
            } while (strm.avail_out == 0);
            
            size -= read_cnt;
        }
        
        inflateEnd(&strm);
        return bytes;
    }
}
