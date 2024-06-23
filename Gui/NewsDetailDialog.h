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

	void toggle_like();

	void edit_news();

	void comment_news();

	void reload_comments();

private:
	Ui::NewsDetailDialogClass ui;
	News* news_;

	void reload_news();

	int insert_comment(int index, Comment& comment, int reply_to);
};
