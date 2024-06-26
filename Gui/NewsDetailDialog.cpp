#include "NewsDetailDialog.h"

#include <QMessageBox>

#include "CommentService.h"
#include "CommentWidget.h"
#include "Common.h"
#include "NewsEditDialog.h"
#include "ReplyDialog.h"
#include "Session.h"
#include "UserService.h"
#include "NewsService.h"

NewsDetailDialog::NewsDetailDialog(News* news, QWidget* parent)
	: QDialog(parent), news_(news)
{
	ui.setupUi(this);
	reload_news();
}

void NewsDetailDialog::reload_news()
{
	ui.lbl_title->setText(QString::fromStdString(news_->title()));
	ui.lbl_author->setText(QString::fromStdString(news_->author()));
	ui.lbl_date->setText(QString::fromStdString(news_->publish_date()));
	ui.lbl_like_count->setText(QString::number(news_->like_count()));
	ui.tb_news->setText(QString::fromStdString(news_->content()));

	ui.pb_edit->setVisible(session->user && session->user->is_admin());
	ui.pb_edit->setEnabled(session->user && session->user->is_admin());
	ui.pb_delete->setVisible(session->user && session->user->is_admin());
	ui.pb_delete->setEnabled(session->user && session->user->is_admin());

	const bool is_like = user_service->is_login_user_like(*news_);
	const QBitmap like = QBitmap::fromImage(QImage(":/images/like.png"));
	const QBitmap unlike = QBitmap::fromImage(QImage(":/images/unlike.png"));
	ui.pb_like->setIcon(QIcon(is_like ? like : unlike));

	reload_comments();
}

void NewsDetailDialog::toggle_like()
{
	if (session->user)
	{
		const bool is_like = user_service->toggle_login_user_like(*news_);
		const int now_like_count = ui.lbl_like_count->text().toInt();
		if (is_like)
		{
			ui.lbl_like_count->setText(QString::number(now_like_count + 1));
			ui.pb_like->setIcon(QIcon(QBitmap::fromImage(QImage(":/images/like.png"))));
		}
		else
		{
			ui.lbl_like_count->setText(QString::number(now_like_count - 1));
			ui.pb_like->setIcon(QIcon(QBitmap::fromImage(QImage(":/images/unlike.png"))));
		}
	}
	else
	{
		QMessageBox::warning(this, "ÌáÊ¾", "ÇëÏÈµÇÂ¼");
	}
}

void NewsDetailDialog::reload_comments()
{
	auto comments = comment_service->get_comment_by_news(*news_);
	const auto count = comments.size();
	int index = 1;
	for (Comment& comment : comments)
	{
		index = insert_comment(index, comment, 0);
	}
}

int NewsDetailDialog::insert_comment(const int index, Comment& comment, const int reply_to)
{
	int next_index = index;
	const auto item = new QListWidgetItem();
	ui.lw_comment->addItem(item);
	ui.lw_comment->setItemWidget(item, new CommentWidget(next_index++, reply_to, &comment, this));
	for (Comment &rep_comment : comment.replies())
	{
		next_index = insert_comment(next_index, rep_comment, index);
	}
	return next_index;
}

void NewsDetailDialog::edit_news()
{
	if (!session->user)
	{
		QMessageBox::critical(this, 
			QString::fromStdString(lang->get_value("edit_news", "Edit News")), 
			QString::fromStdString(lang->get_value("error_status", "Invalid login status")));
	}
	else if (!session->user->is_admin())
	{
		QMessageBox::critical(this,
			QString::fromStdString(lang->get_value("edit_news", "Edit News")),
			QString::fromStdString(lang->get_value("only_admin_can_edit", "Only administrator can edit news")));
	}
	else
	{
		done(NewsEditDialog(news_, dynamic_cast<QWidget*>(parent())).exec());
	}
}

void NewsDetailDialog::delete_news()
{
	if (!session->user)
	{
		QMessageBox::critical(this,
			QString::fromStdString(lang->get_value("delete_news", "Delete News")),
			QString::fromStdString(lang->get_value("error_status", "Invalid login status")));
	}
	else if (!session->user->is_admin())
	{
		QMessageBox::critical(this,
			QString::fromStdString(lang->get_value("delete_news", "Delete News")),
			QString::fromStdString(lang->get_value("only_admin_can_edit", "Only administrator can edit news")));
	}
	else
	{
		const auto result = news_service->delete_news(*news_);
		if (result.success)
		{
			QMessageBox::information(this,
				QString::fromStdString(lang->get_value("delete_news", "Delete News")),
				QString::fromStdString(lang->get_value("deleted", "News is deleted")));
			accept();
		}
		else
		{
			QMessageBox::critical(this,
				QString::fromStdString(lang->get_value("delete_news", "Delete News")),
				QString::fromStdString(result.err));
		}
	}
}

void NewsDetailDialog::comment_news()
{
	if (ReplyDialog(news_, this).exec())
	{
		reload_comments();
	}
}
