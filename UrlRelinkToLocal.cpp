#include "UrlRelinkToLocal.h"
#include <QRegularExpression>
#include <QCryptographicHash>
#include <QFile>
#include <QMutex>
#include "urltofilename.h"

QString tofullurl(QString urlstr, QString pageurl)
{
	urlstr = urlstr.trimmed();
	if (urlstr.startsWith("//"))
	{
		QString urlstr2=pageurl.mid(0,pageurl.indexOf(":"))+urlstr;
		if (urlstr2.indexOf("/", 8) == -1)urlstr2 += "/";
		return urlstr2.trimmed();
	}
	if (urlstr.mid(0, 7).toLower() == "http://")
	{
		urlstr.replace("//", "/");
		urlstr.replace("//", "/");
		urlstr.replace("//", "/");
		urlstr.replace("http:/", "http://");

		if (urlstr.indexOf("/", 8) == -1)urlstr += "/";
		urlstr=urlstr.mid(0, urlstr.indexOf("/", 8)).toLower() + urlstr.mid(urlstr.indexOf("/", 8));
		return urlstr.trimmed();
	}
	if (urlstr.mid(0,8).toLower()=="https://")
	{
		urlstr.replace("//", "/");
		urlstr.replace("//", "/");
		urlstr.replace("//", "/");
		urlstr.replace("https:/", "https://");

		if (urlstr.indexOf("/", 8) == -1)urlstr += "/";
		urlstr = urlstr.mid(0, urlstr.indexOf("/", 8)).toLower() + urlstr.mid(urlstr.indexOf("/", 8));
		return urlstr.trimmed();
	}
	QStringList revls;
	if (urlstr[0] == '.')
	{
		QStringList urlpartls=urlstr.split("/");
		QStringList ls2=pageurl.split("/");
		for (int i = ls2.size() - 1; i >= 0; i--)
		{
			if (ls2[i] == "/")
			{
				ls2.removeAt(i);
			}
		}
		if (ls2.back() != "")
			ls2.pop_back();
		for (int i = urlpartls.size() - 1; i >= 0; i--)
		{
			if (urlpartls[i] == ".")
			{

			}
			else if (urlpartls[i] == "..")
			{
				if (ls2.size())
				{
					revls.push_front(ls2.back());
					ls2.pop_back();
				}
				else{
					return revls.join("");
				}
			}
			else{
				revls.push_front(urlpartls[i]);
			}
		}
		for (int i = ls2.size() - 1; i >= 0; i--)
		{
			revls.push_front("/");
			revls.push_front(ls2[i]);
		}
		QString urlstr2=revls.join("/");
		urlstr2.replace("//", "/");
		urlstr2.replace("//", "/");
		urlstr2.replace("//", "/");
		urlstr2.replace("http:/", "http://");
		urlstr2.replace("https:/", "https://");
		if (urlstr2.indexOf("/", 8) == -1)urlstr2 += "/";
		urlstr = urlstr.mid(0, urlstr.indexOf("/", 8)).toLower() + urlstr.mid(urlstr.indexOf("/", 8));
		return urlstr2.trimmed();
	}
	else if (urlstr[0] == '/')
	{
		QStringList urlpartls = urlstr.split("/");
		urlpartls.push_front("/");
		urlpartls.push_front(pageurl.mid(0, pageurl.indexOf("/",8)));
		QString urlstr2 = urlpartls.join("/");
		urlstr2.replace("//", "/");
		urlstr2.replace("//", "/");
		urlstr2.replace("//", "/");
		urlstr2.replace("http:/", "http://");
		urlstr2.replace("https:/", "https://");
		if (urlstr2.indexOf("/", 8) == -1)urlstr2 += "/";
		urlstr = urlstr.mid(0, urlstr.indexOf("/", 8)).toLower() + urlstr.mid(urlstr.indexOf("/", 8));
		return urlstr2.trimmed();
	}
	else if (urlstr[0] != '#' && !QRegularExpression("[a-zA-Z]+:").match(urlstr).hasMatch())
	{
		QStringList urlpartls = urlstr.split("/");
		QStringList ls2 = pageurl.split("/");
		for (int i = ls2.size() - 1; i >= 0; i--)
		{
			if (ls2[i] == "/")
			{
				ls2.removeAt(i);
			}
		}
		if (ls2.back() != "")
			ls2.pop_back();
		
		ls2.push_back("/");
		ls2.push_back(urlstr);
		
		QString urlstr2 = ls2.join("/");
		urlstr2.replace("//", "/");
		urlstr2.replace("//", "/");
		urlstr2.replace("//", "/");
		urlstr2.replace("http:/", "http://");
		urlstr2.replace("https:/", "https://");
		if (urlstr2.indexOf("/", 8) == -1)urlstr2 += "/";
		urlstr = urlstr.mid(0, urlstr.indexOf("/", 8)).toLower() + urlstr.mid(urlstr.indexOf("/", 8));
		return urlstr2.trimmed();
	}
	else{
		return urlstr.trimmed();
	}


}


