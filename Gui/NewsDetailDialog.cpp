#include "NewsDetailDialog.h"

#include <QMessageBox>

#include "CommentService.h"
#include "Common.h"
#include "MainWindow.h"
#include "NewsEditDialog.h"
#include "ReplyDialog.h"
#include "Session.h"
#include "UserService.h"

NewsDetailDialog::NewsDetailDialog(News* news, QWidget* parent)
	: QDialog(parent), news_(news)
{
	ui.setupUi(this);
	if (!session->user)
	{
		ui.pb_comment->setVisible(false);
		ui.pb_comment->setEnabled(false);
	}
	// news
	ui.lbl_title->setText(QString::fromStdString(news_->title()));
	ui.lbl_author->setText(QString::fromStdString(news_->author()));
	ui.lbl_date->setText(QString::fromStdString(news_->publish_date()));
	ui.lbl_like_count->setText(QString::number(news_->like_count()));
	ui.tb_news->setText(QString::fromStdString(news_->content()));
	const bool is_like = user_service->is_login_user_like(*news_);
	const string icon_res = (*lang)[is_like ? "img_like" : "img_unlike"];
	ui.pb_like->setIcon(QIcon(QPixmap::fromImage(QImage(icon_res.c_str()))));
	// comments
	for (auto comments = comment_service->get_comment_by_news(*news_); Comment& comment : comments)
	{
		insert_comment(comment);
	}
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
			ui.pb_like->setIcon(QIcon(QPixmap::fromImage(QImage((*lang)["img_like"].c_str()))));
		}
		else
		{
			ui.lbl_like_count->setText(QString::number(now_like_count - 1));
			ui.pb_like->setIcon(QIcon(QPixmap::fromImage(QImage((*lang)["img_unlike"].c_str()))));
		}
		dynamic_cast<MainWindow*>(parent())->set_news_like(news_->id(), is_like);
	}
}

void NewsDetailDialog::insert_comment(Comment comment, const int reply_to)
{
	const auto index = comments_.size();
	comments_.push_back(comment);

	auto item = new QListWidgetItem;
	item->setText(QString::fromStdString(format("#{} {}", index + 1, comment.content())));
	if (reply_to > 0)
	{
		item->setToolTip(QString::fromStdString(format("author {} at {} reply #{}", comment.publisher(), comment.publish_date(), reply_to)));
	}
	else
	{
		item->setToolTip(QString::fromStdString(format("author {} at {}", comment.publisher(), comment.publish_date())));
	}
	item->setData(Qt::UserRole, QVariant((int) index));
	ui.lw_comment->addItem(item);

	for (Comment &rep_comment : comment.replies())
	{
		insert_comment(rep_comment, index + 1);
	}
}

void NewsDetailDialog::comment_news()
{
	ReplyDialog(news_, this).exec();
}

void NewsDetailDialog::comment_comment(QListWidgetItem* item)
{
	if (session->user)
	{
		const int i = get<int>(item->data(Qt::UserRole));
		ReplyDialog(&comments_[i], i + 1, this).exec();
	}
}
