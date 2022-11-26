#include "RegexGobalMatchAndCombi.h"


QString RegexGobalMatchAndCombi(QString text, QString findre, QString repre, int NO_start, int NO_Step, int ContinueTimes)
{
	QString sourcetext = text;
	QString rescript = findre;
	QString incrstep = QString("%1").arg(ContinueTimes);
	QRegularExpression::PatternOptions reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	
	QRegularExpression	re(rescript, reflag);
	auto MaIt = re.globalMatch(sourcetext);
	QString replace_with_string = repre;
	QString result_repladed_str;

	int stepnum = NO_Step;
	int startnum = NO_start - stepnum;
	int nowstartnum = startnum;
	int incrstepnum = incrstep.toInt();
	if (incrstepnum <= 0)incrstepnum = 1;

	while (MaIt.hasNext())
	{
		auto Ma = MaIt.next();
		QString matotalstr = Ma.captured(0);
		QString replace_with_string_cp = replace_with_string;
		for (int gi = 0; gi <= Ma.lastCapturedIndex(); ++gi)
		{
			QString capstr = Ma.captured(gi);
			capstr = capstr.replace(QRegularExpression("%(\\d)"), "%#*#\\1");
			replace_with_string_cp=replace_with_string_cp.replace(QString("\\%1").arg(gi), capstr);
		}
		//
		if (replace_with_string_cp.indexOf("%1") != -1)
			replace_with_string_cp = replace_with_string_cp.arg(nowstartnum + stepnum);
		incrstepnum--;
		if (incrstepnum == 0)
		{
			nowstartnum += stepnum;
			//
			incrstepnum = incrstep.toInt();
		}
		//
		result_repladed_str += replace_with_string_cp;
	}
	result_repladed_str = result_repladed_str.replace(QRegularExpression("%#[*]#(\\d)"), "%\\1");
	return result_repladed_str;
}




QStringList RegexGobalMatchAndCombi_v_list(QString text, QString findre, QStringList reprelist, int NO_start, int NO_Step, int ContinueTimes)
{
	QString sourcetext = text;
	QString rescript = findre;
	QString incrstep = QString("%1").arg(ContinueTimes);
	QRegularExpression::PatternOptions reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;

	
	QStringList result_repladed_strls;
	for (int i = 0; i < reprelist.size(); i++)
	{
		result_repladed_strls.push_back("");
	}

	int stepnum = NO_Step;
	int startnum = NO_start - stepnum;
	int nowstartnum = startnum;
	int incrstepnum = incrstep.toInt();
	if (incrstepnum <= 0)incrstepnum = 1;

	QRegularExpression	re(rescript, reflag);
	auto MaIt = re.globalMatch(sourcetext);
	while (MaIt.hasNext())
	{
		auto Ma = MaIt.next();
		QString matotalstr = Ma.captured(0);
		for (int i = 0; i < reprelist.size(); i++)
		{
			QString replace_with_string = reprelist[i];
			QString replace_with_string_cp = replace_with_string;
			for (int gi = 0; gi <= Ma.lastCapturedIndex(); ++gi)
			{
				QString capstr = Ma.captured(gi);
				capstr = capstr.replace(QRegularExpression("%(\\d)"), "%#*#\\1");
				replace_with_string_cp = replace_with_string_cp.replace(QString("\\%1").arg(gi), capstr);
			}
			//
			if (replace_with_string_cp.indexOf("%1") != -1)
				replace_with_string_cp = replace_with_string_cp.arg(nowstartnum + stepnum);
			incrstepnum--;
			if (incrstepnum == 0)
			{
				nowstartnum += stepnum;
				//
				incrstepnum = incrstep.toInt();
			}
			//
			result_repladed_strls[i] += replace_with_string_cp;
		}
	}
	for (int i = 0; i < reprelist.size(); i++)
	{
		result_repladed_strls[i] = result_repladed_strls[i].replace(QRegularExpression("%#[*]#(\\d)"), "%\\1");
	}
	
	return result_repladed_strls;
}




