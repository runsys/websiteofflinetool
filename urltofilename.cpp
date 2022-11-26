#include "urltofilename.h"
#include <QCryptographicHash>
#include <QRegularExpression>


QString urltofilename(QString url, QMap<QString,QRegularExpression> *defined_urlmatch_ext)
{
	auto reflag = QRegularExpression::MultilineOption | QRegularExpression::ExtendedPatternSyntaxOption | QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption;
	QByteArray ba = QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Md5);
	QString md5val = ba.toHex().constData();


	QString ext;
	if (defined_urlmatch_ext != nullptr)
	{
		for (auto it = defined_urlmatch_ext->begin(); it != defined_urlmatch_ext->end(); it++)
		{
			if (it.key() != "")
			{
				if (it.value().match(url).hasMatch())
				{
					ext = it.key();
					if (ext[0] != '.')ext = "." + ext;
					break;
				}
			}
		}
	}

	if (ext == "")
	{
		ext = url;
		if (ext.lastIndexOf(".") != -1)
			ext = ext.mid(ext.lastIndexOf("."));
		else
			ext = "";
		if (ext.indexOf("?") != -1)
			ext = ext.mid(0, ext.indexOf("?"));
		if (ext == ".php")ext = ".html";
		ext = ext.replace(QRegularExpression("[\\?/\\\\:<>\\|*]", reflag), "_").trimmed();
		if (ext.size() > 6)ext = ext.mid(0, 6);
	}

	return md5val + ext;
}

