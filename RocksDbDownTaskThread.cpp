#include "RocksDbDownTaskThread.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <qapplication.h>
#include <QFile>
#include <qvariant.h>
#include <deque>
#include <set>
#include "shlwapi.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QMutex>
#include "CUrlDirectDowner.h"
#include "..\..\UtilityFunc\detecttextcodecname.h"
#include <QTextCodec>
#include "UrlRelinkToLocal.h"
#include <QCryptographicHash>
#include <QRegularExpression>
#include "..\..\UtilityFunc\MMutex.h"
#include "urltofilename.h"

using namespace std;



RocksDbDownTaskThread::RocksDbDownTaskThread(QString savepath, QString inc_ls, QString exc_ls)
{
    mStop = false;
	this->savepath = savepath;
	this->inc_ls = inc_ls;
	this->exc_ls = exc_ls;
}

RocksDbDownTaskThread::~RocksDbDownTaskThread()
{

}



void RocksDbDownTaskThread::run()
{
	//QSqlDatabase db = QSqlDatabase::database("taskdb");

	//QStringList incls = inc_ls.split(";");
	//QStringList excls = exc_ls.split(";");
	//incls.removeDuplicates();
	//excls.removeDuplicates();
	//for (int i = incls.size() - 1; i >= 0; i--)
	//{
	//	incls[i]=incls[i].trimmed();
	//	if (incls[i] == "")
	//	{
	//		incls.removeAt(i);
	//	}
	//}
	//for (int i = excls.size() - 1; i >= 0; i--)
	//{
	//	excls[i] = excls[i].trimmed();
	//	if (excls[i] == "")
	//	{
	//		excls.removeAt(i);
	//	}
	//}
	//QList<QRegularExpression> incre,excre;
	//for (int i = incls.size() - 1; i >= 0; i--)
	//{
	//	incre.push_back(QRegularExpression(incls[i], QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption));
	//}
	//for (int i = excls.size() - 1; i >= 0; i--)
	//{
	//	excre.push_back(QRegularExpression(excls[i], QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption));
	//}

	//int trycnt = 0;
	//while (true)
	//{

	//	QString urlid,url;
	//	bool bgeturl = false;
	//	gettaskmutex.Lock();
	//	QSqlQuery query4(db);
	//	QString query4sql = QString() + "select id,url from  taskurls where urlstate=0 limit 0,1";
	//	bool rl = query4.exec(query4sql);
	//	if (rl)
	//	{
	//		while (query4.next())
	//		{
	//			urlid = query4.value("id").toString();
	//			url = query4.value("url").toString();

	//			QSqlQuery query4(db);
	//			QString query4sql = QString() + "update taskurls set urlstate=1 where id=" + urlid + "";
	//			rl = query4.exec(query4sql);

	//			bgeturl = true;
	//		}
	//	}
	//	gettaskmutex.Unlock();
	//	if (!bgeturl)
	//	{
	//		Sleep(1000);
	//		trycnt++;
	//		if (trycnt > 30)
	//		{
	//			break;
	//		}
	//		else{
	//			continue;
	//		}
	//	}
	//	else{
	//		//download url and relink to local
	//		auto u8url = url.toUtf8();
	//		CUrlDirectorDownOption       downer_opt;
	//		CUrlDirectorDownResult       downer_result;
	//		int							downer_retcode;
	//		downer_opt.httpheaders.push_back("Accept-Encoding: gzip,deflate");
	//		{
	//			downer_retcode = CUrlDirectorDowner(u8url.data(), &downer_opt, &downer_result);
	//		}

	//		if (downer_result.targetHeader == "")
	//		{
	//			QSqlQuery query4(db);
	//			QString query4sql = QString() + "update taskurls set urlstate=3 where id=" + urlid + "";
	//			rl = query4.exec(query4sql);
	//			continue;
	//		}
	//		else
	//		{
	//			if (downer_result.targetHeader.find("text/html") != std::string::npos)
	//			{
	//				std::string codecname = detecttextcodecname(downer_result.targetCoentent.c_str(), downer_result.targetCoentent.size());
	//				auto codc = QTextCodec::codecForName(codecname.c_str());
	//				if (codc == NULL)
	//				{
	//					if (downer_result.targetHeader.find("charset=") != std::string::npos)
	//					{
	//						QString headstr = downer_result.targetHeader.c_str();
	//						QString defcodecname = headstr.mid(headstr.indexOf("charset=") + strlen("charset=")).trimmed();
	//						if (defcodecname.indexOf("\n") != -1)
	//						{
	//							defcodecname = defcodecname.mid(0, defcodecname.indexOf("\n")).trimmed();
	//							codc = QTextCodec::codecForName(defcodecname.toUtf8());
	//						}
	//					}
	//					if (codc == NULL)
	//					{
	//						QSqlQuery query4(db);
	//						QString query4sql = QString() + "update taskurls set urlstate=4 where id=" + urlid + "";
	//						rl = query4.exec(query4sql);
	//						continue;
	//					}
	//				}

	//				QString pagetext = codc->toUnicode(downer_result.targetCoentent.c_str(), downer_result.targetCoentent.size());

	//				std::set<QString> newurl;
	//				QString newpagectt = PageCttRelinkToLocal(pagetext, url, &newurl);


	//				QByteArray ba = QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Md5);
	//				QString md5val = ba.toHex().constData();
	//				QString ext = url;
	//				if (ext.lastIndexOf(".") != -1)
	//					ext = ext.mid(ext.lastIndexOf("."));
	//				else
	//					ext = "";
	//				if (ext.indexOf("?") != -1)
	//					ext = ext.mid(0, ext.indexOf("?"));
	//				if (ext == ".php")ext = ".html";
	//				ext = ext.replace(QRegularExpression("[\\?/\\\\:<>\\|*\"]"), "_").trimmed();
	//				if (ext.size() > 6)ext = ext.mid(0, 6);
	//				QString localhtmlpath = savepath + "/" + md5val + ext;


	//				QFile ff(localhtmlpath);
	//				ff.open(QIODevice::WriteOnly);
	//				ff.write(newpagectt.toUtf8());
	//				ff.close();


	//				db.transaction();
	//				for (auto it = newurl.begin(); it != newurl.end(); it++)
	//				{
	//					bool bpass = incls.size() == 0 ? true : false;
	//					for (int i = incls.size() - 1; i >= 0; i--)
	//					{
	//						if (it->indexOf(incls[i]) != -1 || incre[i].isValid() && incre[i].match(*it).hasMatch())
	//						{
	//							bpass = true;
	//							break;
	//						}
	//					}
	//					if (bpass)
	//					{
	//						for (int i = excls.size() - 1; i >= 0; i--)
	//						{
	//							if (it->indexOf(excls[i]) != -1 || excre[i].isValid() && excre[i].match(*it).hasMatch())
	//							{
	//								bpass = false;
	//								break;
	//							}
	//						}
	//					}

	//					if (bpass)
	//					{
	//						QSqlQuery query4(db);
	//						QString query4sql = QString() + "insert into taskurls(url) values('" + *it + "')";
	//						rl = query4.exec(query4sql);
	//					}
	//				}
	//				db.commit();

	//			}
	//			else
	//			{
	//				QByteArray ba = QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Md5);
	//				QString md5val = ba.toHex().constData();
	//				QString ext = url;
	//				if (ext.lastIndexOf(".") != -1)
	//					ext = ext.mid(ext.lastIndexOf("."));
	//				else
	//					ext = "";
	//				if (ext.indexOf("?") != -1)
	//					ext = ext.mid(0, ext.indexOf("?"));
	//				if (ext == ".php")ext = ".html";
	//				ext = ext.replace(QRegularExpression("[\\?/\\\\:<>\\|*\"]"), "_").trimmed();
	//				if (ext.size() > 6)ext = ext.mid(0, 6);
	//				QString localhtmlpath = savepath + "/" + md5val + ext;


	//				QFile ff(localhtmlpath);
	//				ff.open(QIODevice::WriteOnly);
	//				ff.write(downer_result.targetCoentent.c_str(), downer_result.targetCoentent.size());
	//				ff.close();

	//			}

	//			QSqlQuery query4(db);
	//			QString query4sql = QString() + "update taskurls set urlstate=2 where id=" + urlid + "";
	//			rl = query4.exec(query4sql);

	//		}
	//	}

	//}
}
