#include "websiteofflinetool.h"
#include <QFileDialog>
#include <QTextCodec>
#include <QCloseEvent>
#include <QtWidgets/QApplication>
#include <QInputDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <ctime>
#include <QTime>
#include <qendian.h>
#include "DownTaskThread.h"
#include <QCryptographicHash>
#include <QFile>
#include "..\..\UtilityFunc\CreateDirs.h"
#include <QDir>
#include "..\..\UtilityFunc\detecttextcodecname.h"
#include "gzip.h"
#include "CUrlDirectDowner.h"
#include "CurlHttpPostGet.h"
#include "..\..\UtilityFunc\detecttextcodecname.h"
#include "urltofilename.h"
#include <set>
#include "UrlRelinkToLocal.h"
#include <cstdint>
#include "RegexGobalMatchAndCombi.h"
#include "..\..\UtilityFunc\HTML2Text\HtmlToText.h"
#include "..\..\UtilityFunc\qtoolfunc.h"
#include <qendian.h>



extern QString g_langid;
bool g_canuse = true;
bool g_exit = false;
int g_threadnum=0;
//extern rocksdb::DB  *g_rocksdb;
extern QFile *g_rockf;
bool GetFolderAllFilesSpec(QString rootdir, QString finddir, QList<QString>  &alldir, QList<QString>  &subpath_rootpath)
{
	QDir dir(finddir);
	auto dls = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Hidden | QDir::System);
	for (int i = 0; i < dls.size(); i++)
	{
		QString fpath2 = dls[i].absoluteFilePath();
		fpath2 = fpath2.replace("/", "\\");
		alldir.push_back(fpath2);
		GetFolderAllFilesSpec(rootdir, fpath2, alldir, subpath_rootpath);
	}
	auto fls = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::System);
	for (int i = 0; i < fls.size(); i++)
	{
		QString fpath2 = fls[i].absoluteFilePath();
		fpath2 = fpath2.replace("/", QDir::separator());
		fpath2 = fpath2.replace("\\", QDir::separator());
		subpath_rootpath.push_back(fpath2);

	}



	return true;
}





WebsiteOfflineTool::WebsiteOfflineTool(QWidget *parent)
	: QMainWindow(parent)
{
    ui.setupUi(this);




	info = new QLabel;
	statusBar()->addWidget(info);

    threadcnt = 0;
    quittm=0;
    quit = 0;

    if(g_langid=="0"){
        QFile ff(":/help_en.txt");
        ff.open(QIODevice::ReadOnly);
        auto ctt=ff.readAll();
        ff.close();
        ui.textBrowser->setPlainText(ctt);
    }else{
        QFile ff(":/help_cn.txt");
        ff.open(QIODevice::ReadOnly);
        auto ctt=ff.readAll();
        ff.close();
        ui.textBrowser->setPlainText(ctt);
    }


	connect(ui.actionLebb_cc, SIGNAL(triggered()), this, SLOT(actionLebb_cc_triggerred()));
	connect(ui.actionMd5_filename_to_url_filename, SIGNAL(triggered()), this, SLOT(actionMd5_filename_to_url_filenametriggerred()));
	connect(ui.actionMd5_filename_to_url_file_name_without_relink, SIGNAL(triggered()), this, SLOT(actionMd5_filename_to_url_file_name_without_relink()));
	connect(ui.actionHtml_title_to_filename, SIGNAL(triggered()), this, SLOT(actionHtml_title_to_filenametriggerred()));
	connect(ui.startbtn, SIGNAL(clicked()), this, SLOT(startbtnclicked()));

	connect(ui.actionDelete_if_no_exist_url, SIGNAL(triggered()), this, SLOT(actionDelete_if_no_exist_urlclicked()));
	connect(ui.actionRelinkAllFile, SIGNAL(triggered()), this, SLOT(actionRelinkAllFileclicked()));
	connect(ui.actionSave_data_convert_to_rocksdb, SIGNAL(triggered()), this, SLOT(actionSave_data_convert_to_rocksdbclicked()));
	connect(ui.actionReset_no_filedata_url_state, SIGNAL(triggered()), this, SLOT(actionReset_no_filedata_url_stateclicked()));
	connect(ui.actionMin_file_dir_to_rocksdb, SIGNAL(triggered()), this, SLOT(actionMin_file_dir_to_rocksdbclicked()));
	connect(ui.actionDelete_min_file_directory, SIGNAL(triggered()), this, SLOT(actionDelete_min_file_directoryclicked()));
	connect(ui.actionRocksdb_convert_to_file_db, SIGNAL(triggered()), this, SLOT(actionRocksdb_convert_to_file_dbclicked()));
	connect(ui.actionConvert_to_content_dispositon_file_name, SIGNAL(triggered()), this, SLOT(actionConvert_to_content_dispositon_file_nameclicked()));
	//connect(ui.actionConvert_to_content_dispositon_file_name, SIGNAL(triggered()), this, SLOT(action_ted_con_fix_vtt_disposition_content_value()));
	//connect(ui.actionConvert_to_content_dispositon_file_name, SIGNAL(triggered()), this, SLOT(action_ted_con_fix_filename()));
	connect(ui.actionDownload_a_test_url_to_test_html, SIGNAL(triggered()), this, SLOT(actionDownload_a_test_url_to_test_htmlclicked()));
	connect(ui.actionUpdate_urlstate_by_find_content, SIGNAL(triggered()), this, SLOT(actionUpdate_urlstate_by_find_contentclicked()));
	connect(ui.actionDelete_when_urlstate_is_6, SIGNAL(triggered()), this, SLOT(actionDelete_when_urlstate_is_6()));
	//connect(ui.actionExport_title_by_url_regex, SIGNAL(triggered()), this, SLOT(actionExport_title_by_url_regex()));
	//connect(ui.actionName_list_to_utf8_url_list, SIGNAL(triggered()), this, SLOT(actionName_list_to_utf8_url_list()));
	//connect(ui.actionName_list_to_utf8_url_list, SIGNAL(triggered()), this, SLOT(actionXueTutuUrlNotSameCorrect()));
	connect(ui.actionExport_title_by_url_regex, SIGNAL(triggered()), this, SLOT(actionExport_title_by_url_regex_for_xuetutu()));
	connect(ui.actionName_list_to_utf8_url_list, SIGNAL(triggered()), this, SLOT(actionName_list_to_utf8_url_list_for_xuetutu()));
	connect(ui.actionAddtime_from_html_regex, SIGNAL(triggered()), this, SLOT(actionAddtime_from_html_regex()));
	connect(ui.actionUrl_filename_to_url_md5_filename, SIGNAL(triggered()), this, SLOT(actionUrl_filename_to_url_md5_filename()));

	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(actionSaveclicked()));



	connect(ui.tasklisttree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(tasklisttreeDoubleClick(QModelIndex)));

	LoadData();


	connect(ui.buybtn, SIGNAL(clicked()), this, SLOT(buybtnclicked()), Qt::QueuedConnection);
	connect(ui.regbtn, SIGNAL(clicked()), this, SLOT(regbtnclicked()), Qt::QueuedConnection);
	tip = new QLabel();
	ui.statusBar->addWidget(tip);
	QFile ff("regcode");
	ff.open(QIODevice::ReadOnly);
	QByteArray regcode = ff.readAll();
	ff.close();

    g_canuse = true;
		ui.regcodelab->setVisible(false);
		ui.buybtn->setVisible(false);
		ui.regcodeedit->setVisible(false);
		ui.regbtn->setVisible(false);


	ui.regcodelab->setVisible(false);
	ui.buybtn->setVisible(false);
	ui.regcodeedit->setVisible(false);
	ui.regbtn->setVisible(false);
}


WebsiteOfflineTool::~WebsiteOfflineTool()
{

}

void WebsiteOfflineTool::regtip()
{
	/*
	g_canuse = !g_canuse;
	if (g_canuse == false)
	{
		tip->setText(wtr(L"试用已经结束等待下一个5分钟吧"));
		for (int i = 0; i < thrdls.size(); i++)
		{
			thrdls[i]->terminate();
		}
		ui.startbtn->setEnabled(true);
		ui.tasklisttree->setEnabled(true);
	}
	else{
		tip->setText(wtr(L"欢迎使用"));
	}
	*/
}


void WebsiteOfflineTool::quittmtimeout()
{

}


extern DownTaskThread *linkupthrd;
extern uint64_t g_nexttask_time;

