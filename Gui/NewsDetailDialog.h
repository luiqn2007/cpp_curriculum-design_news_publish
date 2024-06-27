#pragma once

#include <QDialog>

#include "Comment.h"
#include "ui_NewsDetailDialog.h"

class NewsDetailDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewsDetailDialog(News* news, QWidget* parent = nullptr);
	~NewsDetailDialog() override = default;
	
	/**
	 * 插入评论
	 * @param comment 评论内容
	 * @param reply_to 回复评论编号，0 表示不是回复
	 */
	void insert_comment(Comment comment, int reply_to = 0);

public slots:
	/**
	 * 切换点赞状态
	 */
	void toggle_like();

	/**
	 * 评论新闻
	 */
	void comment_news();

	/**
	 * 回复评论
	 */
	void comment_comment(QListWidgetItem* item);

private:
	Ui::NewsDetailDialogClass ui;
	News* news_;
	vector<Comment> comments_;
};
