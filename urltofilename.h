#ifndef URLTOFILENAME_H
#define URLTOFILENAME_H


#include <QString>
#include <QRegularExpression>


QString urltofilename(QString url, QMap<QString,QRegularExpression> *defined_urlmatch_ext = 0);




#endif