void WebsiteOfflineTool::startbtnclicked()
{
	SaveData();

	linkupthrd = 0;
	g_nexttask_time = 0;
	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption;



	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	setWindowTitle(QRegularExpression("([^-]+)", reflag).match(windowTitle()).captured(1) + "-" + curitem1->text());
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);

	auto curitem6 = model->item(ind.row(), 6);
	auto curitem7 = model->item(ind.row(), 7);
	auto curitem8 = model->item(ind.row(), 8);
	auto curitem9 = model->item(ind.row(), 9);
	auto curitem10 = model->item(ind.row(), 10);
	auto curitem11 = model->item(ind.row(), 11);

	auto curitem12 = model->item(ind.row(), 12);
	auto curitem13 = model->item(ind.row(), 13);
	auto curitem14 = model->item(ind.row(), 14);
	auto curitem15 = model->item(ind.row(), 15);
	auto curitem16 = model->item(ind.row(), 16);
	auto curitem17 = model->item(ind.row(), 17);
	auto curitem18 = model->item(ind.row(), 18);
	auto curitem19 = model->item(ind.row(), 19);
	auto curitem20 = model->item(ind.row(), 20);
	auto curitem21 = model->item(ind.row(), 21);

	auto curitem22 = model->item(ind.row(), 22);
	auto curitem23 = model->item(ind.row(), 23);
	auto curitem24 = model->item(ind.row(), 24);
	auto curitem25 = model->item(ind.row(), 25);
	auto curitem26 = model->item(ind.row(), 26);

	auto curitem27 = model->item(ind.row(), 27);
	auto curitem28 = model->item(ind.row(), 28);
	auto curitem29 = model->item(ind.row(), 29);
	auto curitem30 = model->item(ind.row(), 30);
	auto curitem31 = model->item(ind.row(), 31);

	auto curitem32 = model->item(ind.row(), 32);
	auto curitem33 = model->item(ind.row(), 33);
	auto curitem34 = model->item(ind.row(), 34);
	auto curitem35 = model->item(ind.row(), 35);
	auto curitem36 = model->item(ind.row(), 36);
	auto curitem37 = model->item(ind.row(), 37);

	auto curitem38 = model->item(ind.row(), 38);
	auto curitem39 = model->item(ind.row(), 39);
	auto curitem40 = model->item(ind.row(), 40);
	auto curitem41 = model->item(ind.row(), 41);

	auto curitem42 = model->item(ind.row(), 42);
	auto curitem43 = model->item(ind.row(), 43);
	auto curitem44 = model->item(ind.row(), 44);

	auto curitem45 = model->item(ind.row(), 45);

	auto curitem46 = model->item(ind.row(), 46);

	auto curitem47 = model->item(ind.row(), 47);
	auto curitem48 = model->item(ind.row(), 48);
	auto curitem49 = model->item(ind.row(), 49);
	auto curitem50 = model->item(ind.row(), 50);

	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QString waitsec = curitem6->text();
	QString downsec = curitem7->text();

	QString rfurlmatch = curitem8->text();
	QString linkrfsec = curitem9->text();
	QString taskintvalmsec = curitem10->text();
	QString useragent = curitem11->text();

	QString loginlink = curitem12->text();
	QString loginpost = curitem13->text();
	QString loginsucmatch = curitem14->text();
	QString url_ext_relist = curitem15->text();
	QString relink_html_to_local = curitem16->text();
	QString fileexistdonotdown_match = curitem17->text();
	QString savetorocksdb = curitem18->text();
	QString noexistmatch = curitem19->text();
	QString pagenoexistmatch = curitem20->text();
	QString pagetimeoutsec = curitem21->text();

	QString urlfindre = curitem22->text();
	QString combiurlrepre = curitem23->text();
	QString combitimerepre = curitem24->text();
	QString combititlerepre = curitem25->text();
	QString newurlstate = curitem26->text();


	QString urlfindre2 = curitem27->text();
	QString combiurlrepre2 = curitem28->text();
	QString combitimerepre2 = curitem29->text();
	QString combititlerepre2 = curitem30->text();
	QString newurlstate2 = curitem31->text();

	
	QString PageGateSrvList = curitem32->text();
	QString PageGateCrawlUrlMatch1 = curitem33->text();
	QString PageGateScript1 = curitem34->text();
	QString PageGateCrawlUrlMatch2 = curitem35->text();
	QString PageGateScript2 = curitem36->text();

	
	QString ExclCtDispList = curitem37->text();
	if (QFile::exists(ExclCtDispList))
	{
		QFile ff(ExclCtDispList);
		ff.open(QIODevice::ReadOnly);
		auto bind = ff.readAll();
		ff.close();
		auto codename=detecttextcodecname(bind.data(), bind.size());
		QTextCodec *codec = QTextCodec::codecForName(codename.data());
		if (codec != 0)
		{
			ExclCtDispList=codec->toUnicode(bind);
		}
		else{
			info->setText("ContentDispition排除文件 编码错误");
			return;
		}
	}

	QString PageFullUrlRestr1 = curitem38->text();
	QString PageCttFullReStr1 = curitem39->text();
	QString PageFullUrlRestr2 = curitem40->text();
	QString PageCttFullReStr2 = curitem41->text();


	QString MultiHostUrlReStr = curitem42->text();
	QString MultiHostRepExpr = curitem43->text();
	QString MultiHostNotUrlHeadMatch = curitem44->text();
	QString largefiledowntype = curitem45->text();

	QString reurlstatelist = curitem46->text();

	QString urlfindreoptset = curitem47->text();
	QString autofindlinktype = curitem48->text();
	QString replacetestlsstr = curitem49->text();
	QString memdownmaxsizestr = curitem50->text();

	QString mainurlstr = curitem0->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	QString savepath = curitem1->text().replace("/","\\");
	CreateFullDir(savepath.toStdWString().c_str());
	QString jarfile = savepath;
	if (!(jarfile.right(1) == "/" || jarfile.right(1) == "\\"))
	{
		jarfile = jarfile + "/";
		jarfile.replace("\\", "/");
	}
	jarfile += "cookie.jar";
	jarfile = jarfile.replace("/", "\\");

	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{
		db.transaction();
		int rl;
		QSqlQuery query3(db);
		QString query3sql = QString() + "create table if not exists  taskurls (id INTEGER PRIMARY KEY AUTOINCREMENT,url varchar(512) UNIQUE, urlstate INTEGER default 0, addtime TIMESTAMP default current_timestamp, filename varchar(256), contentlength integer, contenttype varchar(24), content_disposition varchar(256),dataindex integer,dataoffset integer)";
		rl = query3.exec(query3sql);
		rl = query3.exec("create index if not exists addtime_index on taskurls(addtime)");
		QString st = query3.lastError().text();


		if (mainurl.size() == 1 && QFile::exists(mainurl[0]))
		{
			db.transaction();
			QFile ff(mainurl[0]);
			ff.open(QIODevice::ReadOnly);
			while (!ff.atEnd())
			{
				QString url=ff.readLine().trimmed();
				if (url.startsWith("http://") || url.startsWith("https://"))
				{
					url = url.mid(0, url.indexOf("/", 8)).toLower() + url.mid(url.indexOf("/", 8));
					QSqlQuery query5(db);
					QString query5sql = QString() + "insert into taskurls(url) values('" + url + "')";
					rl = query5.exec(query5sql);
				}
			}
			db.commit();
			ff.close();
			QSqlQuery query6(db);
			QString query6sql = QString() + "update taskurls set urlstate=0 where urlstate in (1,3)";
			rl = query6.exec(query6sql);

		}
		else{
			QRegularExpression rangeurlre(".*?(\\[(\\d+)-(\\d+)\\]).*", reflag);
			for (int i = 0; i < mainurl.size(); i++)
			{
				if (mainurl[i] == "")continue;
				if (mainurl[0].size() >= 2 && mainurl[0].mid(0, 2) == "--")break;
				if (mainurl[i][0] == '-')continue;
				QString mainu = mainurl[i];
				auto ma = rangeurlre.match(mainu);
				if (ma.hasMatch())
				{
					db.transaction();
					qint64 start = ma.captured(2).toLongLong(), end = ma.captured(3).toLongLong();
					for (qint64 mi = start; mi < end; mi++)
					{
						auto li = mainu.indexOf("[");
						QString realmainurl = mainu;
						realmainurl.replace(li, mainu.lastIndexOf("]") - li + 1, u("%1").arg(mi));
						if (!(realmainurl.startsWith("http://") || realmainurl.startsWith("https://")))
						{
							realmainurl = "http://" + realmainurl;
						}
						if (realmainurl.indexOf("/", 8) == -1)
							realmainurl += "/";
						realmainurl = realmainurl.mid(0, realmainurl.indexOf("/", 8)).toLower() + realmainurl.mid(realmainurl.indexOf("/", 8));
						if (firsturl == "")
							firsturl = realmainurl;
						QSqlQuery query5(db);
						QString query5sql = QString() + "insert into taskurls(url) values('" + realmainurl + "')";
						rl = query5.exec(query5sql);
					}
					db.commit();

					//select max 2 id
					QString likeurl = mainu;
					int st = ma.capturedStart(1), et = ma.capturedEnd(1);
					likeurl.replace(st, et - st, "%");
					QString query4sql = QString() + "select id from  taskurls where url like '" + likeurl+"' and urlstate=2 order by id desc limit 0,1";
					rl = query3.exec(query4sql);
					QString maxlikeid;
					if (rl)
					{
						while (query3.next())
						{
							maxlikeid = query3.value("id").toString();
						}
					}
					query3.clear();
					//update > maxid to 0 and url like range url
					QString query6sql = QString() + "update taskurls set urlstate=0 where url like '" + likeurl + "' and id>" + maxlikeid;
					rl = query3.exec(query6sql);
				}
				else
				{
					if (mainu.trimmed() == "")
					{
						continue;
					}
					if (!(mainu.startsWith("http://") || mainu.startsWith("https://")))
					{
						mainu = "http://" + mainu;
					}
					if (mainu.indexOf("/", 8) == -1)
						mainu += "/";
					mainu = mainu.mid(0, mainu.indexOf("/", 8)).toLower() + mainu.mid(mainu.indexOf("/", 8));
					if (firsturl == "")
						firsturl = mainu;
					QSqlQuery query5(db);
					QString query5sql = QString() + "insert into taskurls(url) values('" + mainu + "')";
					rl = query5.exec(query5sql);
					if (rl == false)//插入失败更新状态
					{
						QSqlQuery query5(db);
						QString query5sql = (QString() + " update taskurls set urlstate=0 where url='%1'").arg(mainu);
						rl = query5.exec(query5sql);
					}
				}
			}

			QSqlQuery query6(db);
			QString query6sql = QString() + "update taskurls set urlstate=0 where urlstate in (1,3,828)";
			rl = query6.exec(query6sql);
		}

		QSqlQuery query6(db);
		QString query6sql = QString() + "update taskurls set urlstate=0 where urlstate=1";
		rl = query6.exec(query6sql);

		QSqlQuery query4(db);
		QString query4sql = QString() + "create table if not exists  taskurlshis(id INTEGER PRIMARY KEY AUTOINCREMENT,url varchar(512) UNIQUE, addtime TIMESTAMP default current_timestamp, filename varchar(256), contentlength integer, contenttype varchar(24), content_disposition varchar(256)  )";
		rl = query4.exec(query4sql);
		rl = query3.exec("create index if not exists addtime_index on taskurls(addtime)");

		db.commit();
	}


	QByteArray ba = QCryptographicHash::hash(firsturl.toUtf8(), QCryptographicHash::Md5);
	QString md5val = ba.toHex().constData();
	QFile ff(savepath + "/index_is_" + md5val);
	ff.open(QIODevice::WriteOnly);
	ff.close();

	if (threadnum == "")threadnum = "3";
	int tnum = threadcnt = threadnum.toInt();

	//login
	if (loginlink != "")
	{
		std::map<std::string, std::string> postdata;
		QStringList postls=loginpost.split(";");
		for (int i = 0; i < postls.size(); i++)
		{
			QStringList segls=postls[i].split("=");
			if (segls.size() == 2)
			{
				postdata.insert(std::make_pair(segls[0].toUtf8().data(), segls[1].toUtf8().data()));
			}
		}

		CUrlDirectorDownResult downer_result;
        int rl;
        rl= CurlHttpGet(loginlink.toStdString(), &postdata, &downer_result, 0, 15, useragent.toStdString(), jarfile.toStdString());
		std::string codecname = detecttextcodecname(downer_result.targetCoentent.data(), downer_result.targetCoentent.size());
		auto codc = QTextCodec::codecForName(codecname.c_str());
		if (codc == NULL)
		{
			QMessageBox msg;
			msg.setText(u("编码识别错误"));
			msg.exec();
			return;
		}
		QString getresult2 = codc->toUnicode(downer_result.targetCoentent.data(), downer_result.targetCoentent.size());
		QRegularExpression rlre(loginsucmatch, reflag);
		if (!rlre.match(getresult2).hasMatch())
		{
			QMessageBox msg;
			msg.setText(u("登录错误错误"));
			msg.exec();
			return;
		}
	}

    //try recover rockdb file
    QFile lastft(savepath+"urldata_rocksdb.lastflushpos");
    lastft.open(QIODevice::ReadOnly);
    auto lastflusht=lastft.readAll();
    lastft.close();
    if(lastflusht==""){
        QFile lastft(savepath+"urldata_rocksdb.lastflushposbak");
        lastft.open(QIODevice::ReadOnly);
        lastflusht=lastft.readAll();
        lastft.close();
    }
    if(lastflusht!=""){
        HANDLE hf=CreateFile((savepath+"urldata_rocksdb").toStdWString().data(),GENERIC_ALL,0,0,OPEN_EXISTING,0,0);
        if(hf!=nullptr && hf!=INVALID_HANDLE_VALUE){
            auto ll=QString(lastflusht).toLongLong();
            auto ll_hi=LONG(ll>>32);
            SetFilePointer(hf,LONG(ll&0xFFFFFFFF),&ll_hi,FILE_BEGIN);
            DWORD rndcnt;
            auto buf=new char[30*1024*1024];
            if(ReadFile(hf,buf,30*1024*1024,&rndcnt,0)){
                int curi=0;
                while(true){
                    if(curi+4>rndcnt || curi==rndcnt){
                        break;
                    }
                    auto keylen=qFromBigEndian<quint32>((uchar*)buf+curi);
                    if(curi+4+keylen+4>rndcnt){
                        break;
                    }
                    auto vallen=qFromBigEndian<quint32>((uchar*)buf+curi+4+keylen);
                    if(curi+4+keylen+4+vallen>rndcnt){
                        break;
                    }
                    curi+=4+keylen+4+vallen;
                }
                if(curi!=rndcnt){
                    ll+=curi;
                    auto ll_hi=LONG(ll>>32);
                    SetFilePointer(hf,LONG(ll&0xFFFFFFFF),&ll_hi,FILE_BEGIN);
                    SetEndOfFile(hf);
                    FlushFileBuffers(hf);

                    QFile lastftf(savepath+"urldata_rocksdb.lastflushpos");
                    QFile::remove(savepath+"urldata_rocksdb.lastflushposbak");
                    QFile::rename(savepath+"urldata_rocksdb.lastflushpos",savepath+"urldata_rocksdb.lastflushposbak");
                    lastftf.open(QIODevice::WriteOnly);
                    lastftf.write(u("%1").arg(ll).toUtf8());
                    lastftf.close();
                }
            }
            delete[]buf;
            CloseHandle(hf);
        }
    }

    savepath=savepath.replace("\\","/");
    savepath=savepath.replace("//","/");
    savepath=savepath.replace("//","/");
    savepath=savepath.replace("//","/");
    if(!savepath.endsWith("/")){
        savepath+="/";
    }

    linkupthrd = 0;

	g_exit = false;
	thrdls.clear();
    g_threadnum=tnum;
	for (int i = 0; i < tnum; i++)
	{
		DownTaskThread *thrd = new DownTaskThread(savepath, inc_ls, exc_ls, waitsec, downsec, linkrfsec, mainurlstr, rfurlmatch, taskintvalmsec, useragent, jarfile, url_ext_relist, relink_html_to_local, fileexistdonotdown_match, savetorocksdb, noexistmatch, pagenoexistmatch, pagetimeoutsec, urlfindre, combiurlrepre, combitimerepre, combititlerepre, newurlstate, urlfindre2, combiurlrepre2, combitimerepre2, combititlerepre2, newurlstate2, PageGateSrvList, PageGateCrawlUrlMatch1, PageGateScript1.replace("\\r\\n", "\n"), PageGateCrawlUrlMatch2, PageGateScript2.replace("\\r\\n", "\n"), ExclCtDispList, PageFullUrlRestr1, PageCttFullReStr1, PageFullUrlRestr2, PageCttFullReStr2, MultiHostUrlReStr, MultiHostRepExpr, MultiHostNotUrlHeadMatch, largefiledowntype, reurlstatelist, urlfindreoptset, autofindlinktype, replacetestlsstr, memdownmaxsizestr);
		connect(thrd, SIGNAL(finished()), this, SLOT(threadfinished()));
		thrd->start();
		thrdls.append(thrd);
	}

	ui.startbtn->setEnabled(false);
	ui.tasklisttree->setEnabled(false);

	info->setText(u("start ok."));
}