std::set<QString> oldurl;
QMutex oldurlmutex;
QString PageCttRelinkToLocal(QString rawctt, QString pageurl, std::set<QString> *out_newurl, const QStringList &incls, const QStringList &excls, const QList<QRegularExpression> &incre, const QList<QRegularExpression> &excre, QMap<QString,QRegularExpression> *urlreextls)
{
	QString newpagectt = rawctt;
	//loop find all link and replace
	QRegularExpression atagre("<a[^>]*(\\shref\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

	auto maes=atagre.globalMatch(rawctt);
	QList<QRegularExpressionMatch> ls;
	while (maes.hasNext())
	{
		auto ma=maes.next();
		ls.push_front(ma);
	}
	for (int i = 0; i < ls.size(); i++)
	{
		QString mastr = ls[i].captured(1), newmastr;
		QString urlstr = mastr;
		urlstr = urlstr.mid(urlstr.indexOf("href")+4).trimmed();
		if (urlstr[0] == '=')
		{
			urlstr = urlstr.mid(1).trimmed();
			if (urlstr[0] == '\'')
			{
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\'') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\''));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}


			}
			else if (urlstr[0] == '\"'){
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\"') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}
			else{
				if (urlstr.indexOf(' ')!=-1 && urlstr.indexOf('>')!=-1)
				{
					if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
						urlstr = urlstr.mid(0, urlstr.indexOf(' '));
					else
						urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
				else if (urlstr.indexOf(' ') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}

			if (urlstr.size() > 0)
			{
				QString fullurl = tofullurl(urlstr, pageurl);
				if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
				{

					bool bpass = incls.size() == 0 ? true : false;
					for (int i = incls.size() - 1; i >= 0; i--)
					{
						if (incre[i].isValid())
						{
							if (incre[i].match(fullurl).hasMatch())
							{
								bpass = true;
								break;
							}
						}
						else if (fullurl.indexOf(incls[i]) != -1)
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
								if (excre[i].match(fullurl).hasMatch())
								{
									bpass = false;
									break;
								}
							}
							else if (fullurl.indexOf(excls[i]) != -1)
							{
								bpass = false;
								break;
							}
						}
					}

					if (bpass)
					{

						QString noanchorurl = fullurl, anchor;
						if (noanchorurl.indexOf("#") != -1)
						{
							anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
							noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
						}


						QString urlfilename = urltofilename(noanchorurl, urlreextls);
						QString localhtmlpath = urlfilename;
						QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

						newmastr = mastr.replace(urlstr, localhtmlpath + anchor);

						oldurlmutex.lock();
						if (oldurl.find(md5val) == oldurl.end())
						{
							out_newurl->insert(noanchorurl);
							if (oldurl.size() < 100000)oldurl.insert(md5val);
						}
						oldurlmutex.unlock();
					}
				}
			}
		}

		if (newmastr.size() > 0)
		{
			int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
			newpagectt.replace(st, ed - st, newmastr);
		}

	}



	//loop find all link and replace
	QRegularExpression atagre2("<img[^>]*(\\ssrc\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);
	auto maes2 = atagre2.globalMatch(newpagectt);
	ls.clear();
	while (maes2.hasNext())
	{
		auto ma = maes2.next();
		ls.push_front(ma);
	}
	for (int i = 0; i < ls.size(); i++)
	{
		QString mastr = ls[i].captured(1), newmastr;
		QString urlstr = mastr;
		urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
		if (urlstr[0] == '=')
		{
			urlstr = urlstr.mid(1).trimmed();
			if (urlstr[0] == '\'')
			{
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\'') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\''));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}


			}
			else if (urlstr[0] == '\"'){
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\"') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}
			else{
				if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
				{
					if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
						urlstr = urlstr.mid(0, urlstr.indexOf(' '));
					else
						urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
				else if (urlstr.indexOf(' ') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}

			if (urlstr.size() > 0)
			{
				QString fullurl = tofullurl(urlstr, pageurl);
				if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
				{
					bool bpass = incls.size() == 0 ? true : false;
					for (int i = incls.size() - 1; i >= 0; i--)
					{
						if (incre[i].isValid())
						{
							if (incre[i].match(fullurl).hasMatch())
							{
								bpass = true;
								break;
							}
						}
						else if (fullurl.indexOf(incls[i]) != -1)
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
								if (excre[i].match(fullurl).hasMatch())
								{
									bpass = false;
									break;
								}
							}
							else if (fullurl.indexOf(excls[i]) != -1)
							{
								bpass = false;
								break;
							}
						}
					}

					if (bpass)
					{
						QString noanchorurl = fullurl, anchor;
						if (noanchorurl.indexOf("#") != -1)
						{
							anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
							noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
						}

						QString urlfilename = urltofilename(noanchorurl, urlreextls);
						QString localhtmlpath = urlfilename;
						QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

						newmastr = mastr.replace(urlstr, localhtmlpath + anchor);

						oldurlmutex.lock();
						if (oldurl.find(md5val) == oldurl.end())
						{
							out_newurl->insert(noanchorurl);
							if (oldurl.size() < 100000)oldurl.insert(md5val);
						}
						oldurlmutex.unlock();
					}
				}
			}
		}
		if (newmastr.size() > 0)
		{
			int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
			newpagectt.replace(st, ed - st, newmastr);
		}

	}



	//loop find all link and replace
	QRegularExpression atagre3("<link[^>]*(\\shref\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

	auto maes3 = atagre3.globalMatch(newpagectt);
	ls.clear();
	while (maes3.hasNext())
	{
		auto ma = maes3.next();
		ls.push_front(ma);
	}
	for (int i = 0; i < ls.size(); i++)
	{
		QString mastr = ls[i].captured(1), newmastr;
		QString urlstr = mastr;
		urlstr = urlstr.mid(urlstr.indexOf("href") + 4).trimmed();
		if (urlstr[0] == '=')
		{
			urlstr = urlstr.mid(1).trimmed();
			if (urlstr[0] == '\'')
			{
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\'') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\''));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}


			}
			else if (urlstr[0] == '\"'){
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\"') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}
			else{
				if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
				{
					if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
						urlstr = urlstr.mid(0, urlstr.indexOf(' '));
					else
						urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
				else if (urlstr.indexOf(' ') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}

			if (urlstr.size() > 0)
			{
				QString fullurl = tofullurl(urlstr, pageurl);
				if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
				{
					bool bpass = incls.size() == 0 ? true : false;
					for (int i = incls.size() - 1; i >= 0; i--)
					{
						if (incre[i].isValid())
						{
							if (incre[i].match(fullurl).hasMatch())
							{
								bpass = true;
								break;
							}
						}
						else if (fullurl.indexOf(incls[i]) != -1)
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
								if (excre[i].match(fullurl).hasMatch())
								{
									bpass = false;
									break;
								}
							}
							else if (fullurl.indexOf(excls[i]) != -1)
							{
								bpass = false;
								break;
							}
						}
					}

					if (bpass)
					{
						QString noanchorurl = fullurl, anchor;
						if (noanchorurl.indexOf("#") != -1)
						{
							anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
							noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
						}

						QString urlfilename = urltofilename(noanchorurl, urlreextls);
						QString localhtmlpath = urlfilename;
						QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

						newmastr = mastr.replace(urlstr, localhtmlpath + anchor);

						oldurlmutex.lock();
						if (oldurl.find(md5val) == oldurl.end())
						{
							out_newurl->insert(noanchorurl);
							if (oldurl.size() < 100000)oldurl.insert(md5val);
						}
						oldurlmutex.unlock();
					}
				}
			}
		}

		if (newmastr.size() > 0)
		{
			int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
			newpagectt.replace(st, ed - st, newmastr);
		}

	}



	//loop find all link and replace
	QRegularExpression atagre4("<script[^>]*(\\ssrc\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

	auto maes4 = atagre4.globalMatch(newpagectt);
	ls.clear();
	while (maes4.hasNext())
	{
		auto ma = maes4.next();
		ls.push_front(ma);
	}
	for (int i = 0; i < ls.size(); i++)
	{
		QString mastr = ls[i].captured(1), newmastr;
		QString urlstr = mastr;
		urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
		if (urlstr[0] == '=')
		{
			urlstr = urlstr.mid(1).trimmed();
			if (urlstr[0] == '\'')
			{
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\'') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\''));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}


			}
			else if (urlstr[0] == '\"'){
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\"') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}
			else{
				if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
				{
					if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
						urlstr = urlstr.mid(0, urlstr.indexOf(' '));
					else
						urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
				else if (urlstr.indexOf(' ') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}

			if (urlstr.size() > 0)
			{
				QString fullurl = tofullurl(urlstr, pageurl);
				if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
				{
					bool bpass = incls.size() == 0 ? true : false;
					for (int i = incls.size() - 1; i >= 0; i--)
					{
						if (incre[i].isValid())
						{
							if (incre[i].match(fullurl).hasMatch())
							{
								bpass = true;
								break;
							}
						}
						else if (fullurl.indexOf(incls[i]) != -1)
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
								if (excre[i].match(fullurl).hasMatch())
								{
									bpass = false;
									break;
								}
							}
							else if (fullurl.indexOf(excls[i]) != -1)
							{
								bpass = false;
								break;
							}
						}
					}

					if (bpass)
					{
						QString noanchorurl = fullurl, anchor;
						if (noanchorurl.indexOf("#") != -1)
						{
							anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
							noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
						}

						QString urlfilename = urltofilename(noanchorurl, urlreextls);
						QString localhtmlpath = urlfilename;
						QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

						newmastr = mastr.replace(urlstr, localhtmlpath + anchor);

						oldurlmutex.lock();
						if (oldurl.find(md5val) == oldurl.end())
						{
							out_newurl->insert(noanchorurl);
							if (oldurl.size() < 100000)oldurl.insert(md5val);
						}
						oldurlmutex.unlock();
					}
				}
			}
		}
		if (newmastr.size() > 0)
		{
			int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
			newpagectt.replace(st, ed - st, newmastr);
		}

	}




	//loop find all link and replace
	QRegularExpression atagre5("<frame[^>]*(\\ssrc\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

	auto maes5 = atagre5.globalMatch(newpagectt);
	ls.clear();
	while (maes5.hasNext())
	{
		auto ma = maes5.next();
		ls.push_front(ma);
	}
	for (int i = 0; i < ls.size(); i++)
	{
		QString mastr = ls[i].captured(1), newmastr;
		QString urlstr = mastr;
		urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
		if (urlstr[0] == '=')
		{
			urlstr = urlstr.mid(1).trimmed();
			if (urlstr[0] == '\'')
			{
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\'') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\''));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}


			}
			else if (urlstr[0] == '\"'){
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\"') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}
			else{
				if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
				{
					if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
						urlstr = urlstr.mid(0, urlstr.indexOf(' '));
					else
						urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
				else if (urlstr.indexOf(' ') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}

			if (urlstr.size() > 0)
			{
				QString fullurl = tofullurl(urlstr, pageurl);
				if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
				{
					bool bpass = incls.size() == 0 ? true : false;
					for (int i = incls.size() - 1; i >= 0; i--)
					{
						if (incre[i].isValid())
						{
							if (incre[i].match(fullurl).hasMatch())
							{
								bpass = true;
								break;
							}
						}
						else if (fullurl.indexOf(incls[i]) != -1)
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
								if (excre[i].match(fullurl).hasMatch())
								{
									bpass = false;
									break;
								}
							}
							else if (fullurl.indexOf(excls[i]) != -1)
							{
								bpass = false;
								break;
							}
						}
					}

					if (bpass)
					{
						QString noanchorurl = fullurl, anchor;
						if (noanchorurl.indexOf("#") != -1)
						{
							anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
							noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
						}


						QString urlfilename = urltofilename(noanchorurl, urlreextls);
						QString localhtmlpath = urlfilename;
						QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

						newmastr = mastr.replace(urlstr, localhtmlpath + anchor);

						oldurlmutex.lock();
						if (oldurl.find(md5val) == oldurl.end())
						{
							out_newurl->insert(noanchorurl);
							if (oldurl.size() < 100000)oldurl.insert(md5val);
						}
						oldurlmutex.unlock();
					}
				}
			}
		}
		if (newmastr.size() > 0)
		{
			int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
			newpagectt.replace(st, ed - st, newmastr);
		}

	}





	//loop find all link and replace
	QRegularExpression atagre6("<iframe[^>]*(\\ssrc\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

	auto maes6 = atagre6.globalMatch(newpagectt);
	ls.clear();
	while (maes6.hasNext())
	{
		auto ma = maes6.next();
		ls.push_front(ma);
	}
	for (int i = 0; i < ls.size(); i++)
	{
		QString mastr = ls[i].captured(1), newmastr;
		QString urlstr = mastr;
		urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
		if (urlstr[0] == '=')
		{
			urlstr = urlstr.mid(1).trimmed();
			if (urlstr[0] == '\'')
			{
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\'') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\''));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}


			}
			else if (urlstr[0] == '\"'){
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\"') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}
			else{
				if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
				{
					if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
						urlstr = urlstr.mid(0, urlstr.indexOf(' '));
					else
						urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
				else if (urlstr.indexOf(' ') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}

			if (urlstr.size() > 0)
			{
				QString fullurl = tofullurl(urlstr, pageurl);
				if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
				{
					bool bpass = incls.size() == 0 ? true : false;
					for (int i = incls.size() - 1; i >= 0; i--)
					{
						if (incre[i].isValid())
						{
							if (incre[i].match(fullurl).hasMatch())
							{
								bpass = true;
								break;
							}
						}
						else if (fullurl.indexOf(incls[i]) != -1)
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
								if (excre[i].match(fullurl).hasMatch())
								{
									bpass = false;
									break;
								}
							}
							else if (fullurl.indexOf(excls[i]) != -1)
							{
								bpass = false;
								break;
							}
						}
					}

					if (bpass)
					{
						QString noanchorurl = fullurl, anchor;
						if (noanchorurl.indexOf("#") != -1)
						{
							anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
							noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
						}


						QString urlfilename = urltofilename(noanchorurl, urlreextls);
						QString localhtmlpath = urlfilename;
						QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

						newmastr = mastr.replace(urlstr, localhtmlpath + anchor);

						oldurlmutex.lock();
						if (oldurl.find(md5val) == oldurl.end())
						{
							out_newurl->insert(noanchorurl);
							if (oldurl.size() < 100000)oldurl.insert(md5val);
						}
						oldurlmutex.unlock();
					}
				}
			}
		}
		if (newmastr.size() > 0)
		{
			int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
			newpagectt.replace(st, ed - st, newmastr);
		}

	}



	//loop find all link and replace
	QRegularExpression atagre7("<source[^>]*(\\ssrc\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

	auto maes7 = atagre7.globalMatch(newpagectt);
	ls.clear();
	while (maes7.hasNext())
	{
		auto ma = maes7.next();
		ls.push_front(ma);
	}
	for (int i = 0; i < ls.size(); i++)
	{
		QString mastr = ls[i].captured(1), newmastr;
		QString urlstr = mastr;
		urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
		if (urlstr[0] == '=')
		{
			urlstr = urlstr.mid(1).trimmed();
			if (urlstr[0] == '\'')
			{
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\'') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\''));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}


			}
			else if (urlstr[0] == '\"'){
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\"') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}
			else{
				if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
				{
					if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
						urlstr = urlstr.mid(0, urlstr.indexOf(' '));
					else
						urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
				else if (urlstr.indexOf(' ') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}

			if (urlstr.size() > 0)
			{
				QString fullurl = tofullurl(urlstr, pageurl);
				if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
				{
					bool bpass = incls.size() == 0 ? true : false;
					for (int i = incls.size() - 1; i >= 0; i--)
					{
						if (incre[i].isValid())
						{
							if (incre[i].match(fullurl).hasMatch())
							{
								bpass = true;
								break;
							}
						}
						else if (fullurl.indexOf(incls[i]) != -1)
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
								if (excre[i].match(fullurl).hasMatch())
								{
									bpass = false;
									break;
								}
							}
							else if (fullurl.indexOf(excls[i]) != -1)
							{
								bpass = false;
								break;
							}
						}
					}

					if (bpass)
					{
						QString noanchorurl = fullurl, anchor;
						if (noanchorurl.indexOf("#") != -1)
						{
							anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
							noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
						}


						QString urlfilename = urltofilename(noanchorurl, urlreextls);
						QString localhtmlpath = urlfilename;
						QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

						newmastr = mastr.replace(urlstr, localhtmlpath + anchor);

						oldurlmutex.lock();
						if (oldurl.find(md5val) == oldurl.end())
						{
							out_newurl->insert(noanchorurl);
							if (oldurl.size() < 100000)oldurl.insert(md5val);
						}
						oldurlmutex.unlock();
					}
				}
			}
		}
		if (newmastr.size() > 0)
		{
			int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
			newpagectt.replace(st, ed - st, newmastr);
		}

	}




	//loop find all link and replace
	QRegularExpression atagre8("<embed[^>]*(\\ssrc\\s*=[^>]*))>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

	auto maes8 = atagre8.globalMatch(newpagectt);
	ls.clear();
	while (maes8.hasNext())
	{
		auto ma = maes8.next();
		ls.push_front(ma);
	}
	for (int i = 0; i < ls.size(); i++)
	{
		QString mastr = ls[i].captured(1).trimmed(), newmastr;
		QString urlstr = mastr;
		urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
		if (urlstr[0] == '=')
		{
			urlstr = urlstr.mid(1).trimmed();
			if (urlstr[0] == '\'')
			{
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\'') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\''));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}


			}
			else if (urlstr[0] == '\"'){
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\"') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}
			else{
				if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
				{
					if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
						urlstr = urlstr.mid(0, urlstr.indexOf(' '));
					else
						urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
				else if (urlstr.indexOf(' ') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}

			if (urlstr.size() > 0)
			{
				QString fullurl = tofullurl(urlstr, pageurl);
				if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
				{
					bool bpass = incls.size() == 0 ? true : false;
					for (int i = incls.size() - 1; i >= 0; i--)
					{
						if (incre[i].isValid())
						{
							if (incre[i].match(fullurl).hasMatch())
							{
								bpass = true;
								break;
							}
						}
						else if (fullurl.indexOf(incls[i]) != -1)
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
								if (excre[i].match(fullurl).hasMatch())
								{
									bpass = false;
									break;
								}
							}
							else if (fullurl.indexOf(excls[i]) != -1)
							{
								bpass = false;
								break;
							}
						}
					}

					if (bpass)
					{
						QString noanchorurl = fullurl, anchor;
						if (noanchorurl.indexOf("#") != -1)
						{
							anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
							noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
						}


						QString urlfilename = urltofilename(noanchorurl, urlreextls);
						QString localhtmlpath = urlfilename;
						QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));


						newmastr = mastr.replace(urlstr, localhtmlpath + anchor);

						oldurlmutex.lock();
						if (oldurl.find(md5val) == oldurl.end())
						{
							out_newurl->insert(noanchorurl);
							if (oldurl.size() < 100000)oldurl.insert(md5val);
						}
						oldurlmutex.unlock();
					}
				}
			}
		}
		if (newmastr.size() > 0)
		{
			int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
			newpagectt.replace(st, ed - st, newmastr);
		}

	}


	//loop find all link and replace
	QRegularExpression atagre9("<object[^>]*(\\sdata\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);
	auto maes9 = atagre9.globalMatch(newpagectt);
	ls.clear();
	while (maes9.hasNext())
	{
		auto ma = maes9.next();
		ls.push_front(ma);
	}
	for (int i = 0; i < ls.size(); i++)
	{
		QString mastr = ls[i].captured(1), newmastr;
		QString urlstr = mastr;
		urlstr = urlstr.mid(urlstr.indexOf("data") + 4).trimmed();
		if (urlstr[0] == '=')
		{
			urlstr = urlstr.mid(1).trimmed();
			if (urlstr[0] == '\'')
			{
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\'') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\''));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}


			}
			else if (urlstr[0] == '\"'){
				urlstr = urlstr.mid(1);
				if (urlstr.indexOf('\"') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
				}
				else if (urlstr.indexOf(' ') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1)
				{
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}
			else{
				if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
				{
					if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
						urlstr = urlstr.mid(0, urlstr.indexOf(' '));
					else
						urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
				else if (urlstr.indexOf(' ') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf(' '));
				}
				else if (urlstr.indexOf('>') != -1){
					urlstr = urlstr.mid(0, urlstr.indexOf('>'));
				}
			}

			if (urlstr.size() > 0)
			{
				QString fullurl = tofullurl(urlstr, pageurl);
				if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
				{
					bool bpass = incls.size() == 0 ? true : false;
					for (int i = incls.size() - 1; i >= 0; i--)
					{
						if (incre[i].isValid())
						{
							if (incre[i].match(fullurl).hasMatch())
							{
								bpass = true;
								break;
							}
						}
						else if (fullurl.indexOf(incls[i]) != -1)
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
								if (excre[i].match(fullurl).hasMatch())
								{
									bpass = false;
									break;
								}
							}
							else if (fullurl.indexOf(excls[i]) != -1)
							{
								bpass = false;
								break;
							}
						}
					}

					if (bpass)
					{
						QString noanchorurl = fullurl, anchor;
						if (noanchorurl.indexOf("#") != -1)
						{
							anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
							noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
						}


						QString urlfilename = urltofilename(noanchorurl, urlreextls);
						QString localhtmlpath = urlfilename;
						QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));


						newmastr = mastr.replace(urlstr, localhtmlpath + anchor);

						oldurlmutex.lock();
						if (oldurl.find(md5val) == oldurl.end())
						{
							out_newurl->insert(noanchorurl);
							if (oldurl.size() < 100000)oldurl.insert(md5val);
						}
						oldurlmutex.unlock();
					}
				}
			}
		}
		if (newmastr.size() > 0)
		{
			int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
			newpagectt.replace(st, ed - st, newmastr);
		}

	}


	return newpagectt;
}












