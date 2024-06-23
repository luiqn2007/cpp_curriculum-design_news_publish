#include "NewsEditDialog.h"

#include <sstream>

#include "Common.h"
#include "NewsService.h"
#include "TypeService.h"

using std::stringstream;

NewsEditDialog::NewsEditDialog(News* news, QWidget* parent)
	: QDialog(parent), news_(news)
{
	ui.setupUi(this);
	ui.le_title->setText(QString::fromStdString(news->title()));
	stringstream ss_type;
	if (!news->types().empty())
	{
		auto p = news->types().cbegin();
		ss_type << p->type();
		++p;
		while (p != news->types().cend())
		{
			ss_type << "; " << p->type();
			++p;
		}
	}
	ui.le_types->setText(QString::fromStdString(ss_type.str()));
	ui.te_news->setText(QString::fromStdString(news->content()));
}

void NewsEditDialog::save()
{
	const string title = ui.le_title->text().toStdString();
	const string content = ui.te_news->toPlainText().toStdString();
	news_->set_title(title);
	news_->set_content(content);
	auto &types = news_->types();
	types.clear();
	for (const auto& type : ui.le_types->text().split(";"))
	{
		string type_str = type.trimmed().toStdString();
		if (!type_str.empty())
		{
			types.push_back(type_service->get_or_create_by_name(type_str));
		}
	}
	news_service->update_news(*news_);
	accept();
}