void WebsiteOfflineTool::threadfinished()
{
	threadcnt -= 1;
	if (threadcnt == 0)
	{
		thrdls.clear();
		ui.startbtn->setEnabled(true);
		ui.tasklisttree->setEnabled(true);

		QSqlDatabase db = QSqlDatabase::database("taskdb");
		db.close();

        if (g_rockf != nullptr)
		{
            g_rockf->close();
            g_rockf= nullptr;
		}

		if (g_exit == true)
		{
			qApp->quit();
		}
	}
}


void WebsiteOfflineTool::SaveData()
{
    QFile ff2("WebsiteOfflineTool.cfg");
    ff2.open(QIODevice::ReadOnly);
    QString ff2ctt = ff2.readAll();
    ff2.close();

    QStringList ff2ls = ff2ctt.trimmed().split("\n");

	auto model = ui.tasklisttree->model();
    auto curind=ui.tasklisttree->currentIndex();
    QString cfgdata;
    for(int i=0;i<ff2ls.size();i++){
        auto linels=ff2ls[i].split("`%");
        if(curind.row()==i){
            QString linedata;
            for (int j = 0; j < model->columnCount(); j++)
            {
                auto item = model->item(curind.row(), j);
                if (item != NULL)
                {
                    linedata += item->text().trimmed() + "`%";
                }else{
                    linedata += wtr(L"") + "`%";
                }
            }
            cfgdata += linedata + "\n";
        }else{
            cfgdata += ff2ls[i] + "\n";
        }
    }

	cfgdata = cfgdata.trimmed();
    QFile::remove("WebsiteOfflineTool_bak6.cfg");
    QFile::rename("WebsiteOfflineTool5.cfg", "WebsiteOfflineTool_bak6.cfg");
    QFile::rename("WebsiteOfflineTool4.cfg", "WebsiteOfflineTool_bak5.cfg");
    QFile::rename("WebsiteOfflineTool3.cfg", "WebsiteOfflineTool_bak4.cfg");
	QFile::rename("WebsiteOfflineTool2.cfg", "WebsiteOfflineTool_bak3.cfg");
	QFile::rename("WebsiteOfflineTool.cfg", "WebsiteOfflineTool_bak2.cfg");
	QFile ff("WebsiteOfflineTool.cfg");
	ff.open(QIODevice::WriteOnly);
	ff.write(cfgdata.toUtf8());
	ff.close();
}
void WebsiteOfflineTool::LoadData()
{
	auto model = ui.tasklisttree->model();
	QFile ff("WebsiteOfflineTool.cfg");
	ff.open(QIODevice::ReadOnly);
    QString ffctt = ff.readAll();
	ff.close();

	QStringList ffls = ffctt.trimmed().split("\n");
	for (int i = 0; i < ffls.size(); i++)
	{
		ffls[i] = ffls[i].trimmed();
		if (ffls[i].size()>0)
		{
            QStringList rowls = ffls[i].split("`%");
			if (rowls[1].trimmed() != "")
			{
				QList<QStandardItem*> row;
				for (int j = 0; j < rowls.size() - 1; j++)
				{
					row.append(new QStandardItem(rowls[j]));
				}
                model->insertRow(i, row);
			}
		}
	}
}

void WebsiteOfflineTool::actionLebb_cc_triggerred()
{
	ShellExecute(0, L"open", L"http://www.lebb.cc/", 0, 0, SW_SHOW);
}

void WebsiteOfflineTool::actionMd5_filename_to_url_filenametriggerred()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext,res);
		}
	}

	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());

	QString urlrestr=QInputDialog::getText(0, u("regex of process url"), u("regex"),QLineEdit::Normal,".*");


	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{
		QRegularExpression urlre(urlrestr, reflag);
		QMap<QString, QString> urlfilename_md5filename,md5fnmap;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls";
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();
				if (urlre.match(url).hasMatch())
				{
					QString filename = url.mid(url.lastIndexOf("/") + 1);
					QString fileext;
					if (filename.lastIndexOf(".") != -1)
						fileext = filename.mid(filename.lastIndexOf(".")).toLower();
					if (!(filename == ""))
					{
						if (urlfilename_md5filename.find(filename) != urlfilename_md5filename.end())
						{
							QString filename2 = filename;
							for (int filei = 1; filei < 1000000; filei++)
							{
								if (filename.lastIndexOf(".") != -1)
								{
									filename2 = filename;
									filename2.replace(filename2.lastIndexOf("."), 1, wtr(L"_%1.").arg(filei));
								}
								else{
									filename2 = filename + wtr(L"_%1").arg(filei);
								}
								if (urlfilename_md5filename.find(filename2) == urlfilename_md5filename.end())
								{
									break;
								}
							}
							filename = filename2;
						}
						filename = filename.replace(QRegularExpression("[\\?/\\\\:<>\\|*]"), "_").trimmed();

						if (url == "https://www.khronos.org/registry/OpenCL/sdk/2.1/docs/man/xhtml/")
							int e = 23434;


						QString urlfilename = urltofilename(url, &urlre_ext);
						urlfilename_md5filename.insert(filename, urlfilename);
						md5fnmap.insert(urlfilename, "");
					}
				}
			}
		}

		//replace ctt and rename
		QMap<QString, QString>    htmlfnmap;
		QList<QString>  allfile, alldir;
		QString rootdir = savepath;
		GetFolderAllFilesSpec(savepath, savepath, alldir, allfile);
		alldir.sort();

		for (int i = 0; i < allfile.size(); i++)
		{
			QString filename=allfile[i].mid(allfile[i].lastIndexOf(QRegularExpression("[/\\\\]")) + 1);
			if (filename.indexOf("4043e370712c5ed04ed4b23518d33323") != -1)
				int e = 3243243;
			if (md5fnmap.find(filename) != md5fnmap.end())
			{
				QFile ff(allfile[i]);
				if (ff.open(QIODevice::ReadOnly))
				{
					QByteArray ffctt = ff.readAll();
					ff.close();

					std::string codecname = detecttextcodecname(ffctt.data(), ffctt.size());
					if (codecname == "UTF-8")
					{
						int tst = ffctt.indexOf("<title>"), ted = ffctt.indexOf("</title>");
						if (tst == -1 || ted == -1)
						{
							tst = ffctt.indexOf("<TITLE>");
							ted = ffctt.indexOf("</TITLE>");
						}
						if (tst != -1 && ted != -1)
						{
							for (auto it = urlfilename_md5filename.begin(); it != urlfilename_md5filename.end(); it++)
							{
								ffctt = ffctt.replace(it.value().toUtf8(), it.key().toUtf8());
								if (ffctt.indexOf(it.key().toUtf8())!=-1)
									htmlfnmap.insert(it.value().toUtf8(), "");
							}
							QFile ff2(allfile[i]);
							if (ff2.open(QIODevice::WriteOnly))
							{
								ff2.write(ffctt);
								ff2.close();
							}
							
						}
					}
				}
			}
		}

		//rename 
		for (auto it = urlfilename_md5filename.begin(); it != urlfilename_md5filename.end(); it++)
		{
			if (htmlfnmap.find(it.value()) != htmlfnmap.end())
			{
				bool brl = QFile::rename(savepath + "/" + it.value(), savepath + "/" + it.key());
				//auto brl=MoveFile(it.value().toStdWString().c_str(), it.key().toStdWString().c_str());
				//DWORD err = GetLastError();
				int e = 324324;
			}
		}


		db.close();
	}
}