QString PageCttRelinkToNet(QString rawctt, QString pageurl, std::set<QString> *out_newurl, const QStringList &incls, const QStringList &excls, const QList<QRegularExpression> &incre, const QList<QRegularExpression> &excre, QMap<QString,QRegularExpression> *urlreextls)
{
    QString newpagectt = rawctt;
    //loop find all link and replace
    QRegularExpression atagre("<a[^>]*(\\shref\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

    auto maes=atagre.globalMatch(rawctt);
    QList<QRegularExpressionMatch> ls;
    while (maes.hasNext())
    {
        auto ma=maes.next();
        ls.push_front(ma);
    }
    for (int i = 0; i < ls.size(); i++)
    {
        QString mastr = ls[i].captured(1), newmastr;
        QString urlstr = mastr;
        urlstr = urlstr.mid(urlstr.indexOf("href")+4).trimmed();
        if (urlstr[0] == '=')
        {
            urlstr = urlstr.mid(1).trimmed();
            if (urlstr[0] == '\'')
            {
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\'') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\''));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }


            }
            else if (urlstr[0] == '\"'){
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\"') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }
            else{
                if (urlstr.indexOf(' ')!=-1 && urlstr.indexOf('>')!=-1)
                {
                    if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
                        urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                    else
                        urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
                else if (urlstr.indexOf(' ') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }

            if (urlstr.size() > 0)
            {
                QString fullurl = tofullurl(urlstr, pageurl);
                if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
                {

                    bool bpass = incls.size() == 0 ? true : false;
                    for (int i = incls.size() - 1; i >= 0; i--)
                    {
                        if (incre[i].isValid())
                        {
                            if (incre[i].match(fullurl).hasMatch())
                            {
                                bpass = true;
                                break;
                            }
                        }
                        else if (fullurl.indexOf(incls[i]) != -1)
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
                                if (excre[i].match(fullurl).hasMatch())
                                {
                                    bpass = false;
                                    break;
                                }
                            }
                            else if (fullurl.indexOf(excls[i]) != -1)
                            {
                                bpass = false;
                                break;
                            }
                        }
                    }

                    if (bpass)
                    {

                        QString noanchorurl = fullurl, anchor;
                        if (noanchorurl.indexOf("#") != -1)
                        {
                            anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
                            noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
                        }


                        QString urlfilename = urltofilename(noanchorurl, urlreextls);
                        QString localhtmlpath = urlfilename;
                        QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

                        newmastr = mastr.replace(urlstr, noanchorurl + anchor);

                        oldurlmutex.lock();
                        if (oldurl.find(md5val) == oldurl.end())
                        {
                            out_newurl->insert(noanchorurl);
                            if (oldurl.size() < 100000)oldurl.insert(md5val);
                        }
                        oldurlmutex.unlock();
                    }
                }
            }
        }

        if (newmastr.size() > 0)
        {
            int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
            newpagectt.replace(st, ed - st, newmastr);
        }

    }



    //loop find all link and replace
    QRegularExpression atagre2("<img[^>]*(\\ssrc\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);
    auto maes2 = atagre2.globalMatch(newpagectt);
    ls.clear();
    while (maes2.hasNext())
    {
        auto ma = maes2.next();
        ls.push_front(ma);
    }
    for (int i = 0; i < ls.size(); i++)
    {
        QString mastr = ls[i].captured(1), newmastr;
        QString urlstr = mastr;
        urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
        if (urlstr[0] == '=')
        {
            urlstr = urlstr.mid(1).trimmed();
            if (urlstr[0] == '\'')
            {
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\'') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\''));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }


            }
            else if (urlstr[0] == '\"'){
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\"') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }
            else{
                if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
                {
                    if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
                        urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                    else
                        urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
                else if (urlstr.indexOf(' ') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }

            if (urlstr.size() > 0)
            {
                QString fullurl = tofullurl(urlstr, pageurl);
                if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
                {
                    bool bpass = incls.size() == 0 ? true : false;
                    for (int i = incls.size() - 1; i >= 0; i--)
                    {
                        if (incre[i].isValid())
                        {
                            if (incre[i].match(fullurl).hasMatch())
                            {
                                bpass = true;
                                break;
                            }
                        }
                        else if (fullurl.indexOf(incls[i]) != -1)
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
                                if (excre[i].match(fullurl).hasMatch())
                                {
                                    bpass = false;
                                    break;
                                }
                            }
                            else if (fullurl.indexOf(excls[i]) != -1)
                            {
                                bpass = false;
                                break;
                            }
                        }
                    }

                    if (bpass)
                    {
                        QString noanchorurl = fullurl, anchor;
                        if (noanchorurl.indexOf("#") != -1)
                        {
                            anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
                            noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
                        }

                        QString urlfilename = urltofilename(noanchorurl, urlreextls);
                        QString localhtmlpath = urlfilename;
                        QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

                        newmastr = mastr.replace(urlstr, noanchorurl + anchor);

                        oldurlmutex.lock();
                        if (oldurl.find(md5val) == oldurl.end())
                        {
                            out_newurl->insert(noanchorurl);
                            if (oldurl.size() < 100000)oldurl.insert(md5val);
                        }
                        oldurlmutex.unlock();
                    }
                }
            }
        }
        if (newmastr.size() > 0)
        {
            int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
            newpagectt.replace(st, ed - st, newmastr);
        }

    }



    //loop find all link and replace
    QRegularExpression atagre3("<link[^>]*(\\shref\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

    auto maes3 = atagre3.globalMatch(newpagectt);
    ls.clear();
    while (maes3.hasNext())
    {
        auto ma = maes3.next();
        ls.push_front(ma);
    }
    for (int i = 0; i < ls.size(); i++)
    {
        QString mastr = ls[i].captured(1), newmastr;
        QString urlstr = mastr;
        urlstr = urlstr.mid(urlstr.indexOf("href") + 4).trimmed();
        if (urlstr[0] == '=')
        {
            urlstr = urlstr.mid(1).trimmed();
            if (urlstr[0] == '\'')
            {
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\'') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\''));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }


            }
            else if (urlstr[0] == '\"'){
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\"') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }
            else{
                if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
                {
                    if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
                        urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                    else
                        urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
                else if (urlstr.indexOf(' ') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }

            if (urlstr.size() > 0)
            {
                QString fullurl = tofullurl(urlstr, pageurl);
                if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
                {
                    bool bpass = incls.size() == 0 ? true : false;
                    for (int i = incls.size() - 1; i >= 0; i--)
                    {
                        if (incre[i].isValid())
                        {
                            if (incre[i].match(fullurl).hasMatch())
                            {
                                bpass = true;
                                break;
                            }
                        }
                        else if (fullurl.indexOf(incls[i]) != -1)
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
                                if (excre[i].match(fullurl).hasMatch())
                                {
                                    bpass = false;
                                    break;
                                }
                            }
                            else if (fullurl.indexOf(excls[i]) != -1)
                            {
                                bpass = false;
                                break;
                            }
                        }
                    }

                    if (bpass)
                    {
                        QString noanchorurl = fullurl, anchor;
                        if (noanchorurl.indexOf("#") != -1)
                        {
                            anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
                            noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
                        }

                        QString urlfilename = urltofilename(noanchorurl, urlreextls);
                        QString localhtmlpath = urlfilename;
                        QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

                        newmastr = mastr.replace(urlstr, noanchorurl + anchor);

                        oldurlmutex.lock();
                        if (oldurl.find(md5val) == oldurl.end())
                        {
                            out_newurl->insert(noanchorurl);
                            if (oldurl.size() < 100000)oldurl.insert(md5val);
                        }
                        oldurlmutex.unlock();
                    }
                }
            }
        }

        if (newmastr.size() > 0)
        {
            int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
            newpagectt.replace(st, ed - st, newmastr);
        }

    }



    //loop find all link and replace
    QRegularExpression atagre4("<script[^>]*(\\ssrc\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

    auto maes4 = atagre4.globalMatch(newpagectt);
    ls.clear();
    while (maes4.hasNext())
    {
        auto ma = maes4.next();
        ls.push_front(ma);
    }
    for (int i = 0; i < ls.size(); i++)
    {
        QString mastr = ls[i].captured(1), newmastr;
        QString urlstr = mastr;
        urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
        if (urlstr[0] == '=')
        {
            urlstr = urlstr.mid(1).trimmed();
            if (urlstr[0] == '\'')
            {
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\'') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\''));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }


            }
            else if (urlstr[0] == '\"'){
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\"') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }
            else{
                if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
                {
                    if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
                        urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                    else
                        urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
                else if (urlstr.indexOf(' ') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }

            if (urlstr.size() > 0)
            {
                QString fullurl = tofullurl(urlstr, pageurl);
                if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
                {
                    bool bpass = incls.size() == 0 ? true : false;
                    for (int i = incls.size() - 1; i >= 0; i--)
                    {
                        if (incre[i].isValid())
                        {
                            if (incre[i].match(fullurl).hasMatch())
                            {
                                bpass = true;
                                break;
                            }
                        }
                        else if (fullurl.indexOf(incls[i]) != -1)
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
                                if (excre[i].match(fullurl).hasMatch())
                                {
                                    bpass = false;
                                    break;
                                }
                            }
                            else if (fullurl.indexOf(excls[i]) != -1)
                            {
                                bpass = false;
                                break;
                            }
                        }
                    }

                    if (bpass)
                    {
                        QString noanchorurl = fullurl, anchor;
                        if (noanchorurl.indexOf("#") != -1)
                        {
                            anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
                            noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
                        }

                        QString urlfilename = urltofilename(noanchorurl, urlreextls);
                        QString localhtmlpath = urlfilename;
                        QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

                        newmastr = mastr.replace(urlstr, noanchorurl + anchor);

                        oldurlmutex.lock();
                        if (oldurl.find(md5val) == oldurl.end())
                        {
                            out_newurl->insert(noanchorurl);
                            if (oldurl.size() < 100000)oldurl.insert(md5val);
                        }
                        oldurlmutex.unlock();
                    }
                }
            }
        }
        if (newmastr.size() > 0)
        {
            int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
            newpagectt.replace(st, ed - st, newmastr);
        }

    }




    //loop find all link and replace
    QRegularExpression atagre5("<frame[^>]*(\\ssrc\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

    auto maes5 = atagre5.globalMatch(newpagectt);
    ls.clear();
    while (maes5.hasNext())
    {
        auto ma = maes5.next();
        ls.push_front(ma);
    }
    for (int i = 0; i < ls.size(); i++)
    {
        QString mastr = ls[i].captured(1), newmastr;
        QString urlstr = mastr;
        urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
        if (urlstr[0] == '=')
        {
            urlstr = urlstr.mid(1).trimmed();
            if (urlstr[0] == '\'')
            {
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\'') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\''));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }


            }
            else if (urlstr[0] == '\"'){
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\"') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }
            else{
                if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
                {
                    if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
                        urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                    else
                        urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
                else if (urlstr.indexOf(' ') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }

            if (urlstr.size() > 0)
            {
                QString fullurl = tofullurl(urlstr, pageurl);
                if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
                {
                    bool bpass = incls.size() == 0 ? true : false;
                    for (int i = incls.size() - 1; i >= 0; i--)
                    {
                        if (incre[i].isValid())
                        {
                            if (incre[i].match(fullurl).hasMatch())
                            {
                                bpass = true;
                                break;
                            }
                        }
                        else if (fullurl.indexOf(incls[i]) != -1)
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
                                if (excre[i].match(fullurl).hasMatch())
                                {
                                    bpass = false;
                                    break;
                                }
                            }
                            else if (fullurl.indexOf(excls[i]) != -1)
                            {
                                bpass = false;
                                break;
                            }
                        }
                    }

                    if (bpass)
                    {
                        QString noanchorurl = fullurl, anchor;
                        if (noanchorurl.indexOf("#") != -1)
                        {
                            anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
                            noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
                        }


                        QString urlfilename = urltofilename(noanchorurl, urlreextls);
                        QString localhtmlpath = urlfilename;
                        QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

                        newmastr = mastr.replace(urlstr, noanchorurl + anchor);

                        oldurlmutex.lock();
                        if (oldurl.find(md5val) == oldurl.end())
                        {
                            out_newurl->insert(noanchorurl);
                            if (oldurl.size() < 100000)oldurl.insert(md5val);
                        }
                        oldurlmutex.unlock();
                    }
                }
            }
        }
        if (newmastr.size() > 0)
        {
            int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
            newpagectt.replace(st, ed - st, newmastr);
        }

    }





    //loop find all link and replace
    QRegularExpression atagre6("<iframe[^>]*(\\ssrc\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

    auto maes6 = atagre6.globalMatch(newpagectt);
    ls.clear();
    while (maes6.hasNext())
    {
        auto ma = maes6.next();
        ls.push_front(ma);
    }
    for (int i = 0; i < ls.size(); i++)
    {
        QString mastr = ls[i].captured(1), newmastr;
        QString urlstr = mastr;
        urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
        if (urlstr[0] == '=')
        {
            urlstr = urlstr.mid(1).trimmed();
            if (urlstr[0] == '\'')
            {
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\'') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\''));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }


            }
            else if (urlstr[0] == '\"'){
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\"') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }
            else{
                if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
                {
                    if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
                        urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                    else
                        urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
                else if (urlstr.indexOf(' ') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }

            if (urlstr.size() > 0)
            {
                QString fullurl = tofullurl(urlstr, pageurl);
                if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
                {
                    bool bpass = incls.size() == 0 ? true : false;
                    for (int i = incls.size() - 1; i >= 0; i--)
                    {
                        if (incre[i].isValid())
                        {
                            if (incre[i].match(fullurl).hasMatch())
                            {
                                bpass = true;
                                break;
                            }
                        }
                        else if (fullurl.indexOf(incls[i]) != -1)
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
                                if (excre[i].match(fullurl).hasMatch())
                                {
                                    bpass = false;
                                    break;
                                }
                            }
                            else if (fullurl.indexOf(excls[i]) != -1)
                            {
                                bpass = false;
                                break;
                            }
                        }
                    }

                    if (bpass)
                    {
                        QString noanchorurl = fullurl, anchor;
                        if (noanchorurl.indexOf("#") != -1)
                        {
                            anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
                            noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
                        }


                        QString urlfilename = urltofilename(noanchorurl, urlreextls);
                        QString localhtmlpath = urlfilename;
                        QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

                        newmastr = mastr.replace(urlstr, noanchorurl + anchor);

                        oldurlmutex.lock();
                        if (oldurl.find(md5val) == oldurl.end())
                        {
                            out_newurl->insert(noanchorurl);
                            if (oldurl.size() < 100000)oldurl.insert(md5val);
                        }
                        oldurlmutex.unlock();
                    }
                }
            }
        }
        if (newmastr.size() > 0)
        {
            int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
            newpagectt.replace(st, ed - st, newmastr);
        }

    }



    //loop find all link and replace
    QRegularExpression atagre7("<source[^>]*(\\ssrc\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

    auto maes7 = atagre7.globalMatch(newpagectt);
    ls.clear();
    while (maes7.hasNext())
    {
        auto ma = maes7.next();
        ls.push_front(ma);
    }
    for (int i = 0; i < ls.size(); i++)
    {
        QString mastr = ls[i].captured(1), newmastr;
        QString urlstr = mastr;
        urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
        if (urlstr[0] == '=')
        {
            urlstr = urlstr.mid(1).trimmed();
            if (urlstr[0] == '\'')
            {
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\'') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\''));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }


            }
            else if (urlstr[0] == '\"'){
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\"') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }
            else{
                if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
                {
                    if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
                        urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                    else
                        urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
                else if (urlstr.indexOf(' ') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }

            if (urlstr.size() > 0)
            {
                QString fullurl = tofullurl(urlstr, pageurl);
                if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
                {
                    bool bpass = incls.size() == 0 ? true : false;
                    for (int i = incls.size() - 1; i >= 0; i--)
                    {
                        if (incre[i].isValid())
                        {
                            if (incre[i].match(fullurl).hasMatch())
                            {
                                bpass = true;
                                break;
                            }
                        }
                        else if (fullurl.indexOf(incls[i]) != -1)
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
                                if (excre[i].match(fullurl).hasMatch())
                                {
                                    bpass = false;
                                    break;
                                }
                            }
                            else if (fullurl.indexOf(excls[i]) != -1)
                            {
                                bpass = false;
                                break;
                            }
                        }
                    }

                    if (bpass)
                    {
                        QString noanchorurl = fullurl, anchor;
                        if (noanchorurl.indexOf("#") != -1)
                        {
                            anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
                            noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
                        }


                        QString urlfilename = urltofilename(noanchorurl, urlreextls);
                        QString localhtmlpath = urlfilename;
                        QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));

                        newmastr = mastr.replace(urlstr, noanchorurl + anchor);

                        oldurlmutex.lock();
                        if (oldurl.find(md5val) == oldurl.end())
                        {
                            out_newurl->insert(noanchorurl);
                            if (oldurl.size() < 100000)oldurl.insert(md5val);
                        }
                        oldurlmutex.unlock();
                    }
                }
            }
        }
        if (newmastr.size() > 0)
        {
            int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
            newpagectt.replace(st, ed - st, newmastr);
        }

    }




    //loop find all link and replace
    QRegularExpression atagre8("<embed[^>]*(\\ssrc\\s*=[^>]*))>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);

    auto maes8 = atagre8.globalMatch(newpagectt);
    ls.clear();
    while (maes8.hasNext())
    {
        auto ma = maes8.next();
        ls.push_front(ma);
    }
    for (int i = 0; i < ls.size(); i++)
    {
        QString mastr = ls[i].captured(1).trimmed(), newmastr;
        QString urlstr = mastr;
        urlstr = urlstr.mid(urlstr.indexOf("src") + 3).trimmed();
        if (urlstr[0] == '=')
        {
            urlstr = urlstr.mid(1).trimmed();
            if (urlstr[0] == '\'')
            {
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\'') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\''));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }


            }
            else if (urlstr[0] == '\"'){
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\"') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }
            else{
                if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
                {
                    if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
                        urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                    else
                        urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
                else if (urlstr.indexOf(' ') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }

            if (urlstr.size() > 0)
            {
                QString fullurl = tofullurl(urlstr, pageurl);
                if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
                {
                    bool bpass = incls.size() == 0 ? true : false;
                    for (int i = incls.size() - 1; i >= 0; i--)
                    {
                        if (incre[i].isValid())
                        {
                            if (incre[i].match(fullurl).hasMatch())
                            {
                                bpass = true;
                                break;
                            }
                        }
                        else if (fullurl.indexOf(incls[i]) != -1)
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
                                if (excre[i].match(fullurl).hasMatch())
                                {
                                    bpass = false;
                                    break;
                                }
                            }
                            else if (fullurl.indexOf(excls[i]) != -1)
                            {
                                bpass = false;
                                break;
                            }
                        }
                    }

                    if (bpass)
                    {
                        QString noanchorurl = fullurl, anchor;
                        if (noanchorurl.indexOf("#") != -1)
                        {
                            anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
                            noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
                        }


                        QString urlfilename = urltofilename(noanchorurl, urlreextls);
                        QString localhtmlpath = urlfilename;
                        QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));


                        newmastr = mastr.replace(urlstr, noanchorurl + anchor);

                        oldurlmutex.lock();
                        if (oldurl.find(md5val) == oldurl.end())
                        {
                            out_newurl->insert(noanchorurl);
                            if (oldurl.size() < 100000)oldurl.insert(md5val);
                        }
                        oldurlmutex.unlock();
                    }
                }
            }
        }
        if (newmastr.size() > 0)
        {
            int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
            newpagectt.replace(st, ed - st, newmastr);
        }

    }


    //loop find all link and replace
    QRegularExpression atagre9("<object[^>]*(\\sdata\\s*=[^>]*)>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::DotMatchesEverythingOption);
    auto maes9 = atagre9.globalMatch(newpagectt);
    ls.clear();
    while (maes9.hasNext())
    {
        auto ma = maes9.next();
        ls.push_front(ma);
    }
    for (int i = 0; i < ls.size(); i++)
    {
        QString mastr = ls[i].captured(1), newmastr;
        QString urlstr = mastr;
        urlstr = urlstr.mid(urlstr.indexOf("data") + 4).trimmed();
        if (urlstr[0] == '=')
        {
            urlstr = urlstr.mid(1).trimmed();
            if (urlstr[0] == '\'')
            {
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\'') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\''));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }


            }
            else if (urlstr[0] == '\"'){
                urlstr = urlstr.mid(1);
                if (urlstr.indexOf('\"') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('\"'));
                }
                else if (urlstr.indexOf(' ') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1)
                {
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }
            else{
                if (urlstr.indexOf(' ') != -1 && urlstr.indexOf('>') != -1)
                {
                    if (urlstr.indexOf(' ')<urlstr.indexOf('>'))
                        urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                    else
                        urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
                else if (urlstr.indexOf(' ') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf(' '));
                }
                else if (urlstr.indexOf('>') != -1){
                    urlstr = urlstr.mid(0, urlstr.indexOf('>'));
                }
            }

            if (urlstr.size() > 0)
            {
                QString fullurl = tofullurl(urlstr, pageurl);
                if (fullurl.startsWith("http://") || fullurl.startsWith("https://"))
                {
                    bool bpass = incls.size() == 0 ? true : false;
                    for (int i = incls.size() - 1; i >= 0; i--)
                    {
                        if (incre[i].isValid())
                        {
                            if (incre[i].match(fullurl).hasMatch())
                            {
                                bpass = true;
                                break;
                            }
                        }
                        else if (fullurl.indexOf(incls[i]) != -1)
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
                                if (excre[i].match(fullurl).hasMatch())
                                {
                                    bpass = false;
                                    break;
                                }
                            }
                            else if (fullurl.indexOf(excls[i]) != -1)
                            {
                                bpass = false;
                                break;
                            }
                        }
                    }

                    if (bpass)
                    {
                        QString noanchorurl = fullurl, anchor;
                        if (noanchorurl.indexOf("#") != -1)
                        {
                            anchor = noanchorurl.mid(noanchorurl.indexOf("#"));
                            noanchorurl = noanchorurl.mid(0, noanchorurl.indexOf("#"));
                        }


                        QString urlfilename = urltofilename(noanchorurl, urlreextls);
                        QString localhtmlpath = urlfilename;
                        QString md5val = urlfilename.mid(0, urlfilename.indexOf("."));


                        newmastr = mastr.replace(urlstr, noanchorurl + anchor);

                        oldurlmutex.lock();
                        if (oldurl.find(md5val) == oldurl.end())
                        {
                            out_newurl->insert(noanchorurl);
                            if (oldurl.size() < 100000)oldurl.insert(md5val);
                        }
                        oldurlmutex.unlock();
                    }
                }
            }
        }
        if (newmastr.size() > 0)
        {
            int st = ls[i].capturedStart(1), ed = ls[i].capturedEnd(1);
            newpagectt.replace(st, ed - st, newmastr);
        }

    }


    return newpagectt;
}














