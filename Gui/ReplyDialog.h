#pragma once

#include <QDialog>

#include "Comment.h"
#include "ui_ReplyDialog.h"

class ReplyDialog : public QDialog
{
	Q_OBJECT

public:
	ReplyDialog(Comment *comment, int reply_to, QWidget* parent = nullptr);
	explicit ReplyDialog(News *news, QWidget *parent = nullptr);
	~ReplyDialog() override = default;

public slots:
	/**
	 * »Ø¸´
	 */
	void reply();

private:
	Ui::ReplyDialogClass ui;
	Comment* comment_;
	News* news_;
	int reply_to_;
};