QString getUrlFileName(QString savepath, QString url)
{
	QString filename = url.mid(url.lastIndexOf("/") + 1);
	if (filename != "")
	{
		auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
		QRegularExpression pere("%[0-9A-Fa-f][0-9A-Fa-f]", reflag);
		QString filename2 = filename;
		if (QFile::exists(savepath + "/" + filename2))
		{
			for (int filei = 1; filei < 1000000; filei++)
			{
				if (filename.lastIndexOf(".") != -1)
				{
					filename2 = filename;
					filename2.replace(filename2.lastIndexOf("."), 1, wtr(L"_%1.").arg(filei));
				}
				else{
					filename2 = filename + wtr(L"_%1").arg(filei);
				}
				if (!QFile::exists(savepath + "/" + filename2))
				{
					break;
				}
			}
		}
		filename = filename2;

		filename = filename.replace(QRegularExpression("[\\?/\\\\:<>\\|*]"), "_").trimmed();

		if (pere.match(filename).hasMatch())
		{
			filename = QByteArray::fromPercentEncoding(filename.toUtf8());
		}
		return filename;
	}
	return "";
}

void WebsiteOfflineTool::actionMd5_filename_to_url_file_name_without_relink()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext,res);
		}
	}

	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());

	QString urllikestr = QInputDialog::getText(0, u("url sql like"), u("url sql like"), QLineEdit::Normal, "%");


	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{
		QMessageBox msg;
		msg.setText(u("文件不存在是否改变任务状态到0?"));
		msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		auto noexistchangestate = msg.exec();
		if (noexistchangestate == QMessageBox::Cancel)
		{
			return;
		}

		QMap<QString, QString> urlfilename_md5filename,md5fnmap;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls where urlstate=2 and url like '"+urllikestr+"'";
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			QRegularExpression pere("%[0-9A-Fa-f][0-9A-Fa-f]", reflag);
			db.transaction();
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();
				QString oldfilename = urltofilename(url, &urlre_ext);
				if (QFile::exists(savepath + "/" + oldfilename))
				{
					QString urlfilename=getUrlFileName(savepath,url);
					if (urlfilename != "")
					{
						QFile::rename(savepath + "/" + oldfilename, savepath + "/" + urlfilename);
					}
				}
				else{
					if (noexistchangestate == QMessageBox::Yes)
					{
						QSqlQuery ddd(db);
						ddd.exec("update taskurls set urlstate=0 where id=" + urlid);
					}
				}
			}
			db.commit();
		}

		db.close();
	}
}


void WebsiteOfflineTool::actionHtml_title_to_filenametriggerred()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);

	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());

	bool bnamelimit = false;
	if (GetKeyState(VK_CONTROL) < 0)
		bnamelimit = true;

	//QString urlrestr = QInputDialog::getText(0, u("regex of process url"), u("regex"), QLineEdit::Normal, ".*");

	QMap<QString, QString> urlfilename_md5filename, md5fnmap, htmlfnmap;

	//replace ctt and rename
	QList<QString>  allfile, alldir;
	QString rootdir = savepath;
	GetFolderAllFilesSpec(savepath, savepath, alldir, allfile);
	alldir.sort();

	for (int i = 0; i < allfile.size(); i++)
	{
		QString filename = allfile[i].mid(allfile[i].lastIndexOf(QRegularExpression("[/\\\\]")) + 1);
		QString purefilename = filename;
		if (purefilename.indexOf(".") != -1)
			purefilename = purefilename.mid(0, purefilename.indexOf("."));
		QString fileext;
		if (allfile[i].lastIndexOf(".") != -1)
			fileext = allfile[i].mid(allfile[i].lastIndexOf(".")).toLower();
		bool bnamepass = true;
		if (bnamelimit)
		{
			if (purefilename.size() != 32)
				bnamepass = false;
		}
		if (bnamepass && !(allfile[i] == "" || fileext == ".jpg" || fileext == ".png" || fileext == ".gif" || fileext == ".bmp" || fileext == ".ico" || fileext == ".svg" || fileext == ".mp3" || fileext == ".mp4" || fileext == ".ogg" || fileext == ".wav" || fileext == ".pdf" || fileext == ".js" || fileext == ".css" || fileext == ".bson" || fileext == ".tiff" || fileext == ".pnm" || fileext == ".db"))
		{
			QFile ff(allfile[i]);
			if (ff.open(QIODevice::ReadOnly))
			{
				QByteArray ffctt = ff.readAll();
				ff.close();
				std::string codecname = detecttextcodecname(ffctt.data(), ffctt.size());
				if (codecname == "UTF-8")
				{
					int tst = ffctt.indexOf("<title>"), ted = ffctt.indexOf("</title>");
					if (tst == -1 || ted == -1)
					{
						tst = ffctt.indexOf("<TITLE>");
						ted = ffctt.indexOf("</TITLE>");
					}
					if (tst != -1 && ted != -1)
					{
						QString title = ffctt.mid(tst + strlen("<title>"), ted - tst - strlen("<title>"));
						QString titlefilename;
						if (allfile[i].lastIndexOf(".") != -1)
						{
							titlefilename = title + allfile[i].mid(allfile[i].lastIndexOf("."));
						}
						else{
							titlefilename = title;
						}
						titlefilename = titlefilename.replace(QRegularExpression("[\\?/\\\\:<>\\|*]"), "_").trimmed();
						if (urlfilename_md5filename.find(titlefilename) != urlfilename_md5filename.end())
						{
							QString titlefilename2 = titlefilename;
							for (int filei = 1; filei < 1000000; filei++)
							{
								if (titlefilename.lastIndexOf(".") != -1)
								{
									titlefilename2 = titlefilename;
									titlefilename2.replace(titlefilename2.lastIndexOf("."), 1, wtr(L"_%1.").arg(filei));
								}
								else{
									titlefilename2 = titlefilename + wtr(L"_%1").arg(filei);
								}
								if (urlfilename_md5filename.find(titlefilename2) == urlfilename_md5filename.end())
								{
									break;
								}
							}
							titlefilename = titlefilename2;
						}
						urlfilename_md5filename.insert(titlefilename, filename);
						md5fnmap.insert(allfile[i].mid(allfile[i].lastIndexOf(QRegularExpression("[/\\\\]")) + 1), "");
					}
				}
			}
		}
	}

	for (int i = 0; i < allfile.size(); i++)
	{
		QString filename = allfile[i].mid(allfile[i].lastIndexOf(QRegularExpression("[/\\\\]")) + 1);
		if (md5fnmap.find(filename) != md5fnmap.end())
		{
			QFile ff(allfile[i]);
			if (ff.open(QIODevice::ReadOnly))
			{
				QByteArray ffctt = ff.readAll();
				ff.close();
				for (auto it = urlfilename_md5filename.begin(); it != urlfilename_md5filename.end(); it++)
				{
					ffctt = ffctt.replace(it.value().toUtf8(), it.key().toUtf8());
				}
				QFile ff2(allfile[i]);
				if (ff2.open(QIODevice::WriteOnly))
				{
					ff2.write(ffctt);
					ff2.close();
				}
				htmlfnmap.insert(filename, "");
			}
		}
	}

	//rename 
	for (auto it = urlfilename_md5filename.begin(); it != urlfilename_md5filename.end(); it++)
	{
		if (htmlfnmap.find(it.value()) != htmlfnmap.end())
		{
			bool brl = QFile::rename(savepath + "/" + it.value(), savepath + "/" + it.key());
			//auto brl=MoveFile(it.value().toStdWString().c_str(), it.key().toStdWString().c_str());
			//DWORD err = GetLastError();
			int e = 324324;
		}
	}

}

void WebsiteOfflineTool::buybtnclicked()
{
    //std::string url = "http://www.lebb.cc/lebbmall.php?id=19&mc=" + getmachinecode();
    //ShellExecuteA(NULL, "open", url.c_str(), 0, 0, SW_SHOW);
}

void WebsiteOfflineTool::regbtnclicked()
{
	QString regcode = ui.regcodeedit->text();
	QFile ff("regcode");
	ff.open(QIODevice::WriteOnly);
	ff.write(regcode.toUtf8());
	ff.close();
}

void WebsiteOfflineTool::actionDelete_if_no_exist_urlclicked()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString,QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext,res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());


	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{

		QMap<QString, QString> urlfilename_md5filename, md5fnmap;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls";
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();

				havurl_filenameset.insert(urltofilename(url, &urlre_ext));
				bdbsuc = true;
			}
		}

		db.close();
	}


	//delete no url file
	QDir savdir(savepath);
	auto allfile=savdir.entryList(QDir::Files);

	if (bdbsuc == true)
	{
		QRegularExpression md5re("[a-zA-Z0-9]{32}");
		for (int i = 0; i < allfile.size(); i++)
		{
			QString downfile = allfile[i];
			if (downfile.indexOf(".") == 32)
			{
				QString md5val = downfile.mid(0, 32);
				if (md5re.match(md5val).hasMatch())
				{
					if (!havurl_filenameset.contains(downfile))
					{
						QFile::remove(savepath+"/"+downfile);
					}
				}
			}
		}
	}


	
}


