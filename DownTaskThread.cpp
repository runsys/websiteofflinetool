#include "DownTaskThread.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtEndian>
#include <QFileInfo>
#include <qapplication.h>
#include <QFile>
#include <qvariant.h>
#include <deque>
#include <QSet>
#include <set>
#include <QDir>
#include <QFile>
#include <qendian.h>
#include "shlwapi.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QMutex>
#include "..\..\UtilityFunc\detecttextcodecname.h"
#include <QTextCodec>
#include "UrlRelinkToLocal.h"
#include <QCryptographicHash>
#include <QRegularExpression>
#include "..\..\UtilityFunc\MMutex.h"
#include "gzip.h"
#include "urltofilename.h"
#include <cstdint>
#include <QDateTime>
#include "RegexGobalMatchAndCombi.h"
#include "..\..\UtilityFunc\qtudpsend.h"
#include "..\..\UtilityFunc\range_rand.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <ctime>
#include "CUrlDirectDowner.h"
//#include "snappy/snappy.h"
#include "..\..\UtilityFunc\HTML2Text\HtmlToText.h"
#include "..\..\UtilityFunc\qtoolfunc.h"
#include "..\..\UtilityFunc\gzip.h"

//901:content size great than max download limit.
//903:meta filter not pass
//904:not eixsts pass.
//905:page not eixsts pass.
//906:page gate's server list is empty
//907:page gate's return is too big
//908:page gate's return html is empty
//909:page gate's error unknow
//911:exclude by file filter disposition-content
//912:multi host found url failed
//913:head is empty
//914:codec not found
//915:head size is 0; conent size is not 0
//916:conent size less than head size
//917:conent size great than head size
//918:full content not match
//919:compress error

using namespace std;

int CurlHttpGet(std::string url, std::map<std::string, std::string>  *postfieldmap, CUrlDirectorDownResult *downer_result, int withheader = 0, int timeout = 30, std::string useragent = "", std::string jarfile = "", unsigned int NeedDataBit = CUrlDD_NeedDataBit_Content | CUrlDD_NeedDataBit_Header | CUrlDD_NeedDataBit_Cookie, long long int rangestart = 0, long long int rangeend = -1, CUrl_downer_write_func_type writefunc = 0, void *writedata = 0);
extern int CurlHttpGet(std::string url, std::map<std::string, std::string>  *postfieldmap, CUrlDirectorDownResult *downer_result, int withheader, int timeout , std::string useragent , std::string jarfile , unsigned int NeedDataBit, long long int rangestart, long long int rangeend , CUrl_downer_write_func_type writefunc , void *writedata);

QMap<QString, int> multihost_hitcnt;
QMutex multihost_hitcntmutex;
QMutex datapackagefm;
QFile *datapackagef=0;

size_t DownTaskThread_write_func(char *data, size_t size, size_t nmemb, void *writer)
{
	if (data){
		size_t sizes = size * nmemb; 
		HANDLE *hand = (HANDLE*)writer;
		DWORD wcnt = 0;
		WriteFile(hand, data, sizes, &wcnt, 0);
		return sizes;
	}
	return 0;
}


bool LsstModifiedTimeQtDateTime(QString timestr,QDateTime &dt)
{
	timestr = timestr.trimmed();
	//Sat, 23 Sep 2017 02:56:47 GMT
	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QRegularExpression utcre("([a-zA-Z]+), (\\d+) ([a-zA-Z]+) (\\d+) (\\d+):(\\d+):(\\d+) GMT", reflag);
	auto ma = utcre.match(timestr);
	if (ma.hasMatch())
	{
		QString day = ma.captured(2);
		QString mon = ma.captured(3);
		QString year = ma.captured(4);
		QString hour = ma.captured(5);
		QString minute = ma.captured(6);
		QString sec = ma.captured(7);
		int moni = -1;
		if (mon == "Jan")
		{
			moni = 1;
		}if (mon == "Feb")
		{
			moni = 2;
		}if (mon == "Mar")
		{
			moni = 3;
		}if (mon == "Apr")
		{
			moni = 4;
		}if (mon == "May")
		{
			moni = 5;
		}if (mon == "June")
		{
			moni = 6;
		}if (mon == "July")
		{
			moni = 7;
		}if (mon == "Aug")
		{
			moni = 8;
		}if (mon == "Sep")
		{
			moni = 9;
		}if (mon == "Oct")
		{
			moni = 10;
		}if (mon == "Nov")
		{
			moni = 11;
		}if (mon == "Dec")
		{
			moni = 12;
		}
		if (moni >= 1 && moni <= 12 && day.toInt() >= 1 && day.toInt() <= 31 && hour.toInt() >= 0 && hour.toInt() <= 24 && minute.toInt() >= 0 && minute.toInt() <= 59 && sec.toInt() >= 0 && sec.toInt() <= 59)
		{
			dt = QDateTime(QDate(year.toInt(), moni, day.toInt()), QTime(hour.toInt(), minute.toInt(), sec.toInt()));
			return true;
		}
	}
	return false;
}




time_t g_linkrf_update_time;
DownTaskThread *linkupthrd = 0;
QMutex g_nexttask_time_lock;
uint64_t g_nexttask_time=0;
QMutex g_rockfmutex;
extern bool g_exit;
/*
rocksdb::DB  *rocksdbopen(QString dbpath)
{
	auto indexname2 = dbpath.toUtf8();
	std::string indexnamepath;
	indexnamepath = indexname2.data();
	while (indexnamepath.find("\\\\") != -1)indexnamepath.replace(indexnamepath.find("\\\\"), 2, "\\");
	
	rocksdb::DB      *dbpathhand = NULL;
	rocksdb::Options  options1;
	options1.create_if_missing = true;
	options1.write_buffer_size = 64 * 1024 * 1024;
	options1.max_open_files = 10000;


	wchar_t wpath1[1024] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, indexnamepath.data(), indexnamepath.size(), wpath1, 1024);
	char cpath1[1024] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, wpath1, wcslen(wpath1), cpath1, 1024, 0, 0);

	// open
	auto openstatus1 = rocksdb::DB::Open(options1, cpath1, &dbpathhand);


	if (openstatus1.ok())
	{
		return dbpathhand;
	}
	return 0;
}
*/

time_t g_dbcommitpre=0;
QFile  *g_rockf = 0;
time_t g_rockflastflush=0;
QFile  *rockfopen(QString dbpath){
    QFile *ff=new QFile(dbpath);
    ff->open(QIODevice::ReadWrite);
    ff->seek(ff->size());
    return ff;
}

QSet<QString> allsite;
QMutex allsitem;
QFile *allsitef=nullptr;

extern int g_threadnum;
DownTaskThread::DownTaskThread(QString savepath, QString inc_ls, QString exc_ls, QString waitsec, QString downsec, QString linkrfsec, QString mainruls, QString rfurlmatches, QString taskintvalmsec, QString useragent, QString jarfile, QString url_ext_relist, QString relink_html_to_local, QString fileexistdonotdown_match, QString savetorocksdb, QString noexistmatch, QString pagenoexistmatch, QString pagetimeoutsec, QString urlfindre, QString combiurlrepre, QString combitimerepre, QString combititlerepre, QString newurlstate, QString urlfindre2, QString combiurlrepre2, QString combitimerepre2, QString combititlerepre2, QString newurlstate2, QString PageGateSrvList, QString PageGateCrawlUrlMatch1, QString PageGateScript1, QString PageGateCrawlUrlMatch2, QString PageGateScript2, QString ExclCtDispList, QString PageFullUrlRestr1, QString PageCttFullReStr1, QString PageFullUrlRestr2, QString PageCttFullReStr2, QString MultiHostUrlReStr, QString MultiHostRepExpr, QString MultiHostNotUrlHeadMatch, QString largefiledowntype, QString reurlstatelist, QString urlfindreoptset, QString autofindlinktype, QString replacetestlsstr, QString memdownmaxsize)
{
    this->mStop=false;
    this->iwaitsec = 0, this->idownsec = 0, this->linkrfsec = 0, this->taskintvalmsec = 0;
	this->savepath = savepath;
	this->inc_ls = inc_ls;
	this->exc_ls = exc_ls;
	this->waitsec = waitsec;
	this->downsec = downsec;
	this->linkrfsec = linkrfsec.toInt();
	this->mainruls = mainruls;
	this->rfurlmatches = rfurlmatches;
	this->taskintvalmsec = taskintvalmsec.toInt();
	this->useragent = useragent;
	this->jarfile = jarfile;
	this->url_ext_relist = url_ext_relist;
	this->relink_html_to_local = relink_html_to_local;
	this->fileexistdonotdown_match = fileexistdonotdown_match;
	this->savetorocksdb = savetorocksdb;
	this->noexistmatch = noexistmatch;
	this->pagenoexistmatch = pagenoexistmatch;
	if (pagetimeoutsec == "" || pagetimeoutsec.toInt() < 1)pagetimeoutsec = "15";
	this->pagetimeoutsec = pagetimeoutsec.toInt();

	this->urlfindre = urlfindre;
	this->combiurlrepre = combiurlrepre;
	this->combitimerepre = combitimerepre;
	this->combititlerepre = combititlerepre;
	this->newurlstate = newurlstate;


	//std::string idstr;
	//for (int i = 0; i < 10000; i++)
	//{
	//	int id = lb_uniform_int(-(2 << 31 - 1), 1 << 30);
	//	idstr.append((char*)&id, sizeof(id));
	//}
	//std::string output;
	//snappy::Compress(idstr.data(), idstr.size(), &output);

	//QFile ff("rand40kbids.data");
	//ff.open(QIODevice::WriteOnly);
	//ff.write(output.data(), output.size());
	//ff.close();


	this->urlfindre2 = urlfindre2;
	this->combiurlrepre2 = combiurlrepre2;
	this->combitimerepre2 = combitimerepre2;
	this->combititlerepre2 = combititlerepre2;
	this->newurlstate2 = newurlstate2;

	this->PageGateSrvList = PageGateSrvList;
	this->PageGateCrawlUrlMatch1 = PageGateCrawlUrlMatch1;
	this->PageGateScript1 = PageGateScript1;
	this->PageGateCrawlUrlMatch2 = PageGateCrawlUrlMatch2;
	this->PageGateScript2 = PageGateScript2;

	this->ExclCtDispList = ExclCtDispList;


	this->PageFullUrlRestr1 = PageFullUrlRestr1;
	this->PageCttFullReStr1 = PageCttFullReStr1;
	this->PageFullUrlRestr2 = PageFullUrlRestr2;
	this->PageCttFullReStr2 = PageCttFullReStr2;

	this->MultiHostUrlReStr = MultiHostUrlReStr;
	this->MultiHostRepExpr = MultiHostRepExpr;
	this->MultiHostNotUrlHeadMatch = MultiHostNotUrlHeadMatch;

	this->memdownmaxsize = memdownmaxsize;
	if (memdownmaxsize.toLongLong() < 1 * 1024 * 1024)
	{
		this->memdownmaxsize = QString("%1").arg(50 * 1024 * 1024);
	}

	if (largefiledowntype == "")largefiledowntype = "2";
	this->largefiledowntype = largefiledowntype;
	
	this->reurlstatelist = reurlstatelist;
	this->urlfindreoptset = urlfindreoptset;
	if (autofindlinktype == "")
		this->autofindlinktype = 1;
	else
		this->autofindlinktype = autofindlinktype.toInt();
    if (!(autofindlinktype < 0 || this->autofindlinktype > 5))
	{
		this->autofindlinktype = 1;
	}
	this->replacetestlsstr = replacetestlsstr;

	if (linkupthrd == 0)
	{
		linkupthrd = this;
	}
	if (g_nexttask_time == 0)
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		g_nexttask_time = (uint64_t)time(0) * 1000 + sys.wMilliseconds + this->taskintvalmsec;
	}
	if (savetorocksdb == "1")
	{
        g_rockfmutex.lock();
        if(g_rockf == nullptr)
            g_rockf = rockfopen(savepath+"urldata_rocksdb");
        g_rockfmutex.unlock();

        QFile lszf(savepath + "multihoststat.txt");
		lszf.open(QIODevice::ReadOnly);
		while (!lszf.atEnd())
		{
			QString linetext = lszf.readLine();
			linetext = linetext.trimmed();
			QStringList linels = linetext.split("\t");
			if (linels.size() >= 2)
				multihost_hitcnt.insert(linels[0], linels[1].toInt());
		}
		lszf.close();
	}
	else if (savetorocksdb == "3"){
		QDir dir(savepath);
		QStringList flt;
		flt << "_segmentdata.*";
		auto allsegmentdatafs=dir.entryInfoList(flt, QDir::Files);
		if (allsegmentdatafs.size() == 0)
		{
			if (datapackagef == nullptr){
                datapackagef = new QFile(savepath + "_segmentdata.0");
				datapackagef->open(QIODevice::WriteOnly);
			}
			segmentdataid = "0";
		}
		else{
			auto maxsegid = 0;
			for (auto it = allsegmentdatafs.begin(); it != allsegmentdatafs.end(); it++){
				auto segid = it->fileName().mid(it->fileName().lastIndexOf(".") + 1).toInt();
				if (segid > maxsegid){
					maxsegid = segid;
				}
			}
			if (datapackagef == nullptr){
                datapackagef = new QFile(savepath + "_segmentdata." + QString("%1").arg(maxsegid));
				datapackagef->open(QIODevice::ReadWrite);
				datapackagef->seek(datapackagef->size());
			}
			segmentdataid = QString("%1").arg(maxsegid);
		}
        g_rockf = 0;
	}else{
        g_rockf = 0;
	}


    allsitem.lock();
    if(allsitef==nullptr){
        allsitef=new QFile(savepath+"collectsite.txt");
        allsitef->open(QIODevice::ReadWrite);
        char *databuf=new char[20*1024*1024+1];
        auto readlen=allsitef->read(databuf,20*1024*1024);
        databuf[readlen]=0;
        QString str(databuf);
        delete[]databuf;
        auto sitels=str.split("\n");
        for(int i=0;i<sitels.size()-1;i++){
            allsite.insert(sitels[i]);
        }
        allsitef->seek(allsitef->size());
    }
    allsitem.unlock();
}

