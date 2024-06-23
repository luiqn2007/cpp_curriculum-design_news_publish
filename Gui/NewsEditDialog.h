#pragma once

#include <QDialog>

#include "News.h"
#include "ui_NewsEditDialog.h"

class NewsEditDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewsEditDialog(News* news, QWidget* parent = nullptr);
	~NewsEditDialog() override = default;

	void save();

private:
	Ui::NewsEditDialogClass ui;

	News* news_;
};
