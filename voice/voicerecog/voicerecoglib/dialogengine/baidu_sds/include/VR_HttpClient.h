#ifndef VR_HTTPCLIENT_H
#define VR_HTTPCLIENT_H

#include <string>

struct WriteThis {
    const char *readptr;
    size_t sizeleft;
};

class VR_HttpClient
{
public:
    VR_HttpClient();
    ~VR_HttpClient();

    /**
    * @brief HTTP POST请求
    * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
    * @param strPost 输入参数,使用如下格式para1=val1¶2=val2&…
    * @param strResponse 输出参数,返回的内容
    * @return 返回是否Post成功
    */
    int Post(const std::string & strPost, std::string & strResponse);

    /**
    * @brief HTTP GET请求
    * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
    * @param strResponse 输出参数,返回的内容
    * @return 返回是否Post成功
    */
    int Get(const std::string & strUrl, std::string & strResponse);
};

#endif // VR_HTTPCLIENT_H