DownTaskThread::~DownTaskThread()
{

}

MMutex gettaskmutex;



void Deflate(Byte *pOutput, uLong pOutputl, Byte* pInput, uLong pInputl)
{
	z_stream zstm;
	memset(&zstm, 0, sizeof(z_stream));

	
	deflateInit(&zstm, Z_DEFAULT_COMPRESSION);

	int err = Z_OK;
	auto ppInput = pInput, ppOutput = pOutput;
	while (TRUE)
	{
		if (ppInput-pInput == pInputl)
			break;

		zstm.next_in = (Bytef*)ppInput;
		zstm.avail_in = (uInt)pInputl;

		while (TRUE)
		{
			zstm.next_out = (Bytef*)ppOutput;
			zstm.avail_out = pOutputl - (ppOutput - pOutput);

			err = deflate(&zstm, Z_SYNC_FLUSH);
			if (err != Z_OK)
				break;

			UINT cbWrite = pOutputl - (ppOutput - pOutput) - zstm.avail_out;
			if (cbWrite == 0)
				break;
			ppOutput += cbWrite;
			ppInput += pInputl;

			if (zstm.avail_out != 0)
				break;
		}
	}

	err = deflateEnd(&zstm);
}

/*
int http_ungzip(char *source, int len, char **dest, int *destlen, bool bgzip)
{
#define CHUNK 409600
	int ret;
	unsigned have;
	z_stream strm;
	unsigned char out[CHUNK];
	int totalsize = 0;

	// allocate inflate state 
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

	// run inflate() on input until output buffer not full 
	do {
		strm.avail_out = CHUNK;
		strm.next_out = out;
		ret = inflate(&strm, Z_NO_FLUSH);
		//assert(ret != Z_STREAM_ERROR);  state not clobbered 
		switch (ret) {
		case Z_NEED_DICT:
			ret = Z_DATA_ERROR; // and fall through 
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
	// clean up and return 
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}
*/


int g_waitsec, g_downsec;

bool g_rockfopenfail = false;


