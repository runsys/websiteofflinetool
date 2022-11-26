#include "websiteofflinetool.h"
#include <QtWidgets/QApplication>
#include <QDir>
#include <array>
#include <iostream>  
#include <map>
#include <string>
#include <QFile>
#include <QProcessEnvironment>
#include "../../UtilityFunc/qtoolfunc.h"
#include "Windows.h"
#include <QTranslator>
using namespace std;


/*

*/

QString g_langid;

extern std::map<std::string, std::string> u8html_code_char_map;
extern void InitHtmlCharMap();
int main(int argc, char *argv[])
{
    //AutoAddPluginPath();
    //SetEnvironmentVariable(L"QT_QPA_PLATFORM_PLUGIN_PATH",L"M:\\work\\tool\\Qt5.5.1vc2010\\5.5\\msvc2010\\plugins");
	QApplication a(argc, argv);


	InitHtmlCharMap();
	/*
	QFile ff("out.txt");
	ff.open(QIODevice::WriteOnly);
	for (auto it = u8html_code_char_map.begin(); it != u8html_code_char_map.end(); it++){
		ff.write(it->first.data(), it->first.size());
		ff.write("`", 1);
		ff.write(it->second.data(), it->second.size());
		ff.write("\n", 1);
	}
	ff.close();
	*/

    QLocale myLocale;
    QString locale = myLocale.name();

    QFile ff(qApp->applicationDirPath()+QDir::separator()+"language.cfg");
    ff.open(QIODevice::ReadOnly);
    auto ffctt=ff.readAll();
    g_langid=ffctt;
    ff.close();
    if(ffctt=="0"){
        QTranslator *translator=new QTranslator;
        auto sdlfd=translator->load(":/websiteofflinetool_zhcn.qm");
        auto kldfsds=a.installTranslator(translator);
    }

	QDir::setCurrent(qApp->applicationDirPath());
	WebsiteOfflineTool w;
	w.show();
	return a.exec();
}
