#ifndef RegexGobalMatchAndCombi_H
#define RegexGobalMatchAndCombi_H

#include <QRegularExpression>




QString RegexGobalMatchAndCombi(QString text,QString findre, QString repre, int NO_start=1, int NO_Step=1, int ContinueTimes=1);
QStringList RegexGobalMatchAndCombi_v_list(QString text, QString findre, QStringList reprelist, int NO_start=1, int NO_Step=1, int ContinueTimes=1);

#endif


