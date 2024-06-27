#include "PublishNewsDialog.h"

#include <vector>
#include <QMessageBox>

#include "Common.h"
#include "MainWindow.h"
#include "NewsService.h"
#include "News.h"
#include "Session.h"
#include "TypeService.h"

using std::vector;

PublishNewsDialog::PublishNewsDialog(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.de_publish->setDate(QDate::currentDate());
	ui.le_autor->setText(QString::fromStdString(session->user->username()));
}

void PublishNewsDialog::publish()
{
	if (!session->user)
	{
		QMessageBox::critical(this, lang->qt("publish"), lang->qt("error_status"));
	}
	else if (!session->user->is_admin())
	{
		QMessageBox::critical(this, lang->qt("publish"), lang->qt("insufficient_permissions"));
	}
	else if (ui.le_title->text().isEmpty())
	{
		QMessageBox::critical(this, lang->qt("publish"), lang->qt("no_title"));
	}
	else
	{
		string title = ui.le_title->text().toStdString();
		string author = ui.le_autor->text().toStdString();
		string date = ui.de_publish->date().toString("yyyy-MM-dd").toStdString();
		string content = ui.te_news->toPlainText().toStdString();
		News news(0, title, author, content, date, 0);
		for (const auto& type : ui.le_types->text().split(";"))
		{
			string type_str = type.trimmed().toStdString();
			if (!type_str.empty())
			{
				news.types().push_back(type_service->get_or_create_by_name(type_str));
			}
		}
		auto result = news_service->save_news(news);
		if (result.success)
		{
			close();
			return;
		}
		QMessageBox::critical(this, lang->qt("publish"), QString::fromStdString(result.err));
	}
}
