#-------------------------------------------------
#
# Project created by QtCreator 2019-02-20T22:51:06
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebsiteOfflineTool
TEMPLATE = app

DEFINES += ZLIB_WINAPI
DEFINES += CURL_STATICLIB
DEFINES += U_STATIC_IMPLEMENTATION


INCLUDEPATH += ..\..\curl-7.75.0\include
INCLUDEPATH += ..\..\zlib-1.2.11\include
INCLUDEPATH += ..\..\icu4c-68_2\include

LIBS +=-L..\..\curl-7.75.0\vcallx64\mt
LIBS += -L..\..\zlib-1.2.11\vcallx64\mt
LIBS += -L..\..\icu4c-68_2\vcallx64\mt

LIBS+=-llibcurl
LIBS+=-lzlibstatic
LIBS+=-lAdvapi32
LIBS+=-lBcrypt
LIBS+=-lCrypt32
LIBS+=-lRpcrt4
LIBS+=-lSecur32
LIBS+=-lUser32
LIBS+=-lshlwapi
LIBS+=-lws2_32
LIBS+=-lWldap32


LIBS+=-lsicudt
LIBS+=-lsicuin
LIBS+=-lsicuio
LIBS+=-lsicutu
LIBS+=-lsicuuc


SOURCES += main.cpp\
    AutoEditTreeView.cpp \
    CurlHttpPostGet.cpp \
    DownTaskThread.cpp \
    RegexGobalMatchAndCombi.cpp \
    RocksDbDownTaskThread.cpp \
    UrlRelinkToLocal.cpp \
    urltofilename.cpp \
    websiteofflinetool.cpp \
    ../../UtilityFunc/MMutex.cpp \
    ../../UtilityFunc/HTML2Text/HtmlToText.cpp \
    ../../UtilityFunc/CreateDirs.cpp \
    ../../UtilityFunc/HTML2Text/HtmlCharMap.cpp \
    ../../UtilityFunc/detecttextcodecname.cpp \
    ../../UtilityFunc/qtudpsend.cpp \
    CUrlDirectDowner.cpp

HEADERS  += AutoEditTreeView.h \
    CUrlDirectDowner.h \
    CurlHttpPostGet.h \
    DownTaskThread.h \
    gzip.h \
    RegexGobalMatchAndCombi.h \
    resource.h \
    RocksDbDownTaskThread.h \
    UrlRelinkToLocal.h \
    urltofilename.h \
    websiteofflinetool.h \
    ../../UtilityFunc/MMutex.h \
    ../../UtilityFunc/HTML2Text/HtmlToText.h \
    ../../UtilityFunc/CreateDirs.h \
    ../../UtilityFunc/HTML2Text/HtmlCharMap.h \
    ../../UtilityFunc/detecttextcodecname.h \
    ../../UtilityFunc/qtudpsend.h \
    ../../UtilityFunc/gzip.h

FORMS    += websiteofflinetool.ui \

RESOURCES += \
    websiteofflinetool.qrc

DISTFILES += \
    WebsiteOfflineTool.rc


TRANSLATIONS = websiteofflinetool_zhcn.ts \
