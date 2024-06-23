#pragma once

#include <QDialog>

#include "Comment.h"
#include "ui_ReplyDialog.h"

class ReplyDialog : public QDialog
{
	Q_OBJECT

public:
	ReplyDialog(Comment *comment, QWidget* parent = nullptr);
	ReplyDialog(News *news, QWidget *parent = nullptr);
	~ReplyDialog() override = default;

	void reply();

private:
	Ui::ReplyDialogClass ui;
	Comment* comment_;
	News* news_;
};