void WebsiteOfflineTool::actionRelinkAllFileclicked()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}

	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());

	bool bnamelimit = false;
	if (GetKeyState(VK_CONTROL) < 0)
		bnamelimit = true;


	QMap<QString, QString> urlfilename_md5filename, md5fnmap, htmlfnmap;

	//replace ctt and rename
	QList<QString>  allfile, alldir;
	QString rootdir = savepath;
	GetFolderAllFilesSpec(savepath, savepath, alldir, allfile);
	alldir.sort();



	//
	QStringList incls = inc_ls.split(";"), metaincls;
	QStringList excls = exc_ls.split(";"), metaexcls;
	incls.removeDuplicates();
	excls.removeDuplicates();
	for (int i = incls.size() - 1; i >= 0; i--)
	{
		incls[i] = incls[i].trimmed();
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
	for (int i = excls.size() - 1; i >= 0; i--)
	{
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



	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else
	{
		QMap<QString, QString> urlfilename_md5filename, md5fnmap;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls";
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();

				QString oldfilename = urltofilename(url, &urlre_ext);

				if (QFile::exists(savepath + "/" + oldfilename))
				{
					QFile ff(savepath + "/" + oldfilename);
					ff.open(QIODevice::ReadOnly);
					QString pagetext = ff.readAll();
					ff.close();

					std::set<QString> newurl;
					QString newpagectt = PageCttRelinkToLocal(pagetext, url, &newurl, incls, excls, incre, excre, &urlre_ext);

					QFile ff2(savepath + "/" + oldfilename);
					ff2.open(QIODevice::WriteOnly);
					ff2.write(newpagectt.toUtf8());
					ff2.close();
				}
			}
		}
		db.close();
	}

}

void WebsiteOfflineTool::actionSave_data_convert_to_rocksdbclicked()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);
	auto curitem18 = model->item(ind.row(), 18);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());



	//
	int noexistdeal = QMessageBox::Yes;
	if (QDir(savepath + "\\urldata_rocksdb").exists())
	{
		QMessageBox msg;
		msg.setText(u("是否改变任务状态当文件不存在?"));
		msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		noexistdeal = msg.exec();
		if (noexistdeal == QMessageBox::Cancel)
		{
			return;
		}
	}

	QMessageBox msg;
	msg.setText(u("转换过程中是否删除条目文件?"));
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	auto delwhenconvert=msg.exec();
	if (delwhenconvert == QMessageBox::Cancel)
	{
		return;
	}

	//
    QFile *dbhand = new QFile(savepath + "\\urldata_rocksdb");
    dbhand->open(QIODevice::WriteOnly);
	if (dbhand == 0)
	{
		QMessageBox msg;
		msg.setText(u("打开数据库失败"));
		msg.exec();
		return;
	}

	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{

		QMap<QString, QString> urlfilename_md5filename, md5fnmap;
		QList<int>   needtostate0id;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls where urlstate=2";
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();
				QString filename = urltofilename(url, &urlre_ext);
				auto urlu8 = url.toUtf8();
				bool btordb = false;
				QString fpa = savepath + "/" + filename;
				if (QFile::exists(fpa))
				{
					QFile ff(fpa);
					if (ff.open(QIODevice::ReadOnly))
					{
						auto ffctt = ff.readAll();
						ff.close();

                        int32_t keylen;
                        int32_t vallen;
                        keylen=urlu8.size();
                        vallen=ffctt.size();
                        char keylenbt[4];
                        qToBigEndian(keylen,(uchar*)keylenbt);
                        char vallenbt[4];
                        qToBigEndian(vallen,(uchar*)vallenbt);
                        dbhand->write(keylenbt,4);
                        dbhand->write(urlu8);
                        dbhand->write(vallenbt,4);
                        dbhand->write(ffctt);

						btordb = true;
						if (delwhenconvert == QMessageBox::Yes)
						{
							QFile::remove(fpa);
						}
					}
				}else
				{
					if (noexistdeal == QMessageBox::Yes)
					{
						needtostate0id.push_back(urlid.toUInt());
					}
				}
			}
		}

		db.transaction();
		for (int i = 0; i < needtostate0id.size(); i++)
		{
			QSqlQuery query4(db);
			query4.exec(u("update taskurls set urlstate=0 where id=%1").arg(needtostate0id[i]));
		}
		db.commit();

		db.close();
	}

	if (dbhand)
	{
        dbhand->close();
		dbhand = 0;
	}


	curitem18->setText("1");
	SaveData();
}

void WebsiteOfflineTool::actionReset_no_filedata_url_stateclicked()
{
    /*
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);
	auto curitem18 = model->item(ind.row(), 18);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QString savetorocksdb=curitem18->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());



	//
	rocksdb::DB *dbhand = 0;
	dbhand = rocksdbopen(savepath + "\\urldata_rocksdb");


	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{

		QMap<QString, QString> urlfilename_md5filename, md5fnmap;
		QList<int>   needtostate0id;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls where urlstate=2";
		bool rl = query4.exec(query4sql);
		int urlcount = 0;
		if (rl)
		{
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();
				QString filename = urltofilename(url, &urlre_ext);
				urlcount++;

				if (savetorocksdb != "3")
				{
				}
				else if (savetorocksdb != "1")
				{
					bool btordb = false;
					QString fpa = savepath + "/" + filename;
					if (QFile::exists(fpa))
					{

					}
					else
					{
						if (dbhand != 0)
						{
							auto urlu8 = url.toUtf8();
							std::string value;
							auto rl = dbhand->Get(rocksdb::ReadOptions(), std::string(urlu8.data(), urlu8.size()), &value);
							if (!rl.ok())
							{
								needtostate0id.push_back(urlid.toUInt());
							}
						}
						else{
							needtostate0id.push_back(urlid.toUInt());
						}
					}
				}
				else{
					auto urlu8 = url.toUtf8();
					std::string value;
					auto rl = dbhand->Get(rocksdb::ReadOptions(), std::string(urlu8.data(), urlu8.size()), &value);
					if (!rl.ok())
					{
						needtostate0id.push_back(urlid.toUInt());
					}
				}
			}
		}

		db.transaction();
		for (int i = 0; i < needtostate0id.size(); i++)
		{
			QSqlQuery query4(db);
			query4.exec(u("update taskurls set urlstate=0 where id=%1").arg(needtostate0id[i]));
		}
		db.commit();

		db.close();
	}

	if (dbhand)
	{
		delete dbhand;
		dbhand = 0;
	}
    */
}


void WebsiteOfflineTool::tasklisttreeDoubleClick(QModelIndex ind)
{
	if (!ind.isValid())return;

	if (ind.column() == 1)
	{
		QString path = ui.tasklisttree->model()->item(ind.row(), 1)->text();
		ShellExecute(0, L"open", path.toStdWString().data(), 0, 0, SW_SHOW);
	}
}


void WebsiteOfflineTool::closeEvent(QCloseEvent * event)
{
	if (thrdls.size())
	{
		g_exit = true;
		event->ignore();
	}
}

void WebsiteOfflineTool::actionMin_file_dir_to_rocksdbclicked()
{
	//root dir
	QString savepath = QInputDialog::getText(0, u("minium file dir input"), u("dir path"));


	QMessageBox msg;
	msg.setText(u("转换过程中是否删除条目文件?"));
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	auto delwhenconvert = msg.exec();
	if (delwhenconvert == QMessageBox::Cancel)
	{
		return;
	}

	//
    QFile *dbhand = new QFile(savepath + "\\urldata_rocksdb");
    dbhand->open(QIODevice::WriteOnly);
	if (dbhand == 0)
	{
		QMessageBox msg;
		msg.setText(u("打开数据库失败"));
		msg.exec();
		return;
	}

	//
	QDir dir(savepath);
	auto minfilels=dir.entryList(QDir::Files);
	for (int i = 0; i < minfilels.size(); i++)
	{
		QString fpa = savepath + "\\" + minfilels[i];
		if (QFile::exists(fpa))
		{
			QFile ff(fpa);
			if (ff.open(QIODevice::ReadOnly))
			{
				auto ffctt = ff.readAll();
				ff.close();
				auto filenameu8 = minfilels[i].toUtf8();


                int32_t keylen;
                int32_t vallen;
                keylen=filenameu8.size();
                vallen=ffctt.size();
                char keylenbt[4];
                qToBigEndian(keylen,(uchar*)keylenbt);
                char vallenbt[4];
                qToBigEndian(vallen,(uchar*)vallenbt);
                dbhand->write(keylenbt,4);
                dbhand->write(filenameu8);
                dbhand->write(vallenbt,4);
                dbhand->write(ffctt);


				if (delwhenconvert == QMessageBox::Yes)
				{
					QFile::remove(fpa);
				}
			}
		}
	}

	if (dbhand)
	{
        dbhand->close();
		dbhand = 0;
	}
}


void WebsiteOfflineTool::actionDelete_min_file_directoryclicked()
{
	//root dir
	QString savepath = QInputDialog::getText(0, u("minium file dir input"), u("dir path"));

	QString fileregex = QInputDialog::getText(0, u("file regex for delete"), u("file regex for delete"), QLineEdit::Normal,".*");


	QMessageBox msg;
	msg.setText(u("所有这个目录下满足匹配的文件将被删除,确定要进行吗?"));
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	auto delwhenconvert = msg.exec();
	if (delwhenconvert == QMessageBox::Cancel)
	{
		return;
	}

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption;
	QRegularExpression pathre(fileregex, reflag);
	//
	QDir dir(savepath);
	auto minfilels = dir.entryList(QDir::Files);
	for (int i = 0; i < minfilels.size(); i++)
	{
		if (pathre.match(minfilels[i]).hasMatch())
		{
			QString fpa = savepath + "\\" + minfilels[i];
			QFile::remove(fpa);
		}
	}

}

void WebsiteOfflineTool::actionRocksdb_convert_to_file_dbclicked()
{
    /*
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);
	auto curitem18 = model->item(ind.row(), 18);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QString savetorocksdb = curitem18->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());



	//
    QFile *dbhand = new QFile(savepath + "\\urldata_rocksdb");
    dbhand->open(QIODevice::WriteOnly);

	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else
	{
		QMap<QString, QString> urlfilename_md5filename, md5fnmap;
		QList<int>   needtostate0id;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls where urlstate=2";
		bool rl = query4.exec(query4sql);
		int urlcount = 0;
		if (rl)
		{
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();
				QString filename = urltofilename(url, &urlre_ext);
				urlcount++;

				if (dbhand != 0)
				{
					auto urlu8 = url.toUtf8();
					std::string value;
					auto rl = dbhand->Get(rocksdb::ReadOptions(), std::string(urlu8.data(), urlu8.size()), &value);
					if (rl.ok())
					{
						QFile ff(savepath + "/" + filename);
						ff.open(QIODevice::WriteOnly);
						ff.write(value.data(),value.size());
						ff.close();
					}
				}
			}
		}


		db.close();
	}

	if (dbhand)
	{
		delete dbhand;
		dbhand = 0;
	}
    */
}


void WebsiteOfflineTool::actionSaveclicked()
{
	SaveData();
}