QString toutf8encodeurl(QString urlstr)
{
	bool blashslash = false;
	QString urlstr2 = urlstr;
	if (urlstr2.indexOf("?")!=-1)
		urlstr2 = urlstr2.mid(0, urlstr2.indexOf("?"));
	QString urlpram = urlstr;
	if (urlpram.indexOf("?") != -1)
		urlpram = urlpram.mid(urlpram.indexOf("?") + 1);
	QStringList strls = urlstr2.split("/");
	if (urlstr2.endsWith("/"))blashslash = true;
	QString urlstrok;
	for (int i = 0; i < strls.size(); i++)
	{
		if (urlstrok.indexOf("://") == -1)
		{
			urlstrok += strls[i] + "/";
		}
		else if (urlstrok.indexOf("/", urlstrok.indexOf("://"))==-1)
		{
			urlstrok += strls[i] + "/";
		}
		else{
			urlstrok += strls[i].toUtf8().toPercentEncoding() + "/";
		}
	}
	if (blashslash == false)
	{
		urlstrok = urlstrok.mid(0, urlstrok.size() - 1);
	}

	QStringList paramseg = urlpram.split("&");
	for (int i = 0; i < paramseg.size(); i++)
	{
		if (paramseg[i].indexOf("=") != -1)
		{
			QString name = paramseg[i].mid(0, paramseg[i].indexOf("=")).trimmed();
			QString value = paramseg[i].mid(paramseg[i].indexOf("=") + 1);

			if (name != "")
			{
				if (i == 0)
				{
					urlstrok += "?";
					urlstrok += name.toUtf8().toPercentEncoding() + "=" + value.toUtf8().toPercentEncoding();
				}
				else{
					urlstrok += "&"+name.toUtf8().toPercentEncoding() + "=" + value.toUtf8().toPercentEncoding();
				}
			}
		}
	}

	
	return urlstrok;
}

