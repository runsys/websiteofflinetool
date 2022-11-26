#ifndef CURLDIRECTDOWNER_H
#define CURLDIRECTDOWNER_H


#include <string>
#include <fstream>
#include <deque>
#include <map>


typedef size_t (*CUrl_downer_write_func_type)(char *data, size_t size, size_t nmemb, void *writerData);
typedef double (*CUrl_downer_process_func_type)(void *shower,double dltotal,double dlnow,double ultotal,double ulnow);


extern size_t CUrl_downer_default_buf_write_func(char *data, size_t size, size_t nmemb, void *writer);
extern size_t CUrl_downer_default_file_write_func(char *data, size_t size, size_t nmemb, void *writer);
extern double CUrl_downer_default_process_func(void *shower,double dltotal,double dlnow,double ultotal,double ulnow);



enum CUrlDD_NeedDataBits
{
    CUrlDD_NeedDataBit_Content =1,
    CUrlDD_NeedDataBit_Header =2,
    CUrlDD_NeedDataBit_Cookie =4,
};


struct CUrlDirectorDownOption
{
    CUrlDirectorDownOption()
    {
        timeout=30;
        proxytype=-1;
        proxyport=-1;
        isfollowlocation=1;
        NeedDataBit = CUrlDD_NeedDataBit_Content | CUrlDD_NeedDataBit_Header | CUrlDD_NeedDataBit_Cookie;
        writefunc=NULL;
        processfunc=NULL;
        rangestart=0;
        rangeend=-1;
		writedata = 0;
        connecttimeout=30;
        useragent = "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) QtWebEngine/5.7.1 Chrome/49.0.2623.111 Safari/537.36";
    }
    std::string						cookiefile;//CURLOPT_COOKIEFILE会使curl下一次发请求时从指定的文件中读取cookie。
    std::string						cookiejar;//CURLOPT_COOKIEJAR会使curl在调用 curl_easy_cleanup的时候把cookie保存到指定的文件中。
    std::string						cookiestr;//CURLOPT_COOKIEFILE会使curl下一次发请求时从指定的文件中读取cookie。
    std::string						useragent;
    int								timeout;
    int								connecttimeout;
    int								proxytype;
    std::string						proxy;
    int								proxyport;
    std::string						proxyuser;
    std::string						proxypassword;
    std::string						proxynoproxy;
    int								isfollowlocation;
        std::string						postflieds;//string post field
    std::string						savedir;
    std::string						savefilename;
	void							*writedata;
    CUrl_downer_write_func_type		writefunc;
    CUrl_downer_process_func_type	processfunc;
    unsigned int					NeedDataBit;//0 to get all.
    std::deque<std::string>			httpheaders;//"Name: Value"
    long long						rangestart;
    long long						rangeend;
};


struct CUrlDirectorDownResult
{
public:
    std::deque<std::string>			targetCookies;
    std::deque<std::string>			redirectUrlChain;
    std::string						targetCoentent;
    long long						targetContentLength;
    std::string						targetHeader;
};


//0 success otherwise faile
int CUrlDirectorDowner(const char *url,CUrlDirectorDownOption *downopt,CUrlDirectorDownResult *downresult, std::map<std::string, std::string>  *postfieldmap);
//0 success otherwise faile
int CUrlDirectorDowner(const wchar_t *url, CUrlDirectorDownOption *downopt, CUrlDirectorDownResult *downresult, std::map<std::string, std::string>  *postfieldmap);




#endif