void WebsiteOfflineTool::actionConvert_to_content_dispositon_file_nameclicked()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}

	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());

	QString urlrestr = QInputDialog::getText(0, u("regex of process url"), u("regex"), QLineEdit::Normal, ".*");

	QString urlstatevcal= QInputDialog::getText(0, u("sql url state value"), u("sql url state value"), QLineEdit::Normal, "urlstate=2");


	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{
		QRegularExpression urlre(urlrestr, reflag);
		QMap<QString, QString> urlfilename_md5filename, md5fnmap;

		auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
		QRegularExpression filestdre("[@\\t\\?/\\\\:<>\\|*\"]", reflag);
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url,content_disposition from  taskurls where " + urlstatevcal;
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();
				QString content_disposition = query4.value("content_disposition").toString();
				if (urlre.match(url).hasMatch() && content_disposition!="")
				{
					QString urlfilename = urltofilename(url, &urlre_ext);
					if (QFile::exists(savepath + "/" + urlfilename))
					{
						QString newfilename = content_disposition;
						QString contenttype = query4.value("contenttype").toString();
						if (contenttype != "" && contenttype.indexOf("/") == -1)
						{
							newfilename += "." + contenttype.mid(contenttype.lastIndexOf("/") + 1);
						}

						int fileno = 1;
						QString newfi = newfilename;
						while (QFile::exists(savepath + "/" + newfi))
						{
							newfi = newfilename;
							newfi.replace(newfi.lastIndexOf("."), 1, u("_%1.").arg(fileno));
							fileno++;
						}
						newfilename = newfi;
						auto lineba2 = newfilename.toUtf8();
						std::string src(lineba2.data(), lineba2.size()), result;
						if (HtmlToText(src, &result))
						{
							newfilename = QString::fromUtf8(result.data(), result.size());
						}
						QString newfilename2 = newfilename.replace(filestdre, "_");
						bool brl = QFile::rename(savepath + "/" + urlfilename, savepath + "/" + newfilename2);
					}
				}
			}
		}

		db.close();
	}
}




void WebsiteOfflineTool::action_ted_con_fix_vtt_disposition_content_value()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}

	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());

	QString urlrestr = QInputDialog::getText(0, u("regex of process url"), u("regex"), QLineEdit::Normal, ".*");

	QString urlstatevcal = QInputDialog::getText(0, u("sql url state value"), u("sql url state value"), QLineEdit::Normal, " url like '%zh-cn/full.vtt'");


	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{
		QRegularExpression urlre(urlrestr, reflag);
		QMap<QString, QString> urlfilename_md5filename, md5fnmap;

		QRegularExpression discontentre("<title>([^<]*)\\s\\x{007c}\\sTED\\sTalk</title>.*?\"ted://talks/(\\d+)\\?source=facebook\"", reflag);

		QDir di2(savepath);
		QStringList di2ls=di2.entryList(QDir::Files);
		QMap<QString, QString> cttid_filepath;
		db.transaction();
		for (int di = 0; di < di2ls.size(); di++)
		{
			QFile ff(savepath + "/" + di2ls[di]);
			ff.open(QIODevice::ReadOnly);
			QString ctt = ff.readAll();
			ff.close();
			if (ctt.indexOf("source=facebook") != -1)
			{
				auto ma = discontentre.match(ctt);
				QString id = ma.captured(2);
				QString title = ma.captured(1);
				if (title == "" || title.indexOf("&#39;")!=-1)
				{
					int e = 32432;
				}

				auto lineba2 = title.toUtf8();
				std::string src(lineba2.data(), lineba2.size()), result;
				if (HtmlToText(src, &result))
				{
					title = QString::fromUtf8(result.data(), result.size());
				}
				cttid_filepath.insert(id, title + "`" + di2ls[di]);

				//QSqlQuery query2(db);
				//QString url = "https://hls.ted.com/talks/" + id + "/subtitles/zh-cn/full.vtt";
				//query2.prepare("update taskurls set url='" + url + "' where content_disposition='" + title + "'");
				//auto rk=query2.exec();
				//if (!rk)
				//{
				//	int e = 324324;
				//}

			}
		}
		db.commit();

		//return;

		auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
		QRegularExpression filestdre("[@\\t\\?/\\\\:<>\\|*\"]", reflag);
		
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url,content_disposition from  taskurls where " + urlstatevcal;
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			db.transaction();
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();
				QString content_disposition = query4.value("content_disposition").toString();
				if (url.indexOf("2879") != -1)
				{
					int e = 32432432;
				}
				if (urlre.match(url).hasMatch())
				{
					QString ttid=QRegularExpression("/(\\d+)/").match(url).captured(1);
					if (cttid_filepath.contains(ttid))
					{
						QString urlfilename = urltofilename(url, &urlre_ext);
						//if (QFile::exists(savepath + "/" + urlfilename))
						{
							QStringList kkkdls = cttid_filepath[ttid].split("`");
							/*QFile ff(kkkdls[1]);
							ff.open(QIODevice::ReadOnly);
							QString pagectt = ff.readAll();
							ff.close();
							auto ma = discontentre.match(pagectt);
							QString contentdispos = ma.captured(1);*/
							QSqlQuery query2(db);
							query2.prepare("update taskurls set content_disposition=:cttdis where id=" + urlid);
							query2.bindValue(":cttdis", kkkdls[0]);
							query2.exec();
						}
					}
				}
			}
			db.commit();
		}

		db.close();
	}
}




void WebsiteOfflineTool::action_ted_con_fix_filename()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}

	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());

	QString urlrestr = QInputDialog::getText(0, u("regex of process url"), u("regex"), QLineEdit::Normal, ".*");

	QString urlstatevcal = QInputDialog::getText(0, u("sql url state value"), u("sql url state value"), QLineEdit::Normal, "urlstate=2");


	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{
		QRegularExpression urlre(urlrestr, reflag);
		QMap<QString, QString> urlfilename_md5filename, md5fnmap;

		QRegularExpression discontentre("<title>([^<]*)\\s\\x{007c}\\sTED\\sTalk</title>.*?\"ted://talks/(\\d+)\\?source=facebook\"", reflag);

		QDir dir2(u("D:\\TED950KB"));
		QStringList filels=dir2.entryList(QDir::Files);

		QDir di2(savepath);
		QStringList di2ls=di2.entryList(QDir::Files);
		QMap<QString, QString> cttid_filepath;
		db.transaction();
		QFile ff2("ted_rename_result.txt");
		ff2.open(QIODevice::WriteOnly);
		for (int di = 0; di < di2ls.size(); di++)
		{
			QFile ff(savepath + "/" + di2ls[di]);
			ff.open(QIODevice::ReadOnly);
			QString ctt = ff.readAll();
			ff.close();
			if (ctt.indexOf("source=facebook") != -1)
			{
				auto ma = discontentre.match(ctt);
				QString id = ma.captured(2);
				QString title = ma.captured(1);
				if (title == "" || title.indexOf("&#39;") != -1)
				{
					int e = 32432;
				}

				auto lineba2 = title.toUtf8();
				std::string src(lineba2.data(), lineba2.size()), result;
				if (HtmlToText(src, &result))
				{
					title = QString::fromUtf8(result.data(), result.size());
				}

				QRegularExpression filestdre("[@\\t\\?/\\\\:<>\\|*\"]", reflag);
				QString title2 = title.replace(filestdre, "_");

				for (int aai = 0; aai < filels.size(); aai++)
				{
					if (ctt.indexOf(filels[aai]) != -1)
					{
						QString filerenameinfo = u("D:\\TED950KB\\") + filels[aai] + "\t" + u("D:\\TED950KB\\") + title + filels[aai].mid(filels[aai].indexOf("_"));
						ff2.write(filerenameinfo.toUtf8()+"\n");
						//QFile::rename(u("M:\\ted.com_talks\\TED视频64K2390个\\") + filels[aai], u("M:\\ted.com_talks\\") + filels[aai]);
						break;
					}
				}


			}
		}
		ff2.close();

		db.commit();

		db.close();
	}
}


void WebsiteOfflineTool::actionDownload_a_test_url_to_test_htmlclicked()
{


	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);

	auto curitem6 = model->item(ind.row(), 6);
	auto curitem7 = model->item(ind.row(), 7);
	auto curitem8 = model->item(ind.row(), 8);
	auto curitem9 = model->item(ind.row(), 9);
	auto curitem10 = model->item(ind.row(), 10);
	auto curitem11 = model->item(ind.row(), 11);

	auto curitem12 = model->item(ind.row(), 12);
	auto curitem13 = model->item(ind.row(), 13);
	auto curitem14 = model->item(ind.row(), 14);
	auto curitem15 = model->item(ind.row(), 15);
	auto curitem16 = model->item(ind.row(), 16);
	auto curitem17 = model->item(ind.row(), 17);
	auto curitem18 = model->item(ind.row(), 18);
	auto curitem19 = model->item(ind.row(), 19);
	auto curitem20 = model->item(ind.row(), 20);
	auto curitem21 = model->item(ind.row(), 21);

	auto curitem22 = model->item(ind.row(), 22);
	auto curitem23 = model->item(ind.row(), 23);
	auto curitem24 = model->item(ind.row(), 24);
	auto curitem25 = model->item(ind.row(), 25);
	auto curitem26 = model->item(ind.row(), 26);

	auto curitem27 = model->item(ind.row(), 27);
	auto curitem28 = model->item(ind.row(), 28);
	auto curitem29 = model->item(ind.row(), 29);
	auto curitem30 = model->item(ind.row(), 30);
	auto curitem31 = model->item(ind.row(), 31);

	auto curitem32 = model->item(ind.row(), 32);
	auto curitem33 = model->item(ind.row(), 33);
	auto curitem34 = model->item(ind.row(), 34);
	auto curitem35 = model->item(ind.row(), 35);
	auto curitem36 = model->item(ind.row(), 36);

	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QString waitsec = curitem6->text();
	QString downsec = curitem7->text();

	QString rfurlmatch = curitem8->text();
	QString linkrfsec = curitem9->text();
	QString taskintvalmsec = curitem10->text();
	QString useragent = curitem11->text();

	QString loginlink = curitem12->text();
	QString loginpost = curitem13->text();
	QString loginsucmatch = curitem14->text();
	QString url_ext_relist = curitem15->text();
	QString relink_html_to_local = curitem16->text();
	QString fileexistdonotdown_match = curitem17->text();
	QString savetorocksdb = curitem18->text();
	QString noexistmatch = curitem19->text();
	QString pagenoexistmatch = curitem20->text();
	QString pagetimeoutsec = curitem21->text();

	QString urlfindre = curitem22->text();
	QString combiurlrepre = curitem23->text();
	QString combitimerepre = curitem24->text();
	QString combititlerepre = curitem25->text();
	QString newurlstate = curitem26->text();


	QString urlfindre2 = curitem27->text();
	QString combiurlrepre2 = curitem28->text();
	QString combitimerepre2 = curitem29->text();
	QString combititlerepre2 = curitem30->text();
	QString newurlstate2 = curitem31->text();


	QString PageGateSrvList = curitem32->text();
	QString PageGateCrawlUrlMatch1 = curitem33->text();
	QString PageGateScript1 = curitem34->text();
	QString PageGateCrawlUrlMatch2 = curitem35->text();
	QString PageGateScript2 = curitem36->text();



	QString url = QInputDialog::getText(0, u("url of download"), u("url"));
	if (url == "")return;
	CUrlDirectorDownResult       downer_result;
	int							downer_retcode;

	info->setText(u("downloading..."));
	qApp->processEvents();
	std::map<std::string, std::string> postdata2;
    downer_retcode = CurlHttpGet(std::string(url.toUtf8().data()), &postdata2, &downer_result, 0, 30, useragent.toStdString(), std::string(""), CUrlDD_NeedDataBit_Content | CUrlDD_NeedDataBit_Header | CUrlDD_NeedDataBit_Cookie);
	QFile ff("test.html");
	ff.open(QIODevice::WriteOnly);
	ff.write(downer_result.targetHeader.data(), downer_result.targetHeader.size());
	ff.write("\r\n\r\n", 4);
	ff.write(downer_result.targetCoentent.data(), downer_result.targetCoentent.size());
	ff.close();
	info->setText(u("download success"));
}

