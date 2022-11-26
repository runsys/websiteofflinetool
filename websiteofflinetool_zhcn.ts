<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="en">
<context>
    <name>WebsiteOfflineToolClass</name>
    <message>
        <location filename="websiteofflinetool.ui" line="14"/>
        <source>WebsiteOfflineTool</source>
        <translation type="unfinished">Website Offline Tool</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="21"/>
        <source>开始抓取选择的网站</source>
        <translation type="unfinished">Start spider website</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="28"/>
        <source>首页是第一个可以定义导入的URL文件,格式每行一个;</source>
        <translation type="unfinished">Home page can be url list file.</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="47"/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;SimSun&apos;; font-size:9pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;操作提示: 多个可以用分号隔开;[regex]开头正则式过滤;网址可以是网址文本路径,格式每行一条;网站中包含[number-number],当序列网址处理;[meta]开头可以正则Content-Type过滤;  URL匹配后缀:*最开始设置后不能修改; 格式:[后缀]URL正则表达式;...   注:8开头的状态是800+curle构成的;   标准PageGate抓取返回数据名:url集合:UrlSet(`分隔);url time:UrlTimeSet;url title:UrlTitleSet;url body:PageBody;save as data:SaveAsData;其他存入页数据;PageGateScript1 __url__ for replacement;URL匹配正则式任务状态值列表匹配时按顺序的;排除下载路径可以定义最大文件可以下载,默认300MB;存储到rockdb：1是存到到rockdb,2是存储到文件，3是存储到packagedata;__cururl__ can express current page url;net address prefix &amp;quot;-&amp;quot; mean don&apos;t init, &amp;quot;--&amp;quot; mean don&apos;t init all;self defined crawl url now() can be express now; 901:content size great than max download limit.;903:meta filter not pass;904:not eixsts pass.;905:page not eixsts pass.;906:page gate&apos;s server list is empty;907:page gate&apos;s return is too big;908:page gate&apos;s return html is empty;909:page gate&apos;s error unknow;911:exclude by file filter disposition-content;912:multi host found url failed;913:head is empty;914:codec not found;915:head size is 0; conent size is not 0;916:conent size less than head size;917:conent size great than head size;918:full content not match;919:compress error;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;8 start urlstate mean:&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_UNSUPPORTED_PROTOCOL,    /* 1 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FAILED_INIT,             /* 2 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_URL_MALFORMAT,           /* 3 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_NOT_BUILT_IN,            /* 4 - [was obsoleted in August 2007 for&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    7.17.0, reused in April 2011 for 7.21.5] */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_COULDNT_RESOLVE_PROXY,   /* 5 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_COULDNT_RESOLVE_HOST,    /* 6 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_COULDNT_CONNECT,         /* 7 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_WEIRD_SERVER_REPLY,  /* 8 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    due to lack of access - when login fails&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    this is not returned. */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_ACCEPT_FAILED,       /* 10 - [was obsoleted in April 2006 for&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    7.15.4, reused in Dec 2011 for 7.24.0]*/&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_WEIRD_PASS_REPLY,    /* 11 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_ACCEPT_TIMEOUT,      /* 12 - timeout occurred accepting server&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    [was obsoleted in August 2007 for 7.17.0,&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    reused in Dec 2011 for 7.24.0]*/&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_WEIRD_PASV_REPLY,    /* 13 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_WEIRD_227_FORMAT,    /* 14 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_CANT_GET_HOST,       /* 15 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_HTTP2,                   /* 16 - A problem in the http2 framing layer.&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    [was obsoleted in August 2007 for 7.17.0,&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    reused in July 2014 for 7.38.0] */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_COULDNT_SET_TYPE,    /* 17 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_PARTIAL_FILE,            /* 18 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_COULDNT_RETR_FILE,   /* 19 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_OBSOLETE20,              /* 20 - NOT USED */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_QUOTE_ERROR,             /* 21 - quote command failure */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_HTTP_RETURNED_ERROR,     /* 22 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_WRITE_ERROR,             /* 23 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_OBSOLETE24,              /* 24 - NOT USED */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_UPLOAD_FAILED,           /* 25 - failed upload &amp;quot;command&amp;quot; */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_READ_ERROR,              /* 26 - couldn&apos;t open/read from file */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_OUT_OF_MEMORY,           /* 27 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  /* Note: CURLE_OUT_OF_MEMORY may sometimes indicate a conversion error&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;           instead of a memory allocation error if CURL_DOES_CONVERSIONS&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;           is defined&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_OBSOLETE29,              /* 29 - NOT USED */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_OBSOLETE32,              /* 32 - NOT USED */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_RANGE_ERROR,             /* 33 - RANGE &amp;quot;command&amp;quot; didn&apos;t work */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_HTTP_POST_ERROR,         /* 34 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_BAD_DOWNLOAD_RESUME,     /* 36 - couldn&apos;t resume download */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FILE_COULDNT_READ_FILE,  /* 37 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_LDAP_CANNOT_BIND,        /* 38 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_LDAP_SEARCH_FAILED,      /* 39 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_OBSOLETE40,              /* 40 - NOT USED */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FUNCTION_NOT_FOUND,      /* 41 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_ABORTED_BY_CALLBACK,     /* 42 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_BAD_FUNCTION_ARGUMENT,   /* 43 */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_OBSOLETE44,              /* 44 - NOT USED */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_OBSOLETE46,              /* 46 - NOT USED */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_TOO_MANY_REDIRECTS ,     /* 47 - catch endless re-direct loops */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_UNKNOWN_OPTION,          /* 48 - User specified an unknown option */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_TELNET_OPTION_SYNTAX ,   /* 49 - Malformed telnet option */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_OBSOLETE50,              /* 50 - NOT USED */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_PEER_FAILED_VERIFICATION, /* 51 - peer&apos;s certificate or fingerprint&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                     wasn&apos;t verified fine */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_GOT_NOTHING,             /* 52 - when this is a specific error */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    default */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SEND_ERROR,              /* 55 - failed sending network data */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_RECV_ERROR,              /* 56 - failure in receiving network data */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_OBSOLETE57,              /* 57 - NOT IN USE */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_CIPHER,              /* 59 - couldn&apos;t use specified cipher */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized/bad encoding */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    that failed */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_LOGIN_DENIED,            /* 67 - user, password or similar was not&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    accepted and we failed to login */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_TFTP_NOTFOUND,           /* 68 - file not found on server */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_TFTP_PERM,               /* 69 - permission problem on server */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_REMOTE_FILE_EXISTS,      /* 73 - File already exists */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_TFTP_NOSUCHUSER,         /* 74 - No such user */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_CONV_FAILED,             /* 75 - conversion failed */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_CONV_REQD,               /* 76 - caller must register conversion&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    callbacks using curl_easy_setopt options&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    CURLOPT_CONV_FROM_NETWORK_FUNCTION,&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    CURLOPT_CONV_TO_NETWORK_FUNCTION, and&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    CURLOPT_CONV_FROM_UTF8_FUNCTION */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    or wrong format */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSH,                     /* 79 - error from the SSH layer, somewhat&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    generic so the error message will be of&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    interest when this has happened */&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;br /&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    connection */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_AGAIN,                   /* 81 - socket is not ready for send/recv,&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    wait till it&apos;s ready and try again (Added&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    in 7.18.2) */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    wrong format (Added in 7.19.0) */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    7.19.0) */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_PRET_FAILED,         /* 84 - a PRET command failed */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_RTSP_CSEQ_ERROR,         /* 85 - mismatch of RTSP CSeq numbers */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_RTSP_SESSION_ERROR,      /* 86 - mismatch of RTSP Session Ids */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_FTP_BAD_FILE_LIST,       /* 87 - unable to parse FTP file list */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_CHUNK_FAILED,            /* 88 - chunk callback reported error */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_NO_CONNECTION_AVAILABLE, /* 89 - No connection available, the&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                    session will be queued */&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;  CURLE_SSL_PINNEDPUBKEYNOTMATCH, /* 90 - specified pinned public key did not&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;                                     match */&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;br /&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="181"/>
        <source>购买</source>
        <translation type="unfinished">Buy</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="188"/>
        <source>注册码</source>
        <translation type="unfinished">Register code</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="195"/>
        <source>注册</source>
        <translation type="unfinished">Register</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="202"/>
        <source>不注册有5分钟试用限制</source>
        <translation type="unfinished">limit 5 minute forno regsiter user</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="219"/>
        <source>&amp;Function</source>
        <translation type="unfinished">&amp;Function</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="247"/>
        <source>Languiage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="258"/>
        <source>lebb.cc home</source>
        <translation type="unfinished">lebb.cc home</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="263"/>
        <source>md5 filename convert to url filename with relink</source>
        <translation type="unfinished">md5 filename convert to url filename with relink</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="268"/>
        <source>md5 filename convert to html title filename</source>
        <translation type="unfinished">md5 filename convert to html title filename</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="273"/>
        <source>delete if no exist url</source>
        <translation type="unfinished">delete if no exist url</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="278"/>
        <source>relink all file to local</source>
        <translation type="unfinished">relink all file to local</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="283"/>
        <source>save data convert to rocksdb</source>
        <translation type="unfinished">save data convert to rocksdb</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="288"/>
        <source>reset no filedata url state</source>
        <translation type="unfinished">reset no filedata url state</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="293"/>
        <source>min file dir to rocksdb</source>
        <translation type="unfinished">min file dir to rocksdb</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="298"/>
        <source>delete min file directory</source>
        <translation type="unfinished">delete min file directory</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="303"/>
        <source>rocksdb convert to file db</source>
        <translation type="unfinished">rocksdb convert to file db</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="308"/>
        <source>Save</source>
        <translation type="unfinished">Save</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="313"/>
        <source>convert to content dispositon file name</source>
        <translation type="unfinished">convert to content dispositon file name</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="318"/>
        <source>download a test url to test.html</source>
        <translation type="unfinished">download a test url to test.html</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="323"/>
        <source>update urlstate by find content</source>
        <translation type="unfinished">update urlstate by find content</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="328"/>
        <source>file size zero update urlstate to 0(no download)</source>
        <translation type="unfinished">file size zero update urlstate to 0(no download)</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="333"/>
        <source>urlstate 6(size not equal) delete file and set urlstate to 0</source>
        <translation type="unfinished">urlstate 6(size not equal) delete file and set urlstate to 0</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="338"/>
        <source>export_title_by_url_regex</source>
        <translation type="unfinished">export_title_by_url_regex</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="343"/>
        <source>name list to utf8 url list</source>
        <translation type="unfinished">name list to utf8 url list</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="348"/>
        <source>addtime from html regex</source>
        <translation type="unfinished">addtime from html regex</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="353"/>
        <source>md5 filename to url file name without relink</source>
        <translation type="unfinished">md5 filename to url file name without relink</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="358"/>
        <source>url filename to url md5 filename</source>
        <translation type="unfinished">url filename to url md5 filename</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="363"/>
        <source>Chinese</source>
        <translation type="unfinished">Chinese</translation>
    </message>
    <message>
        <location filename="websiteofflinetool.ui" line="368"/>
        <source>English</source>
        <translation type="unfinished">English</translation>
    </message>
</context>
</TS>
