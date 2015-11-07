//
//  Base64.h
//  ropto
//
//  Created by Kimmy  on 11/7/15.
//  Copyright (c) 2015 TDCA. All rights reserved.
//

#ifndef ropto_Base64_hpp
#define ropto_Base64_hpp

#include <string>
#include <vector>

namespace ropto
{
    std::string base64_encode(const std::vector<uint8_t>& bytes);
    
    std::vector<uint8_t> base64_decode(const std::string& input);
}


#endif /* defined(__ropto__Base64__) */