void WebsiteOfflineTool::actionUpdate_urlstate_by_find_contentclicked()
{
    /*
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);
	auto curitem18 = model->item(ind.row(), 18);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QString savetorocksdb = curitem18->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());


	QString cttrestr = QInputDialog::getText(0, u("content find regex"), u("regex"));
	if (cttrestr == "")
	{
		return;
	}
	QRegularExpression cttre(cttrestr, reflag);


	QString urlstate = QInputDialog::getText(0, u("urlstate value if match"), u("urlstate"));
	if (urlstate == "")
	{
		return;
	}

	QString ctturlstate = QInputDialog::getText(0, u("find content urlstate"), u("find content urlstate"),QLineEdit::Normal,"2");
	if (ctturlstate == "")
	{
		return;
	}


	QMessageBox msg;
	msg.setText(u("是否删除匹配的条目?"));
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	auto noexistdeal = msg.exec();
	if (noexistdeal == QMessageBox::Cancel)
	{
		return;
	}


	//
	rocksdb::DB *dbhand = 0;
	dbhand = rocksdbopen(savepath + "\\urldata_rocksdb");


	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else
	{
		QMap<QString, QString> urlfilename_md5filename, md5fnmap;
		QList<int>   needtostate0id;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls where urlstate=" + ctturlstate;
		bool rl = query4.exec(query4sql);
		int urlcount = 0;
		if (rl)
		{
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();
				QString filename = urltofilename(url, &urlre_ext);
				urlcount++;

				if (dbhand != 0)
				{
					auto urlu8 = url.toUtf8();
					std::string value;
					auto rl = dbhand->Get(rocksdb::ReadOptions(), std::string(urlu8.data(), urlu8.size()), &value);
					if (rl.ok())
					{
						QString cttff = QString::fromUtf8(value.data(), value.size());
						if (cttre.match(cttff).hasMatch())
						{
							QSqlQuery query4(db);
							QString query4sql = QString() + "update taskurls set urlstate=" + urlstate + " where id=" + urlid;
							bool rl = query4.exec(query4sql);

							if (noexistdeal == QMessageBox::Yes)
							{
								auto delrl = dbhand->Delete(rocksdb::WriteOptions(), url.toUtf8().data());
							}

						}
					}
				}
				else{
					QFile ff(savepath + "/" + filename);
					ff.open(QIODevice::ReadOnly);
					QString cttff=ff.readAll();
					ff.close();

					if (cttre.match(cttff).hasMatch())
					{
						QSqlQuery query4(db);
						QString query4sql = QString() + "update taskurls set urlstate="+urlstate+" where id="+urlid;
						bool rl = query4.exec(query4sql);

						if (noexistdeal == QMessageBox::Yes)
						{
							QFile::remove(savepath + "/" + filename);
						}
					}

				}
			}
		}


		db.close();
	}

	if (dbhand)
	{
		delete dbhand;
		dbhand = 0;
	}
    */
}


void WebsiteOfflineTool::actionDelete_when_urlstate_is_6()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());


	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{
		QSqlQuery query4(db);
		QString query4sql;
		query4sql = QString() + "select id,url,content_disposition from  taskurls where urlstate=6";
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();
				QString content_disposition = query4.value("content_disposition").toString();
				QString ufilename = urltofilename(url);

				if (QFile::exists(savepath + "/" + ufilename))
				{
					QFile::remove(savepath + "/" + ufilename);
				}
			}
		}

		query4.exec("update taskurl set urlstate=0 where urlstate=6 ");

		db.close();
	}


}


void WebsiteOfflineTool::actionExport_title_by_url_regex()
{
    /*
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());


	QString urllike = QInputDialog::getText(0, u("url sql like express"), u("url sql like express"), QLineEdit::Normal, "%");
	if (urllike == "")return;


	//
	rocksdb::DB *dbhand = rocksdbopen(savepath + "\\urldata_rocksdb");
	if (dbhand == 0)
	{
		QMessageBox msg;
		msg.setText(u("打开数据库失败"));
		msg.exec();
		return;
	}




	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{
		QString savepath = QFileDialog::getSaveFileName(0, "title list save to");
		QFile ff(savepath);
		ff.open(QIODevice::WriteOnly);
		QMap<QString, QString> urlfilename_md5filename, md5fnmap;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls where url like '"+urllike+"'";
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			while (query4.next())
			{
				QString id=query4.value("id").toString();
				QString url = query4.value("url").toString();
				QByteArray urlu8 = url.toUtf8();

				std::string ctt;
				auto rl = dbhand->Get(rocksdb::ReadOptions(), std::string(urlu8.data(), urlu8.size()), &ctt);
				if (rl.ok())
				{
					QString text = QString::fromUtf8(ctt.data(), ctt.size());
					int st = text.indexOf(QRegularExpression("<title>", reflag)), ed = text.indexOf(QRegularExpression("</title>",reflag));
					QString Title = text.mid(st + strlen("<title>"), ed - st - strlen("<title>"));
					ff.write(Title.toUtf8());
					ff.write("\r\n");
				}
			}
		}
		ff.close();
		db.close();
	}


	if (dbhand)
	{
		delete dbhand;
		dbhand = 0;
	}
    */
}

void WebsiteOfflineTool::actionExport_title_by_url_regex_for_xuetutu()
{
    /*
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());


	QString urllike = QInputDialog::getText(0, u("url sql like express"), u("url sql like express"), QLineEdit::Normal, "%");
	if (urllike == "")return;


	QString htmlrestr = QInputDialog::getText(0, u("add time regex"), u("add time regex"), QLineEdit::Normal, "");
	if (htmlrestr == "")return;

	QString htmlrerepstr = QInputDialog::getText(0, u("add time regex replace express"), u("add time regex replace express"), QLineEdit::Normal, "");
	if (htmlrerepstr == "")return;



	//
	rocksdb::DB *dbhand = rocksdbopen(savepath + "\\urldata_rocksdb");
	if (dbhand == 0)
	{
		QMessageBox msg;
		msg.setText(u("打开数据库失败"));
		msg.exec();
		return;
	}




	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{
		QString savepath = QFileDialog::getSaveFileName(0, "title list save to");
		QFile ff(savepath);
		ff.open(QIODevice::WriteOnly);
		QMap<QString, QString> urlfilename_md5filename, md5fnmap;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls where url like '"+urllike+"'";
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			int count = 0;
			while (query4.next())
			{
				QString id=query4.value("id").toString();
				QString url = query4.value("url").toString();
				QByteArray urlu8 = url.toUtf8();

				std::string ctt;
				auto rl = dbhand->Get(rocksdb::ReadOptions(), std::string(urlu8.data(), urlu8.size()), &ctt);
				if (rl.ok())
				{
					QString text = QString::fromUtf8(ctt.data(), ctt.size());
					int st = text.indexOf(QRegularExpression("<title>", reflag)), ed = text.indexOf(QRegularExpression(u("</title>"),reflag));
					if (st != -1 && ed != -1)
					{
						QString Title = text.mid(st + strlen("<title>"), ed - st - strlen("<title>"));
						QString reprlstr = RegexGobalMatchAndCombi(text, htmlrestr, htmlrerepstr);
						//if (reprlstr != "")
						{
							ff.write((Title + "\t" + reprlstr).toUtf8());
							ff.write("\r\n");
							count++;
							if (count % 1000 == 0)
							{
								info->setText(u("cur %1").arg(count));
								qApp->processEvents();
							}
						}
					}
				}
			}
		}
		ff.close();
		db.close();
	}


	if (dbhand)
	{
		delete dbhand;
		dbhand = 0;
	}
    */
}


void WebsiteOfflineTool::actionAddtime_from_html_regex()
{
    /*
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());

	QString urllike = QInputDialog::getText(0, u("url sql like express"), u("url sql like express"), QLineEdit::Normal, "%");
	if (urllike == "")return;

	QString htmlrestr = QInputDialog::getText(0, u("url html regex"), u("url html regex"), QLineEdit::Normal, "");
	if (htmlrestr == "")return;

	QString htmlrerepstr = QInputDialog::getText(0, u("url html regex replace express"), u("url html regex replace express"), QLineEdit::Normal, "");
	if (htmlrerepstr == "")return;


	//
	rocksdb::DB *dbhand = rocksdbopen(savepath + "\\urldata_rocksdb");
	if (dbhand == 0)
	{
		QMessageBox msg;
		msg.setText(u("打开数据库失败"));
		msg.exec();
		return;
	}




	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{
		int count = 0;
		db.transaction();
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls where url like '"+urllike+"'";
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			QRegularExpression htre(htmlrestr, reflag);
			
			while (query4.next())
			{
				QString id=query4.value("id").toString();
				QString url = query4.value("url").toString();
				QByteArray urlu8 = url.toUtf8();

				std::string ctt;
				auto rdbrl = dbhand->Get(rocksdb::ReadOptions(), std::string(urlu8.data(), urlu8.size()), &ctt);
				if (rdbrl.ok())
				{
					QString text = QString::fromUtf8(ctt.data(), ctt.size());
					QString reprlstr = RegexGobalMatchAndCombi(text, htmlrestr, htmlrerepstr);
					if (reprlstr != "")
					{
						QSqlQuery query5(db);
						QString query5sql = QString() + "update taskurls set addtime='" + reprlstr + "' where id=" + id;
						rl = query5.exec(query5sql);
						count++;
						if (count % 1000 == 0)
						{
							info->setText(u("cur %1").arg(count));
							qApp->processEvents();
						}
					}
				}
			}
		}
		db.commit();
		db.close();
	}


	if (dbhand)
	{
		delete dbhand;
		dbhand = 0;
	}
    */
}

