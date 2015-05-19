#include "htmlutils.h"

using namespace qf::core::utils;

QString HtmlUtils::fromHtmlList(const QVariantList &body_list, const HtmlUtils::FromHtmlListOptions &options)
{
	QString html_header =
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
		"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
		"	<head>\n"
		"		<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n"
		"		<title>{{documentTitle}}</title>\n"
		"		<style type=\"text/css\">\n"
		"			body {font-family: Verdana, sans-serif}\n"
		"			h1, h2, h3, h4, h5 {\n"
		"				color: black;\n"
		"				font-family: \"Lucida Grande\", Verdana, Lucida, Helvetica, Arial, sans-serif;\n"
		"				font-size: 100%;\n"
		"				font-weight: normal;\n"
		"				clear: left;\n"
		"			}\n"
		"			h1 {font-size: 220%; font-weight: bold;}\n"
		"			h2 {font-size: 150%; font-weight: bold; border-bottom: 1px  dotted #666 ;}\n"
		"			h3 {font-size: 120%;  }\n"
		"			.text {text-align:left}\n"
		"			.code {background-color: lightgray; text-align:left}\n"
		"\n"
		"			table { border: 1px solid gray; border-spacing: 0px 0px;}\n"
		"			tr.odd { background-color: lavender; }\n"
		"			th { padding: 3px; border: 0px solid #53bc1f; background-color: #91e369; }\n"
		"			td { padding: 3px; border: 0px solid #53bc1f; }\n"
		"		</style>\n"
		"	</head>\n";
	QString body = fromHtmlList_helper(body_list, QString(), options);
	QString html_footer = "</html>";
	QString ret = html_header + body + html_footer;
	ret = qf::core::Utils::replaceCaptions(ret, options);
	return ret;
}

QString HtmlUtils::fromHtmlList_helper(const QVariant &item, const QString &indent, const HtmlUtils::FromHtmlListOptions &options)
{
	QString ret;
	Q_UNUSED(options)
	if(item.type() == QVariant::List) {
		QVariantList lst = item.toList();
		QF_ASSERT(!lst.isEmpty(), "Bad item list!", return ret);
		QString element_name = lst.first().toString();
		QF_ASSERT(!element_name.isEmpty(), "Bad element name!", return ret);
		QString attrs_str;
		int ix = 1;
		QVariant attrs = lst.value(ix);
		if(attrs.type() == QVariant::Map) {
			QVariantMap m = attrs.toMap();
			QMapIterator<QString, QVariant> it(m);
			while(it.hasNext()) {
				it.next();
				attrs_str += ' ' + it.key() + '=' + '"' + it.value().toString() + '"';
			}
			ix++;
		}
		bool has_children = (ix < lst.count());
		if(has_children) {
			ret += '\n' + indent;
			ret += '<' + element_name + attrs_str + '>';
			QString indent2 = indent + '\t';
			bool has_child_elemet = false;
			for (; ix < lst.count(); ++ix) {
				QVariant v = lst[ix];
				if(!v.toList().isEmpty())
					has_child_elemet = true;
				ret += fromHtmlList_helper(v, indent2, options) + ' ';
			}
			if(has_child_elemet)
				ret += '\n' + indent;
			ret += "</" + element_name + '>';
		}
		else {
			ret += '<' + element_name + '>';
		}
	}
	else {
		ret = item.toString();
	}
	return ret;
}
