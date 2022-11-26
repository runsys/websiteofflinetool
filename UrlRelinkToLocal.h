#ifndef UrlRelinkToLocal_H
#define UrlRelinkToLocal_H


#include <QString>
#include <QMap>
#include <QRegularExpression>
#include <set>

//标准化url
QString tofullurl(QString urlstr, QString pageurl);
QString PageCttRelinkToLocal(QString rawctt, QString pageurl, std::set<QString> *out_newurl, const QStringList &inc_ls, const QStringList &exc_ls, const QList<QRegularExpression> &incre, const QList<QRegularExpression> &excre, QMap<QString,QRegularExpression> *urlreextls);
QString PageCttRelinkToNet(QString rawctt, QString pageurl, std::set<QString> *out_newurl, const QStringList &incls, const QStringList &excls, const QList<QRegularExpression> &incre, const QList<QRegularExpression> &excre, QMap<QString,QRegularExpression> *urlreextls);
QString toutf8encodeurl(QString urlstr);
QString urlunpercenteccode(QString urlstr);

#endif