void WebsiteOfflineTool::actionName_list_to_utf8_url_list()
{
	QString urltemplatestr = QInputDialog::getText(0, "url template", "url template(__name__ for name place)");
	QString savepath = QFileDialog::getOpenFileName(0, "title list save to");
	QFile ff(savepath);
	ff.open(QIODevice::ReadOnly);

	QFile ff2(savepath + "_newurl.txt");
	ff2.open(QIODevice::WriteOnly);
	while (!ff.atEnd())
	{
		QString linestr=ff.readLine();
		linestr = linestr.trimmed();
		QString newurl = urltemplatestr;
		newurl = newurl.replace("__name__", linestr.toUtf8().toPercentEncoding());
		ff2.write(newurl.toUtf8());
		ff2.write("\r\n");
	}
	ff2.close();

	QString cttls = ff.readAll();
	ff.close();

}


void WebsiteOfflineTool::actionName_list_to_utf8_url_list_for_xuetutu()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());



	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{
		QString query3sql = QString() + "create table if not exists  taskurls (id INTEGER PRIMARY KEY AUTOINCREMENT,url varchar(512) UNIQUE, urlstate INTEGER default 0, addtime TIMESTAMP default current_timestamp, filename varchar(256), contentlength integer, contenttype varchar(24), content_disposition varchar(256) )";
		QSqlQuery query3(db);
		auto ql3rl = query3.exec(query3sql);
		auto rl2 = query3.exec("create index if not exists addtime_index on taskurls(addtime)");
		QString st = query3.lastError().text();


		QString urltemplatestr = QInputDialog::getText(0, "url template", "url template(__name__ for name place)");
		QString savepath = QFileDialog::getOpenFileName(0, "title list open");
		QFile ff(savepath);
		ff.open(QIODevice::ReadOnly);
		int count = 0;
		db.transaction();
		while (!ff.atEnd())
		{
			QString linestr = ff.readLine();
			linestr = linestr.trimmed();
			QStringList linestrls=linestr.split("\t");
			QString newurl = urltemplatestr;
			newurl = newurl.replace("__name__", linestrls[0].toUtf8().toPercentEncoding());
			QString addtimeval;
			if (linestrls.size()>=2)
				addtimeval = linestrls[1].trimmed();
			if (addtimeval == "")
			{
				addtimeval = "now()";
			}
			else{
				addtimeval = "'" + addtimeval + "'";
			}
			QString sql = "insert into taskurls(url,addtime) values('" + newurl + "'," + addtimeval + ")";
			QSqlQuery qq(db);
			auto qrl = qq.exec(sql);
			count++;
			if (count % 1000 == 0)
			{
				info->setText(u("cur %1").arg(count));
				qApp->processEvents();
			}
		}
		db.commit();
		db.close();
		ff.close();
	}
}



void WebsiteOfflineTool::actionXueTutuUrlNotSameCorrect()
{
    /*
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);
	auto curitem18 = model->item(ind.row(), 18);
	auto curitem42 = model->item(ind.row(), 42);
	auto curitem43 = model->item(ind.row(), 43);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QString savetorocksdb = curitem18->text();

	QString MultiHostUrlReStr = curitem42->text();
	QString MultiHostRepExpr = curitem43->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";

	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());


	//
	rocksdb::DB *dbhand = 0;
	dbhand = rocksdbopen(savepath + "\\urldata_rocksdb");


	//get all url and file
	bool bdbsuc = false;
	QSet<QString>  havurl_filenameset;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else
	{
		QMap<QString, QString> urlfilename_md5filename, md5fnmap;
		QList<int>   needtostate0id;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from  taskurls where urlstate=2";
		bool rl = query4.exec(query4sql);
		int urlcount = 0;
		db.transaction();
		if (rl)
		{
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();
				QString filename = urltofilename(url, &urlre_ext);
				urlcount++;

				
				if (QFile::exists(savepath + "/" + filename))
				{
					if (QFileInfo(savepath + "/" + filename).size() < 5 * 1024)
					{
						QString kkk = "update taskurls set urlstate=404 where id=" + urlid;
						QSqlQuery ddd(db);
						auto kkkrl = ddd.exec(kkk);
					}
				}
				else{
					bool bfound = false;
					QStringList mals = MultiHostRepExpr.split(";");
					for (int i = 0; i < mals.size(); i++)
					{
						QString multihostnewurl = RegexGobalMatchAndCombi(url, MultiHostUrlReStr, mals[i]);
						auto u8url = multihostnewurl.toUtf8();

						QString filename = urltofilename(u8url, &urlre_ext);
						if (QFile::exists(savepath + "/" + filename))
						{
							QString kkk = "update taskurls set url='" + u8url + "' where id=" + urlid;
							QSqlQuery ddd(db);
							auto kkkrl = ddd.exec(kkk);
							bfound = true;
							break;
						}
					}
					if (bfound == false)
					{
						QString kkk = "update taskurls set urlstate=0 where id=" + urlid;
						QSqlQuery ddd(db);
						auto kkkrl = ddd.exec(kkk);
					}
				}
			}
		}
		db.commit();

		db.close();
	}

	if (dbhand)
	{
		delete dbhand;
		dbhand = 0;
	}
    */

}


void WebsiteOfflineTool::actionUrl_filename_to_url_md5_filename()
{
	auto ind = ui.tasklisttree->currentIndex();
	if (!ind.isValid())return;
	auto model = ui.tasklisttree->model();
	auto curitem0 = model->item(ind.row(), 0);
	auto curitem1 = model->item(ind.row(), 1);
	auto curitem2 = model->item(ind.row(), 2);
	auto curitem3 = model->item(ind.row(), 3);
	auto curitem4 = model->item(ind.row(), 4);
	auto curitem15 = model->item(ind.row(), 15);

	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QString url_ext_relist = curitem15->text();
	QMap<QString, QRegularExpression> urlre_ext;
	QStringList urlreextls = url_ext_relist.split(";");
	for (int i = 0; i < urlreextls.size(); i++)
	{
		QString ext = urlreextls[i].mid(1, urlreextls[i].indexOf("]") - 1), restr = urlreextls[i].mid(urlreextls[i].indexOf("]") + 1);
		QRegularExpression res(restr, reflag);
		if (res.isValid())
		{
			urlre_ext.insert(ext, res);
		}
	}


	QString inc_ls = curitem2->text();
	QString exc_ls = curitem3->text();
	QString threadnum = curitem4->text();

	QStringList mainurl = curitem0->text().split(";");
	QString firsturl = "";


	QString urllike = QInputDialog::getText(0, u("url sql like express"), u("url sql like express"), QLineEdit::Normal, "%");
	if (urllike == "")return;


	//root dir
	QString savepath = curitem1->text().replace("/", "\\");;
	CreateFullDir(savepath.toStdWString().c_str());


	//get all url and file
	bool bdbsuc = false;
	QMap<QString,QString>  urlfilename_urlmd5fname;
	QSqlDatabase db2 = QSqlDatabase::database("taskdb");
	if (db2.isOpen())
		db2.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "taskdb");
	db.setHostName("localhost");
	db.setDatabaseName(savepath + "/url.db");
	db.setUserName("root");
	db.setPassword("123456");
	if (!db.open())
	{
		QMessageBox msg;
		msg.setText(wtr(L"数据库打开失败！"));
		msg.exec();
	}
	else{

		QMap<QString, QString> urlfilename_md5filename, md5fnmap;
		QSqlQuery query4(db);
		QString query4sql = QString() + "select id,url from taskurls where url like '" + urllike + "'";
		bool rl = query4.exec(query4sql);
		if (rl)
		{
			while (query4.next())
			{
				QString urlid = query4.value("id").toString();
				QString url = query4.value("url").toString();

				QString urlfilename = getUrlFileName(savepath, url);
				urlfilename_urlmd5fname.insert(urlfilename,urltofilename(url, &urlre_ext));
				bdbsuc = true;
			}
		}

		db.close();
	}


	//delete no url file
	QDir savdir(savepath);
	auto allfile = savdir.entryList(QDir::Files);

	if (bdbsuc == true)
	{
		for (int i = 0; i < allfile.size(); i++)
		{
			QString downfile = allfile[i];

			auto urlit = urlfilename_urlmd5fname.find(downfile);
			if (urlit != urlfilename_urlmd5fname.end())
			{
				if (QFile::exists(savepath + "/" + urlit.key()))
				{
					QString urlmd5fn = urlit.value();
					QFile::rename(savepath + "/" + urlit.key(), savepath + "/" + urlit.value());
				}
			}

		}
	}



}

extern int *langindex;
void WebsiteOfflineTool::on_actionChinese_triggered()
{
    qApp->removeTranslator(translator);
    ui.retranslateUi(this);
    QFile ff(qApp->applicationDirPath()+QDir::separator()+"language.cfg");
    ff.open(QIODevice::WriteOnly);
    ff.write("-1");
    ff.close();
    QMessageBox msg;
    msg.setText(u("Effect after restart."));
    msg.exec();
}

void WebsiteOfflineTool::on_actionEnglish_triggered()
{
    //  翻译文件
    translator=new QTranslator;
    translator->load(":/websiteofflinetool_zhcn.qm");
    qApp->installTranslator(translator);
    ui.retranslateUi(this);
    QFile ff(qApp->applicationDirPath()+QDir::separator()+"language.cfg");
    ff.open(QIODevice::WriteOnly);
    ff.write("0");
    ff.close();

    QMessageBox msg;
    msg.setText(u("Effect after restart."));
    msg.exec();
}

void WebsiteOfflineTool::on_actionSave_triggered()
{
    auto model = ui.tasklisttree->model();
    QString cfgdata;
    for (int i = model->rowCount() - 2; i >= 0; i--)
    {
        QString linedata;
        for (int j = 0; j < model->columnCount(); j++)
        {
            auto item = model->item(i, j);
            if (item != NULL)
            {
                linedata += item->text().trimmed() + "`%";
            }else{
                linedata += wtr(L"") + "`%";
            }
        }
        if (linedata.size()>0){
            cfgdata += linedata + "\n";
        }
    }

    cfgdata = cfgdata.trimmed();
    QFile::remove("WebsiteOfflineTool_bak6.cfg");
    QFile::rename("WebsiteOfflineTool5.cfg", "WebsiteOfflineTool_bak6.cfg");
    QFile::rename("WebsiteOfflineTool4.cfg", "WebsiteOfflineTool_bak5.cfg");
    QFile::rename("WebsiteOfflineTool3.cfg", "WebsiteOfflineTool_bak4.cfg");
    QFile::rename("WebsiteOfflineTool2.cfg", "WebsiteOfflineTool_bak3.cfg");
    QFile::rename("WebsiteOfflineTool.cfg", "WebsiteOfflineTool_bak2.cfg");
    QFile ff("WebsiteOfflineTool.cfg");
    ff.open(QIODevice::WriteOnly);
    ff.write(cfgdata.toUtf8());
    ff.close();
}
