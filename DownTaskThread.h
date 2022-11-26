#ifndef DownTaskThread_H
#define DownTaskThread_H

#include <QThread>
#include <cstdint>
#include <QSqlQuery>
#include <QFile>


class DownTaskThread:public QThread
{
	Q_OBJECT
public:
	DownTaskThread(QString savepath, QString inc_ls, QString exc_ls, QString waitsec, QString downsec, QString linkrfsec, QString mainruls, QString rfurlmatches, QString taskintvalmsec, QString useragent, QString jarfile, QString url_ext_relist, QString relink_html_to_local, QString fileexistdonotdown_match, QString savetorocksdb, QString noexistmatch, QString pagenoexistmatch, QString pagetimeoutsec, QString urlfindre, QString combiurlrepre, QString combitimerepre, QString combititlerepre, QString newurlstate, QString urlfindre2, QString combiurlrepre2, QString combitimerepre2, QString combititlerepre2, QString newurlstate2, QString PageGateSrvList, QString PageGateCrawlUrlMatch1, QString PageGateScript1, QString PageGateCrawlUrlMatch2, QString PageGateScript2, QString ExclCtDispList, QString PageFullUrlRestr1, QString PageCttFullReStr1, QString PageFullUrlRestr2, QString PageCttFullReStr2, QString MultiHostUrlReStr, QString MultiHostRepExpr, QString MultiHostNotUrlHeadMatch, QString largefiledowntype, QString reurlstatelist, QString urlfindreoptset, QString autofindlinktype, QString replacetestlsstr, QString memdownmaxsize);
	~DownTaskThread();

	
public slots:
	virtual void	run();
	void Stop(){ mStop = true; }

	void updateurlstate(QString &ctttype, QSqlQuery &query4, QString addtimesetstr, QString content_disposition, QString urlid, QString urlstsate, QString urlsetstrl);
signals:
	void executeResult(uint32_t graphfuncid, QByteArray result);
	void NeedDelThread(QThread *thrd);

private:
	QByteArray executedata;
	QString funcpath, savepath, inc_ls, exc_ls;
    bool mStop;
	uint32_t graphfuncid;
	QString waitsec, downsec;
    int iwaitsec , idownsec , linkrfsec , taskintvalmsec ;
	QString mainruls, rfurlmatches;
	QString useragent, jarfile, url_ext_relist, relink_html_to_local, fileexistdonotdown_match, savetorocksdb, noexistmatch;
	QString pagenoexistmatch;
	int pagetimeoutsec;
	void updateurlstate();

	QString urlfindre, combiurlrepre, combitimerepre, combititlerepre,newurlstate;
	QString urlfindre2, combiurlrepre2, combitimerepre2,combititlerepre2, newurlstate2;
	QString PageGateSrvList, PageGateCrawlUrlMatch1, PageGateScript1, PageGateCrawlUrlMatch2, PageGateScript2, ExclCtDispList;
	QString PageFullUrlRestr1, PageCttFullReStr1, PageFullUrlRestr2, PageCttFullReStr2;
	QString MultiHostUrlReStr, MultiHostRepExpr, MultiHostNotUrlHeadMatch;
	QString largefiledowntype;
	QString reurlstatelist;
	QString urlfindreoptset;
    int autofindlinktype ;
	QString replacetestlsstr;
	QString memdownmaxsize;
	QString segmentdataid;
	
};


#endif