QString urlunpercenteccode(QString urlstr)
{
	bool blashslash = false;

	QString urlstr2 = urlstr;
	if (urlstr2.indexOf("?")!= -1)
		urlstr2 = urlstr2.mid(0, urlstr2.indexOf("?"));
	QString urlpram = urlstr;
	if (urlpram.indexOf("?")!= -1)
		urlpram = urlpram.mid(urlpram.indexOf("?") + 1);


	QStringList strls = urlstr2.split("/");
	if (urlstr2.endsWith("/"))blashslash = true;
	QString urlstrok;
	for (int i = 0; i < strls.size(); i++)
	{
		if (urlstrok.indexOf("://") == -1)
		{
			urlstrok += strls[i] + "/";
		}
		else if (urlstrok.indexOf("/", urlstrok.indexOf("://")) == -1)
		{
			urlstrok += strls[i] + "/";
		}
		else{
			urlstrok += QByteArray::fromPercentEncoding(strls[i].toUtf8()) + "/";
		}
	}
	if (blashslash == false)
	{
		urlstrok = urlstrok.mid(0, urlstrok.size() - 1);
	}

	QStringList paramseg = urlpram.split("&");
	for (int i = 0; i < paramseg.size(); i++)
	{
		if (paramseg[i].indexOf("=") != -1)
		{
			QString name = paramseg[i].mid(0, paramseg[i].indexOf("=")).trimmed();
			QString value = paramseg[i].mid(paramseg[i].indexOf("=") + 1);

			if (name != "")
			{
				if (i == 0)
				{
					urlstrok += "?";
					urlstrok += QByteArray::fromPercentEncoding(name.toUtf8()) + "=" + QByteArray::fromPercentEncoding(value.toUtf8());
				}
				else{
					urlstrok += "&" + QByteArray::fromPercentEncoding(name.toUtf8()) + "=" + QByteArray::fromPercentEncoding(value.toUtf8());
				}
			}
		}
	}

	return urlstrok;
}
