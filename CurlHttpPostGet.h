#ifndef CURLHTTPPOSTGET_H
#define CURLHTTPPOSTGET_H

extern "C"
{
	#include <curl/curl.h>
}
#include <string.h>
#include <string>
#include <map>
#include "..\..\UtilityFunc\UrlEncodeDecode.h"
#include "..\..\UtilityFunc\Base64EncodeDecode.h"
#include "CUrlDirectDowner.h"


//url:example:http://127.0.0.1/getpost.php
int CurlHttpPost(std::string url, std::map<std::string, std::string>  *postfieldmap, std::string *out_result_data, const char *cookiefile,long withheader = 0, long timeout = 30);
int CurlHttpGet(std::string url, std::map<std::string, std::string>  *postfieldmap, CUrlDirectorDownResult *downer_result, int withheader = 0, int timeout = 30, std::string useragent = "", std::string jarfile = "", unsigned int NeedDataBit = CUrlDD_NeedDataBit_Content | CUrlDD_NeedDataBit_Header | CUrlDD_NeedDataBit_Cookie, long long int rangestart = 0, long long int rangeend = -1, CUrl_downer_write_func_type writefunc = 0, void *writedata = 0);




#endif
