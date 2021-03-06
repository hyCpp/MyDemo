/**
 * Copyright (c) 2017 Baidu.com, Inc. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * @author baidu aip
 */
#ifndef __BAIDUSDK_BASE64_H__
#define __BAIDUSDK_BASE64_H__

#include <string>

namespace baiduSDK {
    static inline bool is_base64(const char c);

    std::string base64_encode(const char * bytes_to_encode, unsigned int in_len);
    std::string base64_decode(std::string const & encoded_string);
    
}
#endif
