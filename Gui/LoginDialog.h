#pragma once

#include <QDialog>
#include "ui_LoginDialog.h"

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	LoginDialog(QWidget *parent = nullptr);
	~LoginDialog() override = default;

public slots:
	/**
	 * µÇÂ¼
	 */
	void login();

	/**
	 * ×¢²á
	 */
	void register_user();

private:
	Ui::LoginDialogClass ui;
};
