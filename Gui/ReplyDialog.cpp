#include "ReplyDialog.h"

#include <qdatetime.h>
#include <QMessageBox>

#include "Common.h"
#include "Session.h"
#include "CommentService.h"
#include "NewsDetailDialog.h"
#include "NewsService.h"

ReplyDialog::ReplyDialog(Comment* comment, int reply_to, QWidget* parent)
	: QDialog(parent), comment_(comment), news_(nullptr), reply_to_(reply_to)
{
	ui.setupUi(this);
}

ReplyDialog::ReplyDialog(News* news, QWidget* parent)
	: QDialog(parent), comment_(nullptr), news_(news), reply_to_(0)
{
	ui.setupUi(this);
}

void ReplyDialog::reply()
{
	if (!session->user)
	{
		QMessageBox::critical(this, lang->qt("error"), lang->qt("error_status"));
		reject();
	}
	else
	{
		string rep = ui.te_reply->toPlainText().toStdString();
		string publish_date = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
		if (comment_)
		{
			const Comment comment(0, rep, publish_date, *session->user, comment_->news(), comment_);
			if (auto result = comment_service->comment(comment); !result.success)
			{
				QMessageBox::critical(this, lang->qt("error"), QString::fromStdString(result.err));
				return;
			}
			dynamic_cast<NewsDetailDialog*>(parent())->insert_comment(comment, reply_to_);
		}
		else
		{
			const Comment comment(0, rep, publish_date, *session->user, news_, nullptr);
			if (auto result = comment_service->comment(comment); !result.success)
			{
				QMessageBox::critical(this, lang->qt("error"), QString::fromStdString(result.err));
				return;
			}
			dynamic_cast<NewsDetailDialog*>(parent())->insert_comment(comment, reply_to_);
		}
		accept();
	}
}
