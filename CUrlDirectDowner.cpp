#include "CUrlDirectDowner.h"
#include "shlwapi.h"

extern "C"
{
	#include "curl/curl.h"
}

#include <QMutex>

QMutex mu;
size_t CUrl_downer_default_file_write_func(char *data, size_t size, size_t nmemb, void *writer)
{
    if(data){
        size_t sizes = size * nmemb;
        //fwrite(data,1,sizes,(FILE*)writer);
        DWORD wcnt;
        WriteFile((HANDLE)writer, data, sizes, &wcnt, 0);
        return sizes;
    }
    return 0;
}



double CUrl_downer_default_process_func(void *shower,double dltotal,double dlnow,double ultotal,double ulnow)
{
    return dltotal;
}



int CUrlDirectorDowner(const char *url, CUrlDirectorDownOption *downopt, CUrlDirectorDownResult *downresult, std::map<std::string, std::string>  *postfieldmap)
{
    downopt->NeedDataBit|=CUrlDD_NeedDataBit_Header;
	int			downloadrl=-1;
	CURLcode	curlretcode;
	CURL		*curlhandle=NULL;
	time_t		time_start = time(NULL);

	downresult->targetCoentent.clear();
	downresult->targetHeader.clear();

	if(url==NULL)
	{
		return -1;
	}

	curlhandle = curl_easy_init();
	if(curlhandle==NULL)
	{
		return -1;
	}
	//proxy start
	if(!downopt->proxy.empty() && downopt->proxytype!=-1) 
	{
		curlretcode=curl_easy_setopt(curlhandle,CURLOPT_PROXYTYPE,downopt->proxytype);
		curlretcode=curl_easy_setopt(curlhandle,CURLOPT_PROXY,downopt->proxy.c_str());
		curlretcode=curl_easy_setopt(curlhandle,CURLOPT_PROXYPORT,downopt->proxyport);
		curlretcode=curl_easy_setopt(curlhandle,CURLOPT_USERNAME,downopt->proxyuser.c_str());
		curlretcode=curl_easy_setopt(curlhandle,CURLOPT_PROXYPASSWORD,downopt->proxypassword.c_str());
	}
	//proxy end

	//timeout start
	if(downopt->timeout!=-1)
	{
		curlretcode=curl_easy_setopt(curlhandle,CURLOPT_TIMEOUT,downopt->timeout);
    }
    downopt->httpheaders.push_back("Expect:");
	//timeout end

    //connect timeout start
    if(downopt->connecttimeout!=-1)
    {
        curlretcode=curl_easy_setopt(curlhandle,CURLOPT_CONNECTTIMEOUT,downopt->connecttimeout);
    }
    downopt->httpheaders.push_back("Expect:");
    //connect timeout end


    //post data
    struct curl_httppost *formpost = 0;
    struct curl_httppost *lastptr = 0;
    bool b_have_postfield = false;
    if (postfieldmap != nullptr)
    {
        for (std::map<std::string, std::string>::iterator it = postfieldmap->begin(); it != postfieldmap->end(); it++)
        {
            curl_formadd(&formpost, &lastptr, CURLFORM_PTRNAME, it->first.c_str(), CURLFORM_PTRCONTENTS, it->second.c_str(), CURLFORM_CONTENTSLENGTH, it->second.size(),CURLFORM_END);
            b_have_postfield = true;
        }
    }

    if (b_have_postfield == true)
    {
        curl_easy_setopt(curlhandle, CURLOPT_HTTPPOST, formpost);
    }

	//follow location start
	{
		curlretcode=curl_easy_setopt(curlhandle,CURLOPT_FOLLOWLOCATION,downopt->isfollowlocation);
	}
	//follow location end

	//user agent start
	//like:User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.64 Safari/537.31
	//Mozilla/5.0 (Windows NT 6.3; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0
	//Android Webkit Browser User Agent Strings:
	//Mozilla/5.0 (Linux; U; Android 4.0.3; ko-kr; LG-L160L Build/IML74K) AppleWebkit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30
	//Mozilla/5.0 (Linux; U; Android 2.3.3; en-us; HTC_DesireS_S510e Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile
	if (downopt->useragent.size())
	{
		curlretcode=curl_easy_setopt(curlhandle,CURLOPT_USERAGENT,downopt->useragent.c_str());
	}
	//user agent end

	//post flieds start
	if(!downopt->postflieds.empty())
	{
		curlretcode = curl_easy_setopt(curlhandle, CURLOPT_POSTFIELDS, downopt->postflieds.c_str());
	}
	//post flieds end


	//post flieds star
	if(downopt->rangeend!=-1)
	{
		char rangestr[100];
		sprintf(rangestr,"%I64d-%I64d",downopt->rangestart,downopt->rangeend);
		curlretcode = curl_easy_setopt(curlhandle, CURLOPT_RANGE, rangestr);
	}
	//post flieds end

	struct curl_slist *headers = NULL; /* init to NULL is important */
	/* pass our list of custom made headers */	
	//header start
    if(strstr(url,".ip138.")==0)
    {
	/* we want the headers to this file handle */ 
	{
		for (int headi = 0; headi < downopt->httpheaders.size(); headi++)
		{
			if (downopt->httpheaders[headi].find("Accept-Encoding:")!=std::string::npos)
			{
				std::string accept = downopt->httpheaders[headi].substr(downopt->httpheaders[headi].find("Accept-Encoding:") + strlen("Accept-Encoding:"));
				while (accept[0] == ' ')accept = accept.substr(1);
				curl_easy_setopt(curlhandle, CURLOPT_ACCEPT_ENCODING, accept.c_str());
			}
			else
			{
				headers = curl_slist_append(headers, downopt->httpheaders[headi].c_str());
			}
		}
		if (headers)curl_easy_setopt(curlhandle, CURLOPT_HTTPHEADER, headers);
	}
    }
	//header end

	//headerdata start
	/* we want the headers to this file handle */ 
	{
		if((downopt->NeedDataBit & CUrlDD_NeedDataBit_Header)!=0)//*
		{
			curlretcode=curl_easy_setopt(curlhandle,CURLOPT_WRITEHEADER, &downresult->targetHeader);
		}
	}
	//headerdata end



    if(strstr(url,".ip138.")==0)
    {
    //cookiefile start
    if (!downopt->cookiefile.empty())
    {
        //处理文件cookie
        curl_easy_setopt(curlhandle, CURLOPT_COOKIEFILE, downopt->cookiefile.c_str());
    }
    //cookiefile end

    //cookiefile start
    if (!downopt->cookiestr.empty())
    {
        //处理文件cookie
        curlretcode = curl_easy_setopt(curlhandle, CURLOPT_COOKIELIST, downopt->cookiestr.c_str());
    }
    //cookiefile end

    //cookiejar start
    if (!downopt->cookiejar.empty())
    {
        //处理文件cookie
        curl_easy_setopt(curlhandle, CURLOPT_COOKIEJAR, downopt->cookiejar.c_str());
    }
    //cookiejar end
    }


	//开始下载
	if((downopt->NeedDataBit & CUrlDD_NeedDataBit_Content)!=0)
	{
		if(!downopt->savedir.empty())
		{
			//下载到文件
			if(downopt->writefunc == NULL)
			{
				downopt->writefunc=CUrl_downer_default_file_write_func;
			}

			std::string savefilename;
			if(downopt->savefilename.empty())
			{
				//生成文件名
			}else{
				savefilename=downopt->savefilename;
			}
#ifdef WIN32
			TCHAR FPath[1024]={0};
			int len = MultiByteToWideChar(CP_UTF8,0,savefilename.c_str(),savefilename.length(),FPath,1024);
			FILE *writerf=_wfopen(FPath,L"wb");
#elif defined(__linux__)
			FILE *writerf=fopen(savefilename.c_str(),"wb");
#endif

			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_URL, url);
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, downopt->writefunc);
			if (downopt->writedata!=0)
				curlretcode = curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, downopt->writedata);
			else
				curlretcode = curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, writerf);
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_PROGRESSFUNCTION, downopt->processfunc);
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_OPENSOCKETDATA, NULL);
			curlretcode = curl_easy_perform(curlhandle);
			fflush(writerf);
			fclose(writerf);
			downloadrl = curlretcode;
			if (downloadrl != CURLE_OK)
				goto cleanup;
		}else{
			//下载到Buffer
			if (downopt->writedata != 0 && downopt->writefunc == NULL)
			{
				downopt->writefunc = CUrl_downer_default_file_write_func;
			}
			else if (downopt->writefunc == NULL)
			{
				downopt->writefunc = CUrl_downer_default_buf_write_func;
			}

			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_URL, url);
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, downopt->writefunc);
			if (downopt->writedata != 0)
				curlretcode = curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, downopt->writedata);
			else
				curlretcode = curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, &downresult->targetCoentent);
			if (downopt->processfunc)curlretcode = curl_easy_setopt(curlhandle, CURLOPT_PROGRESSFUNCTION, downopt->processfunc);
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_OPENSOCKETDATA, NULL);
			curlretcode = curl_easy_perform(curlhandle);
			downloadrl = curlretcode;
			if (downloadrl != CURLE_OK)
				goto cleanup;
		}
	}else{
		if((downopt->NeedDataBit & CUrlDD_NeedDataBit_Header)!=0)//*
		{
			//下载到文件
			if (downopt->writefunc == NULL)
			{
				downopt->writefunc = CUrl_downer_default_buf_write_func;
			}

			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_URL, url);
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 1L);
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, downopt->writefunc);
			curlretcode = curl_easy_perform(curlhandle);
			downloadrl = curlretcode;
			if (downloadrl != CURLE_OK)
				goto cleanup;
		}
	}
	

	//get cookies start
	if((downopt->NeedDataBit & CUrlDD_NeedDataBit_Cookie)!=0)
	{
		CURLcode res;
		struct curl_slist *cookies;
		struct curl_slist *nc;

		if(downloadrl!=0)
		{
			//没有执行perform,执行perform
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_URL, url);
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 1L);
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
			curlretcode = curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, downopt->writefunc);
			curlretcode = curl_easy_perform(curlhandle);
			downloadrl = curlretcode;
			if (downloadrl != CURLE_OK)
				goto cleanup;
		}

		curlretcode = curl_easy_getinfo(curlhandle, CURLINFO_COOKIELIST, &cookies);
		if (curlretcode == CURLE_OK)
		{
            //cookie format:
            //#define SEP  "\t"  /* Tab separates the fields */

            //char *my_cookie =
            //  "example.com"    /* Hostname */
            //  SEP "FALSE"      /* Include subdomains */
            //  SEP "/"          /* Path */
            //  SEP "FALSE"      /* Secure */
            //  SEP "0"          /* Expiry in epoch time format. 0 == Session */
            //  SEP "foo"        /* Name */
            //  SEP "bar";       /* Value */
			nc = cookies;
			while(nc)
			{
				downresult->targetCookies.push_back(nc->data);
				nc = nc->next;
			}
			curl_slist_free_all(cookies);
		}
	}
	//get cookies end
	
cleanup:
	if (headers)curl_slist_free_all(headers); /* free the header list */
	curl_easy_cleanup(curlhandle);	

	return downloadrl;
}



size_t CUrl_downer_default_buf_write_func(char *data, size_t size, size_t nmemb, void *writer)
{
	if(data){
		size_t sizes = size * nmemb;
		((std::string*)writer)->append(data, sizes);
		return sizes;
	}
	return 0;
}


int CUrlDirectorDowner(const wchar_t *url, CUrlDirectorDownOption *downopt, CUrlDirectorDownResult *downresult, std::map<std::string, std::string>  *postfieldmap)
{
	char *url2 = new char[1024];
	DWORD url2l=WideCharToMultiByte(CP_ACP, 0, url, wcslen(url), url2, 1024, 0, 0);
	url2[url2l] = 0;
    int rl = CUrlDirectorDowner(url2, downopt, downresult,postfieldmap);
	delete []url2;
	return rl;
}



/*
//example
int main()
{
	CUrlDirectorDownOption       downer_opt;
	CUrlDirectorDownResult       downer_result;
	int							downer_retcode;
	{
		downer_retcode = CUrlDirectorDowner("http://www.baidu.com/", &downer_opt, &downer_result);
	}
}

*/



