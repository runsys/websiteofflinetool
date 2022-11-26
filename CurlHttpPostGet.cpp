#include "CurlHttpPostGet.h"
#include "..\..\UtilityFunc\UrlEncodeDecode.h"
#include <QCryptographicHash>
#include "CUrlDirectDowner.h"
#include <QTime>
extern "C"
{
#include "zlib.h"
}


/*
in global initialize with:
1:
CURLcode res = ::curl_global_init(CURL_GLOBAL_ALL);
if (CURLE_OK != res)
{
//failed
}

2:
::curl_global_cleanup();
*/


std::string g_tokenkey="95305f931a8e4322810b59a3c7754f913e2bc425";

int curlstrsplit(std::string str, std::string split, std::deque < std::string> &strArray)
{
    strArray.clear();

    std::string strTemp = str; //此赋值不能少
    int nIndex = 0; //
    while (1)
    {
        nIndex = strTemp.find(split);
        if (nIndex != std::string::npos)
        {
            std::string addstr = strTemp.substr(0, nIndex);
            if (!addstr.empty())strArray.push_back(addstr);
            strTemp = strTemp.substr(nIndex + split.size());
        }
        else break;
    }
    if (!strTemp.empty())strArray.push_back(strTemp);
    return strArray.size();
}


int http_ungzip(char *source, int len, char **dest, int *destlen, bool bgzip)
{
#define CHUNK 409600
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK];
    int totalsize = 0;

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    if (bgzip)
        ret = inflateInit2(&strm, 47);
    else
        ret = inflateInit(&strm);

    if (ret != Z_OK)
        return ret;

    strm.avail_in = len;
    strm.next_in = (Byte*)source;

    /* run inflate() on input until output buffer not full */
    do {
        strm.avail_out = CHUNK;
        strm.next_out = out;
        ret = inflate(&strm, Z_NO_FLUSH);
        //assert(ret != Z_STREAM_ERROR); /* state not clobbered */
        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR; /* and fall through */
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            inflateEnd(&strm);
            return ret;
        }
        have = CHUNK - strm.avail_out;
        totalsize += have;
        *dest = (char*)realloc(*dest, totalsize);
        memcpy(*dest + totalsize - have, out, have);
    } while (strm.avail_out == 0);
    *destlen = totalsize;
    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}






size_t CurlHttpPostGet_string_write_func(char *data, size_t size, size_t nmemb, void *writer);

//url:example:http://127.0.0.1/getpost.php
int CurlHttpPost(std::string url, std::map<std::string, std::string>  *postfieldmap, std::string *out_result_data,const char *cookiefile, long withheader, long timeout)
{
	if (out_result_data == nullptr)
		return -1;
	std::string postdata;
	if (postfieldmap != nullptr)
	{
		for (std::map<std::string, std::string>::iterator it = postfieldmap->begin(); it != postfieldmap->end(); it++)
		{
			if (postdata.size())
				postdata += "&";
            postdata += it->first + "=" + it->second;
		}
	}
	out_result_data->clear();
	CURL * m_curl;
	m_curl = ::curl_easy_init();
	::curl_easy_reset(m_curl);
    ::curl_easy_setopt(m_curl, CURLOPT_COOKIEFILE, cookiefile);//"cookiefile.txt");
    ::curl_easy_setopt(m_curl, CURLOPT_COOKIEJAR, cookiefile);//"cookiefile.txt");
	::curl_easy_setopt(m_curl, CURLOPT_HEADER, withheader);
	::curl_easy_setopt(m_curl, CURLOPT_URL, url);
    ::curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, postdata.c_str());
    ::curl_easy_setopt(m_curl, CURLOPT_POST, 1);
	::curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, timeout);
	::curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CurlHttpPostGet_string_write_func);
	::curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, out_result_data);
	CURLcode res = ::curl_easy_perform(m_curl);

	::curl_easy_cleanup(m_curl);

	return (0 == res) ? 0 : -1;
}

int CurlHttpGet(std::string url, std::map<std::string, std::string>  *postfieldmap, CUrlDirectorDownResult *downer_result, int withheader, int timeout, std::string useragent, std::string jarfile, unsigned int NeedDataBit, long long int rangestart, long long int rangeend, CUrl_downer_write_func_type writefunc, void *writedata)
{
    if (downer_result == nullptr)
		return -1;

    CUrlDirectorDownOption       downer_opt;
    downer_opt.httpheaders.push_back("Accept-Encoding: gzip,defalte");
	if (useragent.size()>0)
		downer_opt.useragent= useragent;
	downer_opt.cookiefile = jarfile;
	downer_opt.cookiejar = jarfile;
    downer_opt.timeout=timeout;
	downer_opt.NeedDataBit = NeedDataBit;
	downer_opt.rangestart = rangestart;
	downer_opt.rangeend= rangeend;
	downer_opt.writefunc = writefunc;
	downer_opt.writedata = writedata;
    int downer_retcode=0;

    QTime t1;
    t1.start();
	downer_result->targetHeader.clear();
	downer_result->targetCoentent.clear();
    downer_retcode = CUrlDirectorDowner(url.data(), &downer_opt, downer_result,postfieldmap);
    clock_t t2=clock();
    //LBLOGD(WARNING<<url<<"网络用时(m):"<<t1.elapsed());

    if (downer_result->targetHeader.find("Content-Encoding: gzip") != std::string::npos
        || downer_result->targetHeader.find("Content-Encoding: Gzip") != std::string::npos
        )
    {
        char *dd2 = 0;
        int dd2len = 0;
        int rl = http_ungzip((char*)downer_result->targetCoentent.c_str(), downer_result->targetCoentent.size(), &dd2, &dd2len, true);
        if (rl == Z_OK)
            downer_result->targetCoentent.assign((char*)dd2, dd2len);
        if (dd2)free(dd2);
    }
    else if (downer_result->targetHeader.find("Content-Encoding: Deflate") != std::string::npos
        || downer_result->targetHeader.find("Content-Encoding: deflate") != std::string::npos
        )
    {
        char *dd2 = 0;
        int dd2len = 0;
        int rl = http_ungzip((char*)downer_result->targetCoentent.c_str(), downer_result->targetCoentent.size(), &dd2, &dd2len, false);
        if (rl == Z_OK)
            downer_result->targetCoentent.assign((char*)dd2, dd2len);
        if (dd2)free(dd2);
    }




    return downer_retcode;
}



size_t CurlHttpPostGet_string_write_func(char *data, size_t size, size_t nmemb, void *writer)
{
	if (data){
		size_t sizes = size * nmemb;
		((std::string*)writer)->append(data, sizes);
		return sizes;
	}
	return 0;
}