void DownTaskThread::run()
{
	Sleep(1000);
    if (savetorocksdb=="1" && g_rockf == nullptr)
	{
        g_rockfopenfail = true;
		return;
	}
	iwaitsec = waitsec.toInt();
	idownsec = downsec.toInt();
	int seg_sec = iwaitsec + idownsec;
	QSqlDatabase db = QSqlDatabase::database("taskdb");
	if (!db.isOpen())return;
	QSqlQuery query4(db);

	QStringList incls = inc_ls.split(";"),metaincls;
	QStringList excls = exc_ls.split(";"),metaexcls;
	incls.removeDuplicates();
	excls.removeDuplicates();
	for (int i = incls.size() - 1; i >= 0; i--)
	{
		incls[i]=incls[i].trimmed();
		if (incls[i] == "")
		{
			incls.removeAt(i);
		}
		else if (incls[i].startsWith("[meta]"))
		{
			metaincls.append(incls[i]);
			incls.removeAt(i);
		}
	}
	int64_t maxdownsize = 300 * 1024 * 1024;
	QRegularExpression mare("^(\\d+)byte$");
	for (int i = excls.size() - 1; i >= 0; i--)
	{
		auto ma = mare.match(excls[i]);
		if (ma.hasMatch())
		{
			maxdownsize = ma.captured(1).toLongLong();
			excls.removeAt(i);
			continue;
		}
		excls[i] = excls[i].trimmed();
		if (excls[i] == "")
		{
			excls.removeAt(i);
		}
		else if (excls[i].startsWith("[meta]"))
		{
			metaexcls.append(excls[i]);
			excls.removeAt(i);
		}
	}

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;

	QList<QRegularExpression> incre, excre, metaincre, metaexcre;
	for (int i = incls.size() - 1; i >= 0; i--)
	{
		if (incls[i].startsWith("[regex]"))
		{
			incls[i] = incls[i].mid(strlen("[regex]"));
			incre.push_back(QRegularExpression(incls[i], reflag));
		}
		else
		{
			incre.push_back(QRegularExpression("(", reflag));
		}
	}
	for (int i = excls.size() - 1; i >= 0; i--)
	{
		if (excls[i].startsWith("[regex]"))
		{
			excls[i] = excls[i].mid(strlen("[regex]"));
			excre.push_back(QRegularExpression(excls[i], reflag));
		}
		else{
			excre.push_back(QRegularExpression("(", reflag));
		}
	}

	for (int i = metaincls.size() - 1; i >= 0; i--)
	{
		metaincls[i] = metaincls[i].mid(strlen("[meta]"));
		metaincre.push_back(QRegularExpression(metaincls[i], reflag));
	}
	for (int i = metaexcls.size() - 1; i >= 0; i--)
	{
		metaexcls[i] = metaexcls[i].mid(strlen("[meta]"));
		metaexcre.push_back(QRegularExpression(metaexcls[i], reflag));
	}


	QMap<QString,QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]")+1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext,QRegularExpression(restr, reflag));
		}
	}




	QStringList fexistmastrls = fileexistdonotdown_match.split(";");
	fexistmastrls.removeDuplicates();
	for (int i = fexistmastrls.size() - 1; i >= 0; i--)
	{
		fexistmastrls[i] = fexistmastrls[i].trimmed();
		if (fexistmastrls[i] == "")
		{
			fexistmastrls.removeAt(i);
		}
	}
	QList<QRegularExpression>  fexistmastrrels;
	for (int i = fexistmastrls.size() - 1; i >= 0; i--)
	{
		if (fexistmastrls[i].startsWith("[regex]"))
		{
			fexistmastrls[i] = fexistmastrls[i].mid(strlen("[regex]"));
			fexistmastrrels.push_back(QRegularExpression(fexistmastrls[i], reflag));
		}
		else
		{
			fexistmastrrels.push_back(QRegularExpression("(", reflag));
		}
	}
	

	QStringList noexistmatchls = noexistmatch.split(";"); noexistmatchls.removeAll("");
	QList<QRegularExpression>  noexistmatchrels;
	for (int i = noexistmatchls.size() - 1; i >= 0; i--)
	{
		if (noexistmatchls[i].startsWith("[regex]"))
		{
			noexistmatchls[i] = noexistmatchls[i].mid(strlen("[regex]"));
			noexistmatchrels.push_back(QRegularExpression(noexistmatchls[i], reflag));
		}
		else
		{
			noexistmatchrels.push_back(QRegularExpression("(", reflag));
		}
	}


	QStringList pagenoexistmatchls = pagenoexistmatch.split(";"); pagenoexistmatchls.removeAll("");
	QList<QRegularExpression>  pagenoexistmatchrels;
	for (int i = pagenoexistmatchls.size() - 1; i >= 0; i--)
	{
		if (pagenoexistmatchls[i].startsWith("[regex]"))
		{
			pagenoexistmatchls[i] = pagenoexistmatchls[i].mid(strlen("[regex]"));
			pagenoexistmatchrels.push_back(QRegularExpression(pagenoexistmatchls[i], reflag));
		}
		else
		{
			pagenoexistmatchrels.push_back(QRegularExpression("(", reflag));
		}
	}

	QStringList PageGateSrvLs = PageGateSrvList.split(";");
	PageGateSrvLs.removeAll("");


	bool bhavecontent_disposition = false;
	gettaskmutex.Lock();
	QString query4sql = QString() + "select id,url,content_disposition from  taskurls where urlstate=0 limit 0,1";
	bool rl = query4.exec(query4sql);
	if (rl)
	{
		while (query4.next())
		{
			bhavecontent_disposition = true;
		}
	}
	gettaskmutex.Unlock();


	QStringList MultiHostRepExprLs = MultiHostRepExpr.split(";");
	if (MultiHostRepExprLs.size() > 0)
	{
		if (MultiHostRepExprLs[0].startsWith("all:"))
			MultiHostRepExprLs[0] = MultiHostRepExprLs[0].mid(4);
	}
	MultiHostRepExprLs.removeAll("");


	QStringList reurlstatels=reurlstatelist.split(";");
	QList<QRegularExpression> ustaterels;
	QStringList ustatevalls;
	for (int i = 0; i < reurlstatels.size(); i++)
	{
		reurlstatels[i] = reurlstatels[i].trimmed();
		if (reurlstatels[i] != "")
		{
			QStringList reval = reurlstatels[i].split("`");
			ustaterels.push_back(QRegularExpression(reval[0], reflag));
			ustatevalls.push_back(reval[1]);
		}
	}

	QStringList urlfindreoptls = urlfindreoptset.split(";");
	QStringList urlfindrels, combiurlreprels, combitimereprels, combititlereprels, newurlstatels;

	if (urlfindre != "")
	{
		urlfindrels.push_back(urlfindre);
		combiurlreprels.push_back(combiurlrepre);
		combitimereprels.push_back(combitimerepre);
		combititlereprels.push_back(combititlerepre);
		newurlstatels.push_back(newurlstate);
	}
	if (urlfindre2 != "")
	{
		urlfindrels.push_back(urlfindre2);
		combiurlreprels.push_back(combiurlrepre2);
		combitimereprels.push_back(combitimerepre2);
		combititlereprels.push_back(combititlerepre2);
		newurlstatels.push_back(newurlstate2);
	}


	for (int i = 0; i < urlfindreoptls.size(); i++)
	{
		urlfindreoptls[i] = urlfindreoptls[i].trimmed();
		if (urlfindreoptls[i] != "")
		{
			QStringList optls = urlfindreoptls[i].split("`");
			if (optls.size() >= 5)
			{
				urlfindrels.push_back(optls[0]);
				combiurlreprels.push_back(optls[1]);
				combitimereprels.push_back(optls[2]);
				combititlereprels.push_back(optls[3]);
				newurlstatels.push_back(optls[4]);
			}
		}
	}


	
	QStringList replacetestls2;
	QStringList replacetestls=replacetestlsstr.split(";");
	for (int i = 0; i < replacetestls.size(); i++)
	{
		replacetestls[i]=replacetestls[i].replace("\\x{3b}", ";");

		replacetestls2.append(replacetestls[i]);
	}

	

	int trycnt = 0;
	char *destbuf = new char[8 * 1024 * 1024];
	time_t lasttime = time(nullptr);
    g_dbcommitpre=time(nullptr);
    auto bprecommite=false;
    db.transaction();
	while (true)
	{
        if(datapackagef!=nullptr){
            datapackagefm.lock();
            if (time(nullptr) - lasttime>50){
                datapackagef->flush();
                lasttime = time(nullptr);
            }
            datapackagefm.unlock();
        }

        if(g_rockf!=nullptr && time(nullptr) - g_rockflastflush>60){
            g_rockfmutex.lock();
            if (time(nullptr) - g_rockflastflush>50){
                g_rockf->flush();
                auto fsize=g_rockf->size();
                QFile lastftf(savepath+"/urldata_rocksdb.lastflushpos");
                QFile::remove(savepath+"/urldata_rocksdb.lastflushposbak");
                QFile::rename(savepath+"/urldata_rocksdb.lastflushpos",savepath+"/urldata_rocksdb.lastflushposbak");
                lastftf.open(QIODevice::WriteOnly);
                lastftf.write(u("%1").arg(fsize).toUtf8());
                lastftf.close();
                g_rockflastflush=time(nullptr);
                allsitef->flush();
            }
            g_rockfmutex.unlock();
        }



		if (g_exit == true)
		{
            gettaskmutex.Lock();
            g_threadnum-=1;
            if(g_threadnum==0){
                db.commit();
                if(g_rockf!=nullptr){
                    g_rockf->flush();
                    g_rockf->close();
                }
                if(datapackagef!=nullptr){
                    datapackagef->flush();
                    datapackagef->close();
                }
                allsitef->flush();
                allsitef->close();
            }
            gettaskmutex.Unlock();
			break;
		}
		QString urlid, url, content_disposition,addtime;
		QDateTime addtt;
		bool bgeturl = false;
		gettaskmutex.Lock();
        if(time(nullptr)-g_dbcommitpre>60){
            db.commit();
            db.transaction();
            g_dbcommitpre=time(nullptr);
        }

        //db.transaction();
		QString query4sql;
		if (bhavecontent_disposition == false)
		{
			query4sql = QString() + "select id,url,addtime from  taskurls where urlstate=0 limit 0,1";
		}
		else{
			query4sql = QString() + "select id,url,content_disposition,addtime from  taskurls where urlstate=0 limit 0,1";
		}
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			while (query4.next())
			{
				urlid = query4.value("id").toString();
				url = query4.value("url").toString();
				if (bhavecontent_disposition == true)
					content_disposition = query4.value("content_disposition").toString().trimmed();
				addtt = query4.value("addtime").toDateTime();
				addtime = addtt.toString("yyyy-MM-dd HH:mm:ss");
				query4.finish(); 
				query4.clear();
				

				QString query4sql;
				if (url.size() != url.trimmed().size())
				{
					url = url.trimmed();
					query4sql = QString() + "update taskurls set url='" + url + "',urlstate=1 where id=" + urlid + "";
				}
				else{
					query4sql = QString() + "update taskurls set urlstate=1 where id=" + urlid + "";
				}
				rl = query4.exec(query4sql);

				bgeturl = true;
			}
		}
        //db.commit();
		gettaskmutex.Unlock();
		if (!bgeturl)
		{
			Sleep(1000);
			trycnt++;
			if (trycnt > 30)
			{
				break;
			}
			else{
				continue;
			}
		}
		else
		{
			QString urlfilename = urltofilename(url, &urlre_ext);
			if (fexistmastrrels.size()>0)
			{
				bool bfeurlma = false;
				for (int ij = 0; ij < fexistmastrrels.size(); ij++)
				{
					if (fexistmastrrels[ij].isValid())
					{
						if (fexistmastrrels[ij].match(url).hasMatch())
						{
							bfeurlma = true;
						}
					}
					else if (url.indexOf(fexistmastrls[ij])!=-1)
					{
						bfeurlma = true;
					}
					if (bfeurlma)break;
				}
				if (bfeurlma)
				{
					bool bcedo = false;
                    if (QFile::exists(savepath + urlfilename))
					{
						if (savetorocksdb == "1")
						{
							gettaskmutex.Lock();
							QString query4sql = QString() + "update taskurls set urlstate=2 where id=" + urlid + "";
							rl = query4.exec(query4sql);
							gettaskmutex.Unlock();

							QString filename = urltofilename(url, &urlre_ext);
                            QString fpa = savepath  + filename;
							QFile ff(fpa);
							if (ff.open(QIODevice::ReadOnly))
							{
								auto ffctt = ff.readAll();
								ff.close();

                                auto buf=new char[ffctt.size()+4096];
                                uLong buflen=ffctt.size()+4096;
                                gzcompress((Bytef*)ffctt.data(),ffctt.size(),(Bytef*)buf,&buflen);

                                g_rockfmutex.lock();
                                int32_t keylen;
                                int32_t vallen;
                                keylen=url.toUtf8().size();
                                vallen=buflen;
                                char keylenbt[4];
                                qToBigEndian(keylen,(uchar*)keylenbt);
                                char vallenbt[4];
                                qToBigEndian(vallen,(uchar*)vallenbt);
                                g_rockf->write(keylenbt,4);
                                g_rockf->write(url.toUtf8());
                                g_rockf->write(vallenbt,4);
                                g_rockf->write(buf,buflen);
                                g_rockfmutex.unlock();
                                bcedo = true;
                                delete []buf;
							}
						}
						else if (savetorocksdb == "3"){


							QString filename = urltofilename(url, &urlre_ext);
                            QString fpa = savepath + filename;
							QFile ff(fpa);
							if (ff.open(QIODevice::ReadOnly))
							{
								auto ffctt = ff.readAll();
								ff.close();
								uLong destbuflen = 8 * 1024 * 1024-4;
								auto srcd = (url + "\n" + ffctt).toUtf8();
								int rl = compress2((Bytef*)destbuf, &destbuflen, (Bytef*)srcd.data(), srcd.size(), 9);
								if (rl == Z_OK){
									datapackagefm.lock();
									auto datafpos = datapackagef->pos();
									uint32_t destbflen = (uint32_t)destbuflen;
									qToBigEndian<qint32>((qint32)destbflen, (uchar*)destbuf + 8 * 1024 * 1024 - 4);
									datapackagef->write(destbuf+8*1024*1024-4, 4);
									datapackagef->write(destbuf, destbuflen);
									datapackagefm.unlock();
									gettaskmutex.Lock();
									QString query4sql = QString() + "update taskurls set urlstate=2,dataindex=" + segmentdataid + ",dataoffset=" + QString("%1").arg(datafpos) + " where id=" + urlid + "";
									rl = query4.exec(query4sql);
									gettaskmutex.Unlock();

									bcedo = true;
								}
								else{
									gettaskmutex.Lock();
									QString query4sql = QString() + "update taskurls set urlstate=0 where id=" + urlid + "";
									rl = query4.exec(query4sql);
									gettaskmutex.Unlock();
								}
							}
						}else{
							gettaskmutex.Lock();
							QString query4sql = QString() + "update taskurls set urlstate=2 where id=" + urlid + "";
							rl = query4.exec(query4sql);
							gettaskmutex.Unlock();

							bcedo = true;
						}
					}
					if (bcedo)continue;
				}
			}

			if (linkrfsec != 0 && g_linkrf_update_time<time(0) && linkupthrd==this)
			{
                //db.transaction();
				QStringList mainurlls=mainruls.split(";");
				for (int i = 0; i < mainurlls.size(); i++)
				{
					QString query4sql = QString() + "update taskurls set urlstate=0 where url='" + mainurlls[i] + "'";
					rl = query4.exec(query4sql);
				}

				QStringList rfurlmatchesls = rfurlmatches.split(";");
				QStringList regexls;
				for (int i = 0; i < rfurlmatchesls.size(); i++)
				{
					if (rfurlmatchesls[i].startsWith("[regex]"))
					{
						regexls.push_back(rfurlmatchesls[i].mid(strlen("[regex]")));
					}
					else{
						QString query4sql = QString() + "update taskurls set urlstate=0 where url like '%" + rfurlmatchesls[i] + "%'";
						rl = query4.exec(query4sql);
					}
				}
				if (regexls.size() > 0)
				{
					QList<QRegularExpression> rels;
					for (int i = 0; i < regexls.size(); i++)
					{
						rels.push_back(QRegularExpression(regexls[i], reflag));
					}
					QString query4sql = QString() + "select id,url from  taskurls where urlstate=0 limit 0,1";
					bool rl = query4.exec(query4sql);
					if (rl)
					{
						QStringList u0idls;
						while (query4.next())
						{
							urlid = query4.value("id").toString();
							url = query4.value("url").toString();
							for (int j = 0; j < rels.size(); j++)
							{
								if (rels[j].match(url).hasMatch())
								{
									u0idls.push_back(urlid);
								}
							}
						}
						for (int i = 0; i < u0idls.size(); i++)
						{
							QString query4sql;
							query4sql = QString() + "update taskurls set urlstate=0 where id=" + urlid + "";
							rl = query4.exec(query4sql);
						}
					}
				}
                //db.commit();
				g_linkrf_update_time = time(0) + linkrfsec;
			}


			if (seg_sec != 0)
			{
				while (true)
				{
					auto segsecmod = time(0) % seg_sec;
					if (segsecmod >= iwaitsec)
					{
						break;
					}
					else if (segsecmod < iwaitsec)
					{
						msleep(50);
					}
				}
			}

			if (taskintvalmsec != 0)
			{
				while (true)
				{
					bool breachtime = false;
					g_nexttask_time_lock.lock();
					SYSTEMTIME sys;
					GetLocalTime(&sys);
					if ((uint64_t)time(0) * 1000 + sys.wMilliseconds>g_nexttask_time)
					{
						breachtime = true;
						g_nexttask_time = (uint64_t)time(0) * 1000 + sys.wMilliseconds + taskintvalmsec;
					}
					g_nexttask_time_lock.unlock();
					if (breachtime == true)
					{
						break;
					}
				}
			}


			if (ExclCtDispList != "" && content_disposition != "")
			{
				QString content_disposition2;
				if (content_disposition.lastIndexOf(".") != -1)
					content_disposition2 = content_disposition.mid(0, content_disposition.indexOf("."));
				else
					content_disposition2 = content_disposition;
				if (ExclCtDispList.indexOf(content_disposition2) != -1)
				{
					gettaskmutex.Lock();
					QString query4sql = QString() + "update taskurls set urlstate=911 where id=" + urlid + "";
					rl = query4.exec(query4sql);
					gettaskmutex.Unlock();
					continue;
				}
			}


			//download url and relink to local
			auto u8url = url.toUtf8();
			CUrlDirectorDownResult       downer_result;
			int							downer_retcode;
			QString contentlength="";
			QString ctttype2 = "";
			QString LastModified;
			QString headstr2;
			std::map<std::string, std::string> postdata;

			int urlrepexpi = 0;
			QString multihostnewurl;
			QStringList MultiHostRepExprLs2;
			bool bMultiHostRepExprLs2 = false, bmultihosturlok=false;
			QString oldurl = url;
			while (true)
			{
                downer_retcode = CurlHttpGet(std::string(u8url.data()), &postdata, &downer_result, 0, pagetimeoutsec, useragent.toStdString(), jarfile.toStdString(), CUrlDD_NeedDataBit_Header,0,-1,0,0);


				if (downer_retcode != 0)
				{
					break;
				}

				headstr2 = downer_result.targetHeader.c_str();
				if (headstr2.lastIndexOf("HTTP/1") != -1)headstr2 = headstr2.mid(headstr2.lastIndexOf("HTTP/1"));
				contentlength = QRegularExpression("Content-Length: ([^\\r\\n]+)", reflag).match(headstr2).captured(1).trimmed().toLower().trimmed();
				if (MultiHostUrlReStr == "")
				{
					bmultihosturlok = true;
					break;
				}
				else
				{
					auto ma = QRegularExpression(MultiHostUrlReStr,reflag).match(url);
					if (!ma.hasMatch())
					{
						bmultihosturlok = true;
						break;
					}
					else{
						bool b200ok = false;
						QString mhosturl2;
						for (int i = 0; i < replacetestls2.size(); i++)
						{
							QStringList replacetestls2ls = replacetestls2[i].split("`");
							if (replacetestls2ls.size() < 2)continue;
							mhosturl2 = urlunpercenteccode(url);
							if (replacetestls2ls[0].startsWith("[regex]"))
							{
								QString restrforurlf = replacetestls2ls[0].mid(7);
								mhosturl2 = RegexGobalMatchAndCombi(mhosturl2, restrforurlf, replacetestls2ls[1]);
							}
							else{
								mhosturl2 = mhosturl2.replace(replacetestls2ls[0], replacetestls2ls[1]);
							}
							mhosturl2 = toutf8encodeurl(mhosturl2);
							if (mhosturl2 == url)
								continue;
							downer_retcode = CurlHttpGet(u8url.data(), &postdata, &downer_result, 0, pagetimeoutsec, useragent.toStdString(), jarfile.toStdString(), CUrlDD_NeedDataBit_Header);
							headstr2 = downer_result.targetHeader.c_str();
							if (headstr2.lastIndexOf("HTTP/1") != -1)headstr2 = headstr2.mid(headstr2.lastIndexOf("HTTP/1"));
							QString httpreturncode2 = QRegularExpression("http/\\d+[.]\\d+\\s+(\\d+)\\s.*", reflag).match(headstr2).captured(1).toLower().trimmed();
							if (httpreturncode2 == "200")
							{
								b200ok = false;
								break;
							}
						}
						if (b200ok)
						{
							url = mhosturl2;
							if (multihostnewurl != "")
							{
								multihost_hitcntmutex.lock();
								if (urlrepexpi >= 1)
								{
									if (multihost_hitcnt.find(MultiHostRepExprLs2[urlrepexpi - 1]) == multihost_hitcnt.end())
										multihost_hitcnt.insert(MultiHostRepExprLs2[urlrepexpi - 1], 1);
									else
										multihost_hitcnt[MultiHostRepExprLs2[urlrepexpi - 1]]++;
                                    QFile lszf(savepath + "multihoststat.txt");
									lszf.open(QIODevice::WriteOnly);
									for (auto lszit = multihost_hitcnt.begin(); lszit != multihost_hitcnt.end(); lszit++)
									{
										lszf.write((lszit.key() + u("\t%1\n").arg(lszit.value())).toUtf8());
									}
									lszf.close();
								}
								multihost_hitcntmutex.unlock();
							}
							bmultihosturlok = true;
							break;
						}

						if (headstr2.indexOf("Content-Length: ")==-1 || QRegularExpression(MultiHostNotUrlHeadMatch, reflag).match(headstr2).hasMatch())
						{
							if (bMultiHostRepExprLs2 == false)
							{
								QRegularExpressionMatch maddd;
								if (MultiHostRepExprLs.size()>0)
									maddd = QRegularExpression("/(\\d{6})/", reflag).match(MultiHostRepExprLs[0]);
								if (MultiHostRepExpr.startsWith("all:")==false && maddd.hasMatch())
								{
									std::list <QDateTime> urlddlist;
									for (int lkki = 0; lkki < MultiHostRepExprLs.size(); lkki++)
									{
										maddd = QRegularExpression("/(\\d{6})/", reflag).match(MultiHostRepExprLs[lkki]);
										QString yeardd = maddd.captured(1);
										if (yeardd != "")
										{
											QDateTime urldd = QDateTime::fromString(yeardd + "01", "yyyyMMdd");
											urlddlist.push_back(urldd);
										}
										else{
											MultiHostRepExprLs2.push_back(MultiHostRepExprLs[lkki]);
										}
									}

									urlddlist.sort();
									std::deque<QDateTime> urlddlist2;

									for (auto kdit = urlddlist.begin(); kdit != urlddlist.end(); kdit++)
									{
										urlddlist2.push_back(*kdit);
									}
									std::deque<QDateTime> selddls;
									for (int kdi = 0; kdi < urlddlist2.size(); kdi++)
									{
										QString dkkdf1 = urlddlist2[kdi].toString("yyyy-MM-dd HH:mm:ss");
										QString dkkdf2;
										if (kdi + 1<urlddlist2.size())
											dkkdf2 = urlddlist2[kdi + 1].toString("yyyy-MM-dd HH:mm:ss");
										QString kkfk3 = addtt.toString("yyyy-MM-dd HH:mm:ss");
										if (kdi + 1<urlddlist2.size() && addtt >= urlddlist2[kdi] && addtt <= urlddlist2[kdi + 1])
										{
											if (kdi + 2 < urlddlist2.size())
											{
												selddls.push_front(urlddlist2[kdi + 2]);
											}
											if (kdi - 1 >= 0)
											{
												selddls.push_front(urlddlist2[kdi - 1]);
											}
											selddls.push_front(urlddlist2[kdi]);
											selddls.push_front(urlddlist2[kdi + 1]);
											

											break;
										}
										else if (kdi + 1==urlddlist2.size() && addtt >= urlddlist2[kdi])
										{
											selddls.push_front(urlddlist2[kdi]);
											selddls.push_front(urlddlist2[kdi - 1]);
										}
									}


									for (int kdi = 0; kdi < selddls.size(); kdi++)
									{
										QString kksk2 = selddls[kdi].toString("/yyyyMM/");
										for (int kkai = 0; kkai < MultiHostRepExprLs.size(); kkai++)
										{
											if (MultiHostRepExprLs[kkai].indexOf(kksk2) != -1)
											{
												MultiHostRepExprLs2.push_back(MultiHostRepExprLs[kkai]);
												break;
											}
										}
									}
									if (MultiHostRepExprLs2.size() == 0)
									{
										MultiHostRepExprLs2 = MultiHostRepExprLs;
									}
								}
								else{
									MultiHostRepExprLs2 = MultiHostRepExprLs;
								}
								bMultiHostRepExprLs2 = true;
							}

							if (urlrepexpi < MultiHostRepExprLs2.size())
							{
								QString url2 = urlunpercenteccode(url);
								multihostnewurl = RegexGobalMatchAndCombi(url2, MultiHostUrlReStr, MultiHostRepExprLs2[urlrepexpi]);
								multihostnewurl = toutf8encodeurl(multihostnewurl);
								u8url = multihostnewurl.toUtf8();
								urlrepexpi++;
							}
							else{
								break;
							}

						}
						else
						{
							if (multihostnewurl != "")
							{
								url = multihostnewurl;

								multihost_hitcntmutex.lock();
								if (urlrepexpi >= 1)
								{
									if (multihost_hitcnt.find(MultiHostRepExprLs2[urlrepexpi - 1]) == multihost_hitcnt.end())
										multihost_hitcnt.insert(MultiHostRepExprLs2[urlrepexpi - 1], 1);
									else
										multihost_hitcnt[MultiHostRepExprLs2[urlrepexpi - 1]]++;
                                    QFile lszf(savepath + "multihoststat.txt");
									lszf.open(QIODevice::WriteOnly);
									for (auto lszit = multihost_hitcnt.begin(); lszit != multihost_hitcnt.end(); lszit++)
									{
										lszf.write((lszit.key() + u("\t%1\n").arg(lszit.value())).toUtf8());
									}
									lszf.close();
								}
								multihost_hitcntmutex.unlock();
							}
							bmultihosturlok = true;
							break;
						}
					}
				}
			}


			if (contentlength != "")
			{
				gettaskmutex.Lock();
				QString query4sql = QString() + "update taskurls set contentlength=" + contentlength + " where id=" + urlid + "";
				rl = query4.exec(query4sql);
				gettaskmutex.Unlock();
			}


			if (downer_retcode != 0)
			{
				gettaskmutex.Lock();
				QString query4sql = QString() + "update taskurls set urlstate=800+" + u("%1").arg(downer_retcode) + " where id=" + urlid + "";
				rl = query4.exec(query4sql);
				gettaskmutex.Unlock();
				continue;
			}


			QString urlsetstrl;
			if (oldurl != url)
			{
				urlsetstrl = " url='" + url + "', ";
			}

			if (MultiHostUrlReStr != "" && bmultihosturlok == false)
			{
				gettaskmutex.Lock();
				QString query4sql = QString() + "update taskurls set urlstate=912 where id=" + urlid + "";
				rl = query4.exec(query4sql);
				gettaskmutex.Unlock();
				continue;
			}
			//上面测试后url可能变化,再次生成文件名
			urlfilename = urltofilename(url, &urlre_ext);


			/*if (url.indexOf("bzfxw.com") != -1 && contentlength.toLongLong()<6*1024)
			{
				if (url.indexOf("/lw1/") != -1)
				{
					url = url.replace("/lw1/", "/lw2/");
					u8url = url.toUtf8();
					downer_retcode = CurlHttpGet(u8url.data(), &postdata, &downer_result, 0, pagetimeoutsec, useragent.toStdString(), jarfile.toStdString(), CUrlDD_NeedDataBit_Header);


					if (downer_retcode != 0)
					{
						gettaskmutex.Lock();
						QString query4sql = QString() + "update taskurls set urlstate=800+" + u("%1").arg(downer_retcode) + " where id=" + urlid + "";
						rl = query4.exec(query4sql);
						gettaskmutex.Unlock();
						continue;
					}

					headstr2 = downer_result.targetHeader.c_str();
					contentlength = QRegularExpression("Content-Length: ([^\\r\\n]+)", reflag).match(headstr2).captured(1).trimmed().toLower().trimmed();
					if (contentlength.toLongLong() < 6 * 1024)
					{
						gettaskmutex.Lock();
						QString query4sql = QString() + "update taskurls set urlstate=912 where id=" + urlid + "";
						rl = query4.exec(query4sql);
						gettaskmutex.Unlock();
						continue;
					}
				}
				else{
					gettaskmutex.Lock();
					QString query4sql = QString() + "update taskurls set urlstate=912 where id=" + urlid + "";
					rl = query4.exec(query4sql);
					gettaskmutex.Unlock();
					continue;
				}
				
			}*/


			ctttype2 = QRegularExpression("Content-Type: ([^\\r\\n]+)", reflag).match(headstr2).captured(1).toLower().trimmed();
			LastModified = QRegularExpression("Last-Modified: ([^\\r\\n]+)", reflag).match(headstr2).captured(1).toLower().trimmed();
			QString httpreturncode2 = QRegularExpression("http/\\d+[.]\\d+\\s+(\\d+)\\s.*", reflag).match(headstr2).captured(1).toLower().trimmed();
			if (!(httpreturncode2=="200" || httpreturncode2 == "301" || httpreturncode2 == "302"))
			{
				gettaskmutex.Lock();
				QString query4sql = QString() + "update taskurls set urlstate=" + httpreturncode2 + " where id=" + urlid + "";
				rl = query4.exec(query4sql);
				gettaskmutex.Unlock();
				continue;
			}

			if (LastModified != "")
			{
				QDateTime lmodt;
				if (LsstModifiedTimeQtDateTime(LastModified, lmodt))
				{
					if (lmodt.toString("yyyy-MM-dd HH:mm:ss") == addtime)
					{
						gettaskmutex.Lock();
						QString query4sql = QString() + "update taskurls set urlstate=2 where id=" + urlid + "";
						rl = query4.exec(query4sql);
						gettaskmutex.Unlock();
						continue;
					}
				}
			}

			if (contentlength.toLongLong() > maxdownsize)
			{
				gettaskmutex.Lock();
				QString query4sql = QString() + "update taskurls set urlstate=901 where id=" + urlid + "";
				rl = query4.exec(query4sql);
				gettaskmutex.Unlock();
				continue;
			}



			bool bmetapass = metaincls.size() == 0 ? true : false;
			for (int i = metaincls.size() - 1; i >= 0; i--)
			{
				if (metaincre[i].isValid())
				{
					if (metaincre[i].match(headstr2).hasMatch())
					{
						bmetapass = true;
						break;
					}
				}
				else if (headstr2.indexOf(metaincls[i]) != -1)
				{
					bmetapass = true;
					break;
				}
			}
			if (bmetapass)
			{
				for (int i = metaexcls.size() - 1; i >= 0; i--)
				{
					if (metaexcre[i].isValid())
					{
						if (metaexcre[i].match(headstr2).hasMatch())
						{
							bmetapass = false;
							break;
						}
					}
					else if (headstr2.indexOf(metaexcls[i]) != -1)
					{
						bmetapass = false;
						break;
					}
				}
			}

			if (!bmetapass)
			{
				gettaskmutex.Lock();
				QString query4sql = QString() + "update taskurls set urlstate=903 where id=" + urlid + "";
				rl = query4.exec(query4sql);
				gettaskmutex.Unlock();
				continue;
			}


			bool bnoexistpass = true;
			for (int i = noexistmatchrels.size() - 1; i >= 0; i--)
			{
				if (noexistmatchrels[i].isValid())
				{
					if (noexistmatchrels[i].match(headstr2).hasMatch())
					{
						bnoexistpass = false;
						break;
					}
				}
				else if (headstr2.indexOf(noexistmatchls[i]) != -1)
				{
					bnoexistpass = false;
					break;
				}
			}
			if (bnoexistpass == false)
			{
				gettaskmutex.Lock();
				QString query4sql = QString() + "update taskurls set urlstate=904 where id=" + urlid + "";
				rl = query4.exec(query4sql);
				gettaskmutex.Unlock();
				continue;
			}

			int pagetimeoutsec2 = 0;
			if (ctttype2 == "application/octet-stream"
				|| ctttype2.toLower().indexOf("mp3") != -1
				|| ctttype2.toLower().indexOf("mp4") != -1
				|| ctttype2.toLower().indexOf("video") != -1
				|| ctttype2.toLower().indexOf("audio") != -1)
			{
				pagetimeoutsec2 = contentlength.toLongLong()/(60*1024)+20;
			}


			//PageGate start
			bool bpagegatedata = false;
			QString PageGateHtml;
			bool bpagegate1ma = false,bpagegate2ma=false;
			if (PageGateCrawlUrlMatch1 != "" && QRegularExpression(PageGateCrawlUrlMatch1).match(url).hasMatch())
			{
				bpagegate1ma = true;
			}
			if (PageGateCrawlUrlMatch2 != "" && QRegularExpression(PageGateCrawlUrlMatch2).match(url).hasMatch())
			{
				bpagegate2ma = true;
			}
			if (bpagegate1ma || bpagegate2ma)
			{
				if (PageGateSrvLs.size() == 0)
				{
					gettaskmutex.Lock();
					QString query4sql = QString() + "update taskurls set urlstate=906 where id=" + urlid + "";
					rl = query4.exec(query4sql);
					gettaskmutex.Unlock();
					continue;
				}
			}

			if (bpagegate1ma || bpagegate2ma)
			{
				//this->PageGateCrawlUrlMatch1 = PageGateCrawlUrlMatch1;
				//this->PageGateScript1 = PageGateScript1;
				QString srv = PageGateSrvLs[lb_uniform_int(0, (int)PageGateSrvLs.size() - 1)];
				QString ip = srv.mid(0, srv.indexOf(":")), port = srv.mid(srv.lastIndexOf(":")+1);
				std::string pgresult;
				if (bpagegate1ma)
				{
					QString PageGateScript1_1 = PageGateScript1;
					PageGateScript1_1 = PageGateScript1_1.replace("__url__", url);
					auto pgu8script = PageGateScript1_1.toUtf8();
					pgresult = qtudpclisendrecv(ip.toStdString(), port.toInt(), std::string(pgu8script.data(), pgu8script.size()));
				}
				else if (bpagegate2ma)
				{
					QString PageGateScript2_1 = PageGateScript1;
					PageGateScript2_1 = PageGateScript2_1.replace("__url__", url);
					auto pgu8script = PageGateScript2_1.toUtf8();
					pgresult = qtudpclisendrecv(ip.toStdString(), port.toInt(), std::string(pgu8script.data(), pgu8script.size()));
				}
                std::string output=pgresult;
                //snappy::Uncompress(pgresult.data(), pgresult.size(), &output);
                /*
				if (output == "****data too big****")
				{
					gettaskmutex.Lock();
					QString query4sql = QString() + "update taskurls set urlstate=907 where id=" + urlid + "";
					rl = query4.exec(query4sql);
					gettaskmutex.Unlock();
					continue;
				}
                */

				QJsonDocument pdoc = QJsonDocument::fromJson(QByteArray(output.data(), output.size()));
				auto obj = pdoc.object();
				//UrlSet to task db
				if (obj.contains("UrlSet"))
				{
					QMap<QString, QString> url_urlstate2;
					QString UrlSetStr=obj["UrlSet"].toString();
					QStringList urlls=UrlSetStr.split("`");
					for (int i = 0; i < urlls.size(); i++)
					{
						bool bpass = incls.size() == 0 ? true : false;
						for (int i2 = incls.size() - 1; i2 >= 0; i2--)
						{
							if (incre[i2].isValid())
							{
								if (incre[i2].match(urlls[i2]).hasMatch())
								{
									bpass = true;
									break;
								}
							}
							else if (urlls[i2].indexOf(incls[i2]) != -1)
							{
								bpass = true;
								break;
							}
						}
						if (bpass)
						{
							for (int i2 = excls.size() - 1; i2 >= 0; i2--)
							{
								if (excre[i2].isValid())
								{
									if (excre[i2].match(urlls[i2]).hasMatch())
									{
										bpass = false;
										break;
									}
								}
								else if (urlls[i2].indexOf(excls[i2]) != -1)
								{
									bpass = false;
									break;
								}
							}
						}

						if (bpass)
						{
							bool bfound = false;
							for (int uti = 0; uti < ustaterels.size(); uti++)
							{
								if (ustaterels[uti].match(urlls[i]).hasMatch())
								{
									bfound = true;
									url_urlstate2.insert(urlls[i], ustatevalls[uti]);
									break;
								}
							}
							if (bfound == false)
							{
								url_urlstate2.insert(urlls[i], "0");
							}
						}
					}
					gettaskmutex.Lock();
                    //db.transaction();
					for (auto uiit = url_urlstate2.begin(); uiit != url_urlstate2.end(); uiit++)
					{
						QString query4sql;
						query4sql = QString() + "insert into taskurls(url,urlstate) values('" + uiit.key() + "'," + uiit .value()+ ")";
						bool prl = query4.prepare(query4sql);
						auto rl = query4.exec();
					}
                    //db.commit();
					gettaskmutex.Unlock();

				}

				//
				if (obj.contains("SaveAsData") && obj["SaveAsData"].toString() == "1")
				{
					if (savetorocksdb == "" || savetorocksdb != "1")
					{
						if (savetorocksdb == "3")
						{
							
							auto output2 = url.toUtf8() + "\n"+QByteArray(output.data(),output.size());
							uLong destbuflen = 8 * 1024 * 1024-4;
							int rl = compress2((Bytef*)destbuf, &destbuflen, (Bytef*)output2.data(), output2.size(), 9);
							if (rl == Z_OK){
								datapackagefm.lock();
								auto datapfpos=datapackagef->pos();
								uint32_t outputlen = (uint32_t)destbuflen;
								qToBigEndian<qint32>(qint32(outputlen), (uchar*)destbuf+8*1024*1024-4);
								datapackagef->write(destbuf + 8 * 1024 * 1024 - 4, 4);
								datapackagef->write(destbuf, destbuflen);
								datapackagefm.unlock();

								gettaskmutex.Lock();
								QString query4sql = QString() + "update taskurls set urlstate=2,dataindex=" + segmentdataid + ",dataoffset=" + QString("%1").arg(datapfpos) + " where id=" + urlid + "";
								rl = query4.exec(query4sql);
								gettaskmutex.Unlock();
							}
							else{
								gettaskmutex.Lock();
								QString query4sql = QString() + "update taskurls set urlstate=919 where id=" + urlid + "";
								rl = query4.exec(query4sql);
								gettaskmutex.Unlock();
							}
						}
						else{
							gettaskmutex.Lock();
							QString query4sql = QString() + "update taskurls set urlstate=2 where id=" + urlid + "";
							rl = query4.exec(query4sql);
							gettaskmutex.Unlock();

                            QString localhtmlpath = savepath + urlfilename;
							QFile ff(localhtmlpath);
							ff.open(QIODevice::WriteOnly);
							if (relink_html_to_local == "" || relink_html_to_local == "1")
							{
								ff.write(output.data(), output.size());
							}
							else{
								ff.write(output.data(), output.size());
							}
							ff.close();
						}
					}
					else{
						gettaskmutex.Lock();
						QString query4sql = QString() + "update taskurls set urlstate=2 where id=" + urlid + "";
						rl = query4.exec(query4sql);
                        gettaskmutex.Unlock();

                        auto buf=new char[output.size()+4096];
                        uLong buflen=output.size()+4096;
                        gzcompress((Bytef*)output.data(),output.size(),(Bytef*)buf,&buflen);


                        g_rockfmutex.lock();
                        int32_t keylen;
                        int32_t vallen;
                        keylen=url.toUtf8().size();
                        vallen=buflen;
                        char keylenbt[4];
                        qToBigEndian(keylen,(uchar*)keylenbt);
                        char vallenbt[4];
                        qToBigEndian(vallen,(uchar*)vallenbt);
                        g_rockf->write(keylenbt,4);
                        g_rockf->write(url.toUtf8());
                        g_rockf->write(vallenbt,4);
                        g_rockf->write(buf,buflen);

                        g_rockfmutex.unlock();
                        delete[]buf;
					}
					continue;
				}
				else if (obj.contains("PageHtml"))
				{
					auto kdsfjdf = obj["PageHtml"].toString();;
					if (obj["PageHtml"].toString() == "")
					{
						//PageHtml Empty
						gettaskmutex.Lock();
						QString query4sql = QString() + "update taskurls set urlstate=908 where id=" + urlid + "";
						rl = query4.exec(query4sql);
						gettaskmutex.Unlock();
						continue;
					}
					else{
						bpagegatedata = true;
						PageGateHtml = obj["PageHtml"].toString();
					}
				}
				else{
					//unknow deal
					gettaskmutex.Lock();
					QString query4sql = QString() + "update taskurls set urlstate=909 where id=" + urlid + "";
					rl = query4.exec(query4sql);
					gettaskmutex.Unlock();
					continue;
				}
			}
			//page gate end
			//else{
			

			std::map<std::string, std::string> postdata2;
			bool bneedct = false;
			if (bpagegatedata == false)
			{
				if (contentlength != "" && contentlength.toLongLong() > memdownmaxsize.toLongLong() && (largefiledowntype == "1" || largefiledowntype == "2"))
				{
					if (largefiledowntype == "1")
					{
                        QString localhtmlpath2 = savepath  + urlfilename;
                        QString localhtmlpath = savepath + urlfilename + ".temp";
						uint64_t downstarti = 0;
						bool bdownfexist = false;
						if (QFile::exists(localhtmlpath))
						{
							QFileInfo downfinfo(localhtmlpath);
							downstarti = downfinfo.size();
							bdownfexist = true;
						}
						QFile ff(localhtmlpath);
						if (bdownfexist == false)
							ff.open(QIODevice::WriteOnly);
						else
							ff.open(QIODevice::Append);
						bool bdowner = false;
						for (; downstarti < contentlength.toLongLong(); downstarti += memdownmaxsize.toLongLong())
						{
							int downsize = memdownmaxsize.toLongLong();
							if (downstarti + memdownmaxsize.toLongLong() > contentlength.toLongLong())
							{
								downsize = contentlength.toLongLong() - downstarti;
							}
							int64_t downrangeend = downstarti + downsize - 1;
							downer_retcode = CurlHttpGet(u8url.data(), &postdata2, &downer_result, 0, pagetimeoutsec2, useragent.toStdString(), jarfile.toStdString(), CUrlDD_NeedDataBit_Content | CUrlDD_NeedDataBit_Header | CUrlDD_NeedDataBit_Cookie, downstarti, downrangeend);
							if (bpagegatedata == false && downer_result.targetHeader == "")
							{
								gettaskmutex.Lock();
								QString query4sql = QString() + "update taskurls set urlstate=913 where id=" + urlid + "";
								rl = query4.exec(query4sql);
								gettaskmutex.Unlock();
								bdowner = true;
								break;
							}
							else
							{
								QString headstr = downer_result.targetHeader.c_str();
								if (headstr.lastIndexOf("HTTP/1") != -1)headstr = headstr.mid(headstr.lastIndexOf("HTTP/1"));
								QString httpreturncode = QRegularExpression("http/\\d+[.]\\d+\\s+(\\d+)\\s.*", reflag).match(headstr).captured(1).toLower().trimmed();
								QString contentlength2 = QRegularExpression("Content-Length: ([^\\r\\n]+)", reflag).match(headstr).captured(1).trimmed().toLower().trimmed();
								if (bpagegatedata == false && !(httpreturncode == "200" || httpreturncode == "301" || httpreturncode == "302"))
								{
									gettaskmutex.Lock();
									QString query4sql = QString() + "update taskurls set urlstate=" + httpreturncode + " where id=" + urlid + "";
									rl = query4.exec(query4sql);
									gettaskmutex.Unlock();
									bdowner = true;
									break;
								}
								if (downer_result.targetCoentent.size() == downsize)
								{
									ff.write(downer_result.targetCoentent.c_str(), downer_result.targetCoentent.size());
								}
								else{
									bdowner = true;
									break;
								}
							}
						}
						int64_t resultfsize = ff.size();
						ff.close();
						if (bdowner == false)
						{
							if (resultfsize == contentlength.toLongLong())
							{
								QString headstr = downer_result.targetHeader.c_str();
								if (headstr.lastIndexOf("HTTP/1") != -1)headstr = headstr.mid(headstr.lastIndexOf("HTTP/1"));
								QString ctttype = QRegularExpression("Content-Type: ([^\\r\\n]+)", reflag).match(headstr).captured(1).toLower().trimmed();

								QString addtimesetstr = "";
								if (LastModified != "")
								{
									QDateTime datet;
									if (LsstModifiedTimeQtDateTime(LastModified, datet))
									{
										addtimesetstr = " addtime='" + datet.toString("yyyy-MM-dd HH:mm:ss") + "', ";
									}
								}

								QFile::rename(localhtmlpath, localhtmlpath2);

								updateurlstate(ctttype, query4, addtimesetstr, content_disposition, urlid, "2", urlsetstrl);
								continue;
							}
						}
					}
					else if (largefiledowntype == "2")
					{
                        /*QString localhtmlpath = savepath + urlfilename;
						QFile ff(localhtmlpath);
						ff.open(QIODevice::WriteOnly);
						downer_retcode = CurlHttpGet(u8url.data(), &postdata2, &downer_result, 0, pagetimeoutsec2, useragent.toStdString(), jarfile.toStdString(), CUrlDD_NeedDataBit_Content | CUrlDD_NeedDataBit_Header | CUrlDD_NeedDataBit_Cookie, 0, -1, 0, &ff);
						ff.close();*/

                        /*QString localhtmlpath = savepath + urlfilename;
						auto downfwpa=localhtmlpath.toStdWString();
						HANDLE downhf = CreateFile(downfwpa.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
						downer_retcode = CurlHttpGet(u8url.data(), &postdata2, &downer_result, 0, pagetimeoutsec2, useragent.toStdString(), jarfile.toStdString(), CUrlDD_NeedDataBit_Content | CUrlDD_NeedDataBit_Header | CUrlDD_NeedDataBit_Cookie, 0, -1, DownTaskThread_write_func, downhf);
						CloseHandle(downhf);*/

                        /*QString localhtmlpath = savepath + urlfilename;
						auto downfwpa = localhtmlpath.toStdWString();
						FILE *ff=_wfopen(downfwpa.data(), L"wb");
						if (ff != 0)
						{
							downer_retcode = CurlHttpGet(u8url.data(), &postdata2, &downer_result, 0, pagetimeoutsec2, useragent.toStdString(), jarfile.toStdString(), CUrlDD_NeedDataBit_Content | CUrlDD_NeedDataBit_Header | CUrlDD_NeedDataBit_Cookie, 0, -1, 0, ff);
							fclose(ff);
						}*/

                        QString localhtmlpath = savepath + urlfilename;
						auto downfwpa = localhtmlpath.toStdWString();
						HANDLE ff = CreateFile(downfwpa.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, 0);
						if (ff != INVALID_HANDLE_VALUE)
						{
							downer_retcode = CurlHttpGet(u8url.data(), &postdata2, &downer_result, 0, pagetimeoutsec2, useragent.toStdString(), jarfile.toStdString(), CUrlDD_NeedDataBit_Content | CUrlDD_NeedDataBit_Header | CUrlDD_NeedDataBit_Cookie, 0, -1, 0, ff);
							CloseHandle(ff);
						}

						QFileInfo downfinfo(localhtmlpath);
						if (downfinfo.size() == contentlength.toLongLong() || contentlength=="")
						{
							QString headstr = QString::fromUtf8(downer_result.targetHeader.c_str(), downer_result.targetHeader.size());
							if (headstr.lastIndexOf("HTTP/1") != -1)headstr = headstr.mid(headstr.lastIndexOf("HTTP/1"));
							QString ctttype = QRegularExpression("Content-Type: ([^\\r\\n]+)", reflag).match(headstr).captured(1).toLower().trimmed();

							QString addtimesetstr = "";
							if (LastModified != "")
							{
								QDateTime datet;
								if (LsstModifiedTimeQtDateTime(LastModified, datet))
								{
									addtimesetstr = " addtime='" + datet.toString("yyyy-MM-dd HH:mm:ss") + "', ";
								}
							}

							updateurlstate(ctttype, query4, addtimesetstr, content_disposition, urlid, "2", urlsetstrl);
							bneedct = true;
						}
						else{ 
							if (downfinfo.size() > contentlength.toLongLong())
							{
								gettaskmutex.Lock();
								QString query4sql = QString() + "update taskurls set urlstate=917 where id=" + urlid + "";
								rl = query4.exec(query4sql);
								gettaskmutex.Unlock();
								bneedct = true;
							}
							else{
								gettaskmutex.Lock();
								QString query4sql = QString() + "update taskurls set urlstate=916 where id=" + urlid + "";
								rl = query4.exec(query4sql);
								gettaskmutex.Unlock();
								bneedct = true;
							}
						}
					}
				}
				else{
					downer_retcode = CurlHttpGet(u8url.data(), &postdata2, &downer_result, 0, pagetimeoutsec2, useragent.toStdString(), jarfile.toStdString(), CUrlDD_NeedDataBit_Content | CUrlDD_NeedDataBit_Header | CUrlDD_NeedDataBit_Cookie);
				}
			}
			//
			if (bneedct)continue;

			if (bpagegatedata==false && downer_result.targetHeader == "")
			{
				gettaskmutex.Lock();
				QString query4sql = QString() + "update taskurls set urlstate=913 where id=" + urlid + "";
				rl = query4.exec(query4sql);
				gettaskmutex.Unlock();
				continue;
			}
			else
			{
				QString headstr = QString::fromUtf8(downer_result.targetHeader.c_str(), downer_result.targetHeader.size());
				if (headstr.lastIndexOf("HTTP/1") != -1)headstr = headstr.mid(headstr.lastIndexOf("HTTP/1") );
				QString httpreturncode = QRegularExpression("http/\\d+[.]\\d+\\s+(\\d+)\\s.*", reflag).match(headstr).captured(1).toLower().trimmed();
				QString contentlength2 = QRegularExpression("Content-Length: ([^\\r\\n]+)", reflag).match(headstr).captured(1).trimmed().toLower().trimmed();
				if (bpagegatedata == false && !(httpreturncode == "200" || httpreturncode == "301" || httpreturncode == "302"))
				{
					gettaskmutex.Lock();
					QString query4sql = QString() + "update taskurls set urlstate=" + httpreturncode + " where id=" + urlid + "";
					rl = query4.exec(query4sql);
					gettaskmutex.Unlock();
					continue;
				}
				if (bpagegatedata == false && contentlength2 != "" && contentlength2.toLongLong()>0 && downer_result.targetCoentent.size()<contentlength2.toLongLong())
				{
					gettaskmutex.Lock();
					QString query4sql = QString() + "update taskurls set urlstate=916 where id=" + urlid + "";
					rl = query4.exec(query4sql);
					gettaskmutex.Unlock();
					continue;
				}

				

				QString content_disposition = QRegularExpression("Content-Disposition: ([^\\r\\n]+)", reflag).match(headstr).captured(1).trimmed();
				if (content_disposition.indexOf("filename=") != -1)
				{
					content_disposition = content_disposition.mid(content_disposition.indexOf("filename=") + strlen("filename="));
				}
				//似乎不存在
				//if (content_disposition.size() > 0)
				//{
				//	auto nameurldecode=UrlDecode(content_disposition.toStdString());
				//	auto cdispcodename=detecttextcodecname(nameurldecode.data(), nameurldecode.size());
				//	if (cdispcodename.size() > 0)
				//	{
				//		auto cdispcodec=QTextCodec::codecForName(cdispcodename.data());
				//		content_disposition=cdispcodec->toUnicode(nameurldecode.data(), nameurldecode.size());
				//	}
				//}


				QString addtimesetstr = "";
				if (LastModified != "")
				{
					QDateTime datet;
					if (LsstModifiedTimeQtDateTime(LastModified, datet))
					{
						addtimesetstr = " addtime='" + datet.toString("yyyy-MM-dd HH:mm:ss") + "', ";
					}
				}

				QString ctttype = QRegularExpression("Content-Type: ([^\\r\\n]+)", reflag).match(headstr).captured(1).toLower().trimmed();
				if (bpagegatedata==true || ctttype.indexOf("text/html") != -1)
				{
					QTextCodec* codc = 0;
					if (bpagegatedata == false)
					{
						std::string codecname = detecttextcodecname(downer_result.targetCoentent.c_str(), downer_result.targetCoentent.size());
						codc = QTextCodec::codecForName(codecname.c_str());
						if (codc == NULL)
						{
							if (ctttype.indexOf("charset=") != -1)
							{
								QString defcodecname = ctttype.mid(ctttype.indexOf("charset=") + strlen("charset=")).trimmed();
								QRegularExpression re2("([A-Za-z0-9\\-]+).*", reflag);
								auto ma2 = re2.match(defcodecname);
								if (ma2.hasMatch())
								{
									defcodecname = ma2.captured(1);
									if (defcodecname.size() > 0)
										codc = QTextCodec::codecForName(defcodecname.toUtf8());
								}
							}
							if (codc == NULL)
							{
								gettaskmutex.Lock();
								QString query4sql = QString() + "update taskurls set urlstate=914 where id=" + urlid + "";
								rl = query4.exec(query4sql);
								gettaskmutex.Unlock();
								continue;
							}
						}
					}

					QString pagetext;
					if (bpagegatedata == false)
					{
						pagetext = codc->toUnicode(downer_result.targetCoentent.c_str(), downer_result.targetCoentent.size());
					}
					else{
						pagetext = PageGateHtml;
					}



					if (PageFullUrlRestr1.size() != 0)
					{
						if (QRegularExpression(PageFullUrlRestr1, reflag).match(url).hasMatch())
						{
							if (!QRegularExpression(PageCttFullReStr1, reflag).match(pagetext).hasMatch())
							{
								gettaskmutex.Lock();
								QString query4sql = QString() + "update taskurls set urlstate=918 where id=" + urlid + "";
								rl = query4.exec(query4sql);
								gettaskmutex.Unlock();
								continue;
							}
						}
					}
					if (PageFullUrlRestr2.size() != 0)
					{
						if (QRegularExpression(PageFullUrlRestr2, reflag).match(url).hasMatch())
						{
							if (!QRegularExpression(PageCttFullReStr2, reflag).match(pagetext).hasMatch())
							{
								gettaskmutex.Lock();
								QString query4sql = QString() + "update taskurls set urlstate=918 where id=" + urlid + "";
								rl = query4.exec(query4sql);
								gettaskmutex.Unlock();
								continue;
							}
						}
					}


					bool bpagenoexistpass = true;
					for (int i = pagenoexistmatchrels.size() - 1; i >= 0; i--)
					{
						if (pagenoexistmatchrels[i].isValid())
						{
							if (pagenoexistmatchrels[i].match(pagetext).hasMatch())
							{
								bpagenoexistpass = false;
								break;
							}
						}
						else if (pagetext.indexOf(pagenoexistmatchls[i]) != -1)
						{
							bpagenoexistpass = false;
							break;
						}
					}
					if (bpagenoexistpass == false)
					{
						gettaskmutex.Lock();
						QString query4sql = QString() + "update taskurls set urlstate=905 where id=" + urlid + "";
						rl = query4.exec(query4sql);
						gettaskmutex.Unlock();
						continue;
					}


					std::set<QString> newurl;
					QString newpagectt;
					

					//self defined get url
					bool burlfindre1 = false,burlfindre2=false;
					QList<QStringList>  combiresultlist;
					for (int boni = 0; boni < combiurlreprels.size(); boni++)
					{
						QStringList reprelist;
						reprelist.push_back(combiurlreprels[boni]);
						reprelist.push_back(combitimereprels[boni]);
						reprelist.push_back(combititlereprels[boni]);
						for (int repi = 0; repi < reprelist.size(); repi++)
						{
							reprelist[repi] += "`^";
						}
						auto findurlrlls2 = RegexGobalMatchAndCombi_v_list(pagetext, urlfindrels[boni], reprelist);
						combiresultlist.append(findurlrlls2);
						burlfindre2 = true;
					}


					bool bhaveurlpass = false;
					if (combiresultlist.size()>0 && (burlfindre1 == true || burlfindre2 == true))
					{
						QMap<QString, QStringList> sqllist_title;
						for (int combrli = 0; combrli < combiresultlist.size(); combrli++)
						{
							QStringList allurlstrls = combiresultlist[combrli][0].split("`^");
							QStringList alltimstrls = combiresultlist[combrli][1].split("`^");
							QStringList alltitlestrls = combiresultlist[combrli][2].split("`^");
							int re1size = allurlstrls.size();

							if (allurlstrls.size() > 0 && allurlstrls.size() == alltimstrls.size() && alltimstrls.size() == alltitlestrls.size())
							{
								for (int reui = 0; reui < allurlstrls.size(); reui++)
								{
									bool bcururl = false;
									allurlstrls[reui]=allurlstrls[reui].trimmed();
									if (allurlstrls[reui] == "__cururl__")
									{
										bcururl = true;
										allurlstrls[reui] = url;
									}
									allurlstrls[reui]=allurlstrls[reui].replace("\\9", "");
									allurlstrls[reui] = allurlstrls[reui].replace("\\8", "");
									allurlstrls[reui] = allurlstrls[reui].replace("\\7", "");
									allurlstrls[reui] = allurlstrls[reui].replace("\\6", "");
									allurlstrls[reui] = allurlstrls[reui].replace("\\5", "");
									allurlstrls[reui] = allurlstrls[reui].replace("\\4", "");
									allurlstrls[reui] = allurlstrls[reui].replace("\\3", "");
									allurlstrls[reui] = allurlstrls[reui].replace("\\2", "");
									allurlstrls[reui] = allurlstrls[reui].replace("\\1", "");
									allurlstrls[reui] = allurlstrls[reui].replace("\\0", "");
									allurlstrls[reui] = allurlstrls[reui].trimmed();
									if (allurlstrls[reui] == "")
									{
										continue;
									}
									allurlstrls[reui] = tofullurl(allurlstrls[reui], url);
									if (allurlstrls[reui].indexOf("://") == -1)
									{
										continue;
									}
									QString newurl43 = toutf8encodeurl(allurlstrls[reui]);
									allurlstrls[reui] = newurl43;

									QString newurlstateval;
									newurlstateval = newurlstatels[combrli];

									bool bpass = incls.size() == 0 ? true : false;
									for (int i = incls.size() - 1; i >= 0; i--)
									{
										if (incre[i].isValid())
										{
											if (incre[i].match(allurlstrls[reui]).hasMatch())
											{
												bpass = true;
												break;
											}
										}
										else if (allurlstrls[reui].indexOf(incls[i]) != -1)
										{
											bpass = true;
											break;
										}
									}
									if (bpass)
									{
										for (int i = excls.size() - 1; i >= 0; i--)
										{
											if (excre[i].isValid())
											{
												if (excre[i].match(allurlstrls[reui]).hasMatch())
												{
													bpass = false;
													break;
												}
											}
											else if (allurlstrls[reui].indexOf(excls[i]) != -1)
											{
												bpass = false;
												break;
											}
										}
									}

									if (bpass)
									{
										QString dctt = alltitlestrls[reui];
										auto dcttu8 = dctt.toUtf8();
										std::string dcttsrc(dcttu8.data(), dcttu8.size()), dcttresult;
										if (HtmlToText(dcttsrc, &dcttresult))
										{
											dctt = QString::fromUtf8(dcttresult.data(), dcttresult.size());
										}
										//if (query4sql.indexOf(".vtt") != -1 && dctt == "")
										//{
										//	int e = 324324;
										//}
										if (dctt.size() > 256)dctt = dctt.mid(0, 256);
										if (bcururl)
										{
											if (alltimstrls[reui] != "" || dctt != "")
											{
												sqllist_title.insert(allurlstrls[reui], QStringList() << newurlstateval << alltimstrls[reui] << dctt);
											}
										}
										else{
											sqllist_title.insert(allurlstrls[reui], QStringList() << newurlstateval << alltimstrls[reui] << dctt);
										}
										bhaveurlpass = true;
									}
								}
							}
						}
						gettaskmutex.Lock();
                        //db.transaction();
						for (auto stit = sqllist_title.begin(); stit != sqllist_title.end(); stit++)
						{
							QString query4sql;
							if (stit.value()[1].length() > 0)
							{
								stit.value()[1] = stit.value()[1].replace("/", "-");
								query4sql = QString() + "insert into taskurls(url,addtime,content_disposition,urlstate) values('" + stit.key() + "','" + stit.value()[1] + "',:content_disposition," + stit.value()[0] + ")";
							}
							else{
								query4sql = QString() + "insert into taskurls(url,content_disposition,urlstate) values('" + stit.key() + "',:content_disposition," + stit.value()[0] + ")";
							}
							bool prl = query4.prepare(query4sql);
							query4.bindValue(":content_disposition", stit.value()[2]);
							auto rl = query4.exec();
							if (rl == false && stit.value()[1]!="")
							{
								//update with url when addtime not equal current
								//update taskurls set urlstate=0,addtime=cur where url='' and addtime!=cur
								if (stit.value()[1] == "now()")
								{
									stit.value()[1] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
								}
								query4sql = QString() + "update taskurls set content_disposition=:content_disposition,urlstate=0,addtime='" + stit.value()[1] + "' where url='" + stit.key() + "' and (addtime!='" + stit.value()[1] + "' or addtime is null)";
								bool prl2 = query4.prepare(query4sql);
								query4.bindValue(":content_disposition", stit.value()[2]);
								auto rl2 = query4.exec();
								int e = 324324;
							}
						}
                        //db.commit();
						gettaskmutex.Unlock();
					}

					if (autofindlinktype == 0)
					{

					}
					else if (autofindlinktype == 1)
					{
						newpagectt = PageCttRelinkToLocal(pagetext, url, &newurl, incls, excls, incre, excre, &urlre_ext);
					}
					else if (autofindlinktype == 2)
					{
						if (bhaveurlpass == false)
						{
							newpagectt = PageCttRelinkToLocal(pagetext, url, &newurl, incls, excls, incre, excre, &urlre_ext);
						}
                    }else if (autofindlinktype == 4)
                    {
                        newpagectt = PageCttRelinkToNet(pagetext, url, &newurl, incls, excls, incre, excre, &urlre_ext);
                    }else if (autofindlinktype == 5)
                    {
                        if (bhaveurlpass == false)
                        {
                            newpagectt = PageCttRelinkToNet(pagetext, url, &newurl, incls, excls, incre, excre, &urlre_ext);
                        }
                    }



					QRegularExpression re("<meta[^>]*charset=[^>]*>", reflag);
					auto maes = re.globalMatch(newpagectt);
					if (maes.hasNext())
					{
						auto ma = maes.next();
						QString ctttype = ma.captured(0);
						QString ctttype2 = ctttype;
						QString codename = ctttype2.mid(ctttype2.indexOf(QRegularExpression("charset=", reflag)) + strlen("charset="));
						QRegularExpression re2("([A-Za-z0-9\\-\"']+).*", reflag);
						auto ma2 = re2.match(codename);
						codename = ma2.captured(1);
						if (codename.size() > 0)
						{
							if (codename[codename.size() - 1] == '"' && codename[0] != '"')
							{
								codename = codename.mid(0, codename.size() - 1);
							}
							if (codename[codename.size() - 1] == '\'' && codename[0] != '\'')
							{
								codename = codename.mid(0, codename.size() - 1);
							}
							if (codename[codename.size() - 1] == '"' && codename[0] == '"')
							{
								codename = codename.mid(1, codename.size() - 2);
							}
							if (codename[codename.size() - 1] == '\'' && codename[0] == '\'')
							{
								codename = codename.mid(1, codename.size() - 2);
							}
							ctttype2.replace(codename, "utf-8");
							newpagectt.replace(ctttype, ctttype2);
						}
					}

					/*
					//PageCttRelinkToLocal 函数会过滤
					std::set<QString> passurl;
					//newurl.clear();
					//newurl.insert("http://sb.v.youth.cn/zqw/vod/2017/01/25/2e214450a0aa48449636141a99e766f8?fmt=h264_1200k_mp4&slice=001&passkey=3243008051&sb.mp4");
					for (auto it = newurl.begin(); it != newurl.end(); it++)
					{
						bool bpass = incls.size() == 0 ? true : false;
						for (int i = incls.size() - 1; i >= 0; i--)
						{
							if (incre[i].isValid())
							{
								if (incre[i].match(*it).hasMatch())
								{
									bpass = true;
									break;
								}
							}
							else if (it->indexOf(incls[i]) != -1)
							{
								bpass = true;
								break;
							}
						}
						if (bpass)
						{
							for (int i = excls.size() - 1; i >= 0; i--)
							{
								if (excre[i].isValid())
								{
									if (excre[i].match(*it).hasMatch())
									{
										bpass = false;
										break;
									}
								}
								else if (it->indexOf(excls[i]) != -1)
								{
									bpass = false;
									break;
								}
							}
						}

						if (bpass)
						{
							passurl.insert(it->trimmed());

						}
					}
					*/

					QMap<QString, QString> url_urlstate2;
					for (auto it = newurl.begin(); it != newurl.end(); it++)//old is passurl
					{
                        auto start=(*it).indexOf("//")+2;
                        auto end=(*it).indexOf("/",9);
                        if(start!=-1 && end!=-1){
                            auto site=(*it).mid(start,end-start);
                            allsitem.lock();
                            if(allsite.find(site)==allsite.end()){
                                allsite.insert(site);
                                allsitef->write(site.toUtf8()+"\n");
                            }
                            allsitem.unlock();
                        }

						bool bfound = false;
						for (int uti = 0; uti < ustaterels.size(); uti++)
						{
							if (ustaterels[uti].match(*it).hasMatch())
							{
								bfound = true;
								url_urlstate2.insert(*it, ustatevalls[uti]);
								break;
							}
						}
						if (bfound == false)
						{
							url_urlstate2.insert(*it, "0");
						}
					}

					gettaskmutex.Lock();
                    //db.transaction();
					for (auto it = url_urlstate2.begin(); it != url_urlstate2.end(); it++)//old is passurl
					{
						QString query4sql = QString() + "insert into taskurls(url,urlstate) values('" + it.key()+"',"+it.value() + ")";
						rl = query4.exec(query4sql);
					}
                    //db.commit();
					gettaskmutex.Unlock();


					if (savetorocksdb == "" || savetorocksdb != "1")
					{
						if (savetorocksdb == "3")
						{
							
							QByteArray writedata;
							if (relink_html_to_local == "" || relink_html_to_local == "1")
							{
								writedata=url.toUtf8()+"\n"+newpagectt.toUtf8();
							}
							else{
								writedata = url.toUtf8() + "\n" + pagetext.toUtf8();
							}
							uint32_t outputlen = writedata.size();

							uLong destbuflen = 8 * 1024 * 1024 - 4;
							int rl = compress2((Bytef*)destbuf, &destbuflen, (Bytef*)writedata.data(), writedata.size(), 9);
							if (rl == Z_OK){
								datapackagefm.lock();
								auto datapfpos = datapackagef->pos();
								uint32_t outputlen = (uint32_t)destbuflen;
								qToBigEndian<qint32>(qint32(outputlen), (uchar*)destbuf + 8 * 1024 * 1024 - 4);
								datapackagef->write(destbuf + 8 * 1024 * 1024 - 4, 4);
								datapackagef->write(destbuf, destbuflen);
								datapackagefm.unlock();

								gettaskmutex.Lock();
								if (ctttype.indexOf("text/html") == -1)
								{
									QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=2,contenttype=:contenttype,content_disposition=:content_disposition,dataindex=" + segmentdataid + ",dataoffset=" + QString("%1").arg(datapfpos) + " where id=" + urlid + "";
									if (query4.prepare(query4sql))
									{
										query4.bindValue(":contenttype", ctttype);
										query4.bindValue(":content_disposition", content_disposition);
										if (query4.exec() == false)
										{
											QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=909,dataindex=" + segmentdataid + ",dataoffset=" + QString("%1").arg(datapfpos) + " where id=" + urlid + "";
											rl = query4.exec(query4sql);
										}
									}
									else{
										QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=2,dataindex=" + segmentdataid + ",dataoffset=" + QString("%1").arg(datapfpos) + " where id=" + urlid + "";
										rl = query4.exec(query4sql);
									}
								}
								else{
									QString query4sql = QString() + "update taskurls set  " + addtimesetstr + urlsetstrl + " urlstate=2,dataindex=" + segmentdataid + ",dataoffset=" + QString("%1").arg(datapfpos) + " where id=" + urlid + "";
									rl = query4.exec(query4sql);
								}
								gettaskmutex.Unlock();
								continue;
							}
							else{
								QString query4sql = QString() + "update taskurls set  " + addtimesetstr + urlsetstrl + " urlstate=919 where id=" + urlid + "";
								rl = query4.exec(query4sql);
								continue;
							}
						}
						else{
                            QString localhtmlpath = savepath + urlfilename;
							QFile ff(localhtmlpath);
							ff.open(QIODevice::WriteOnly);
							if (relink_html_to_local == "" || relink_html_to_local == "1")
							{
								ff.write(newpagectt.toUtf8());
							}
							else{
								ff.write(pagetext.toUtf8());
							}
							ff.close();
						}
					}
					else{
                        auto pagetextu8 = pagetext.toUtf8();

                        auto buf=new char[pagetextu8.size()+4096];
                        uLong buflen=pagetextu8.size()+4096;
                        gzcompress((Bytef*)pagetextu8.data(),pagetextu8.size(),(Bytef*)buf,&buflen);

                        g_rockfmutex.lock();
                        int32_t keylen;
                        int32_t vallen;
                        keylen=url.toUtf8().size();
                        vallen=buflen;
                        char keylenbt[4];
                        qToBigEndian(keylen,(uchar*)keylenbt);
                        char vallenbt[4];
                        qToBigEndian(vallen,(uchar*)vallenbt);
                        g_rockf->write(keylenbt,4);
                        g_rockf->write(url.toUtf8());
                        g_rockf->write(vallenbt,4);
                        g_rockf->write(buf,buflen);

                        g_rockfmutex.unlock();
                        delete[]buf;
					}
				}
				else
				{
					if (savetorocksdb == "" 
						|| savetorocksdb != "1" 
						|| ctttype == "application/octet-stream" 
						|| ctttype.toLower().indexOf("mp3") != -1
						|| ctttype.toLower().indexOf("mp4") != -1
						|| ctttype.toLower().indexOf("video") != -1
						|| ctttype.toLower().indexOf("audio") != -1
						)
					{
						if (savetorocksdb == "3"){
							
							QByteArray writedata = url.toUtf8()+"\n"+QByteArray(downer_result.targetCoentent.c_str(), downer_result.targetCoentent.size());

							uLong destbuflen = 8 * 1024 * 1024 - 4;
							int rl = compress2((Bytef*)destbuf, &destbuflen, (Bytef*)writedata.data(), writedata.size(), 9);
							if (rl == Z_OK){
								datapackagefm.lock();
								auto datapfpos = datapackagef->pos();
								uint32_t outputlen = (uint32_t)destbuflen;
								qToBigEndian<qint32>(qint32(outputlen), (uchar*)destbuf + 8 * 1024 * 1024 - 4);
								datapackagef->write(destbuf + 8 * 1024 * 1024 - 4, 4);
								datapackagef->write(destbuf, destbuflen);
								datapackagefm.unlock();

								if (contentlength == "" || contentlength.toLongLong() == downer_result.targetCoentent.size())
								{
									gettaskmutex.Lock();
									if (ctttype.indexOf("text/html") == -1)
									{
										QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=2,contenttype=:contenttype,content_disposition=:content_disposition,dataindex=" + segmentdataid + ",dataoffset=" + QString("%1").arg(datapfpos) + " where id=" + urlid + "";
										if (query4.prepare(query4sql))
										{
											query4.bindValue(":contenttype", ctttype);
											query4.bindValue(":content_disposition", content_disposition);
											if (query4.exec() == false)
											{
												QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=909,dataindex=" + segmentdataid + ",dataoffset=" + QString("%1").arg(datapfpos) + " where id=" + urlid + "";
												rl = query4.exec(query4sql);
											}
										}
										else{
											QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=2,dataindex=" + segmentdataid + ",dataoffset=" + QString("%1").arg(datapfpos) + " where id=" + urlid + "";
											rl = query4.exec(query4sql);
										}
									}
									else{
										QString query4sql = QString() + "update taskurls set  " + addtimesetstr + urlsetstrl + " urlstate=2,dataindex=" + segmentdataid + ",dataoffset=" + QString("%1").arg(datapfpos) + " where id=" + urlid + "";
										rl = query4.exec(query4sql);
									}
									gettaskmutex.Unlock();
									continue;
								}
								else if (downer_result.targetCoentent.size() > 0 && contentlength.toLongLong() == 0)
								{
									updateurlstate(ctttype, query4, addtimesetstr, content_disposition, urlid, "915", urlsetstrl);
									continue;
								}
								else if (contentlength.toLongLong() != 0 && downer_result.targetCoentent.size() != 0 && downer_result.targetCoentent.size() > contentlength.toLongLong())
								{
									updateurlstate(ctttype, query4, addtimesetstr, content_disposition, urlid, "917", urlsetstrl);
									continue;
								}
								else
								{
									updateurlstate(ctttype, query4, addtimesetstr, content_disposition, urlid, "916", urlsetstrl);
									continue;
								}
							}
							else{
								updateurlstate(ctttype, query4, addtimesetstr, content_disposition, urlid, "919", urlsetstrl);
								continue;
							}
						}
						else{
                            QString localhtmlpath = savepath + urlfilename;
							if (contentlength == "" || contentlength.toLongLong() == downer_result.targetCoentent.size())
							{
								QFile ff(localhtmlpath);
								ff.open(QIODevice::WriteOnly);
								ff.write(downer_result.targetCoentent.c_str(), downer_result.targetCoentent.size());
								ff.close();
							}
							else if (downer_result.targetCoentent.size() > 0 && contentlength.toLongLong() == 0)
							{
								QFile ff(localhtmlpath);
								ff.open(QIODevice::WriteOnly);
								ff.write(downer_result.targetCoentent.c_str(), downer_result.targetCoentent.size());
								ff.close();

								updateurlstate(ctttype, query4, addtimesetstr, content_disposition, urlid, "915", urlsetstrl);
								continue;
							}
							else if (contentlength.toLongLong() != 0 && downer_result.targetCoentent.size() != 0 && downer_result.targetCoentent.size() > contentlength.toLongLong())
							{
								QFile ff(localhtmlpath);
								ff.open(QIODevice::WriteOnly);
								ff.write(downer_result.targetCoentent.c_str(), downer_result.targetCoentent.size());
								ff.close();

								updateurlstate(ctttype, query4, addtimesetstr, content_disposition, urlid, "917", urlsetstrl);
								continue;
							}
							else
							{
								QFile ff(localhtmlpath);
								ff.open(QIODevice::WriteOnly);
								ff.write(downer_result.targetCoentent.c_str(), downer_result.targetCoentent.size());
								ff.close();

								updateurlstate(ctttype, query4, addtimesetstr, content_disposition, urlid, "916", urlsetstrl);
								continue;
							}
						}
					}
                    else{
                        auto buf=new char[downer_result.targetCoentent.size()+4096];
                        uLong buflen=downer_result.targetCoentent.size()+4096;
                        gzcompress((Bytef*)downer_result.targetCoentent.data(),downer_result.targetCoentent.size(),(Bytef*)buf,&buflen);

                        g_rockfmutex.lock();
                        int32_t keylen;
                        int32_t vallen;
                        keylen=buflen;
                        vallen=downer_result.targetCoentent.size();
                        char keylenbt[4];
                        qToBigEndian(keylen,(uchar*)keylenbt);
                        char vallenbt[4];
                        qToBigEndian(vallen,(uchar*)vallenbt);
                        g_rockf->write(keylenbt,4);
                        g_rockf->write(url.toUtf8());
                        g_rockf->write(vallenbt,4);
                        g_rockf->write(buf,buflen);
                        g_rockfmutex.unlock();
                        delete[]buf;
					}
				}

				gettaskmutex.Lock();
				if (ctttype.indexOf("text/html") == -1)
				{
					QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=2,contenttype=:contenttype,content_disposition=:content_disposition where id=" + urlid + "";
					if (query4.prepare(query4sql))
					{
						query4.bindValue(":contenttype", ctttype);
						query4.bindValue(":content_disposition", content_disposition);
						if (query4.exec()==false)
						{
							QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=909 where id=" + urlid + "";
							rl = query4.exec(query4sql);
						}
					}
					else{
						QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=2 where id=" + urlid + "";
						rl = query4.exec(query4sql);
					}
				}
				else{
					QString query4sql = QString() + "update taskurls set  " + addtimesetstr + urlsetstrl + " urlstate=2 where id=" + urlid + "";
					rl = query4.exec(query4sql);
				}
				gettaskmutex.Unlock();

			}
		}

	}


}


void DownTaskThread::updateurlstate(QString &ctttype, QSqlQuery &query4, QString addtimesetstr, QString content_disposition, QString urlid, QString urlstsate, QString urlsetstrl)
{
	bool rl = false;
	gettaskmutex.Lock();
	if (ctttype.indexOf("text/html") == -1)
	{
		QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=" + urlstsate + ",contenttype=:contenttype,content_disposition=:content_disposition where id=" + urlid + "";
		if (query4.prepare(query4sql))
		{
			query4.bindValue(":contenttype", ctttype);
			query4.bindValue(":content_disposition", content_disposition);
			if (query4.exec() == false)
			{
				QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=909 where id=" + urlid + "";
				rl = query4.exec(query4sql);
			}
		}
		else{
			QString query4sql = QString() + "update taskurls set " + addtimesetstr + urlsetstrl + " urlstate=" + urlstsate + " where id=" + urlid + "";
			rl = query4.exec(query4sql);
		}
	}
	else{
		QString query4sql = QString() + "update taskurls set  " + addtimesetstr + urlsetstrl + " urlstate=" + urlstsate + " where id=" + urlid + "";
		rl = query4.exec(query4sql);
	}
	gettaskmutex.Unlock();
}

