#include "LoginDialog.h"

#include <QMessageBox>

#include "Common.h"
#include "Property.h"
#include "UserService.h"

LoginDialog::LoginDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

void LoginDialog::login()
{
	string username = ui.le_username->text().toStdString();
	string password = ui.le_password->text().toStdString();
	auto reg = user_service->login(username, password);
	if (reg.success)
	{
		accept();
	}
	else
	{
		QMessageBox::critical(this, 
			QString::fromStdString(lang->get_value("login_failed", "Login Failed")), 
			QString::fromStdString(reg.err));
	}
}

void LoginDialog::register_user()
{
	string username = ui.le_username->text().toStdString();
	string password = ui.le_password->text().toStdString();
	bool is_admin = QMessageBox::question(this, 
		QString::fromStdString(lang->get_value("register", "Register")), 
		QString::fromStdString(lang->get_value("is_admin", "Is Admin?"))) == QMessageBox::Yes;
	auto reg = user_service->register_user(username, password, is_admin);
	if (reg.success)
	{
		login();
	}
	else
	{
		QMessageBox::critical(this, 
			QString::fromStdString(lang->get_value("register_failed", "Register Failed")), 
			QString::fromStdString(reg.err));
	}
}
