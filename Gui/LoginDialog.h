#pragma once

#include <QDialog>
#include "ui_LoginDialog.h"

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	LoginDialog(QWidget *parent = nullptr);
	~LoginDialog() override = default;

	void login();

	void register_user();

private:
	Ui::LoginDialogClass ui;
};
