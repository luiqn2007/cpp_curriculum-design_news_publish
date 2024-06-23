#pragma once

#include <QDialog>
#include "ui_PublishNewsDialog.h"

class PublishNewsDialog : public QDialog
{
	Q_OBJECT

public:
	PublishNewsDialog(QWidget* parent = nullptr);
	~PublishNewsDialog() override = default;

public slots:
	/**
	 * 发布新闻
	 */
	void publish();

private:
	Ui::PublishNewsDialogClass ui;
};
