#pragma once

#include <QDialog>

#include "Comment.h"
#include "ui_NewsDetailDialog.h"

class NewsDetailDialog : public QDialog
{
	Q_OBJECT

public:
	NewsDetailDialog(News* news, QWidget* parent = nullptr);
	~NewsDetailDialog() override = default;

	/**
	 * 重载评论
	 */
	void reload_comments();

public slots:
	/**
	 * 切换点赞状态
	 */
	void toggle_like();

	/**
	 * 编辑新闻
	 */
	void edit_news();

	/**
	 * 删除新闻
	 */
	void delete_news();

	/**
	 * 回复新闻
	 */
	void comment_news();

private:
	Ui::NewsDetailDialogClass ui;
	News* news_;

	void reload_news();

	int insert_comment(int index, Comment& comment, int reply_to);
};
