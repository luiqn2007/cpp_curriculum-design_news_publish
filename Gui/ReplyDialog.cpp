#include "ReplyDialog.h"

#include <format>
#include <qdatetime.h>
#include <QMessageBox>

#include "Common.h"
#include "Session.h"
#include "CommentService.h"
#include "NewsService.h"

using std::format;

ReplyDialog::ReplyDialog(Comment* comment, QWidget* parent)
	: QDialog(parent), comment_(comment), news_(nullptr)
{
	ui.setupUi(this);
}

ReplyDialog::ReplyDialog(News* news, QWidget* parent)
	: QDialog(parent), comment_(nullptr), news_(news)
{
	ui.setupUi(this);
}

void ReplyDialog::reply()
{
	if (!session->user)
	{
		QMessageBox::critical(this, 
			QString::fromStdString(lang->get_value("error", "Error")), 
			QString::fromStdString(lang->get_value("error_status", "Invalid login status")));
		reject();
	}
	else
	{
		string rep = ui.te_reply->toPlainText().toStdString();
		QDate now = QDate::currentDate();
		string publish_date = QDate::currentDate().toString("yyyy/MM/dd").toStdString();
		if (comment_)
		{
			const Comment comment(0, rep, publish_date, *session->user, comment_->news(), comment_);
			comment_service->comment(comment);
		}
		else
		{
			const Comment comment(0, rep, publish_date, *session->user, news_, nullptr);
			comment_service->comment(comment);
		}
		accept();
	}
	
}

