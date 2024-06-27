#include "MainWindow.h"

#include <cstdarg>
#include <QMessageBox>

#include "Common.h"
#include "LoginDialog.h"
#include "NewsDetailDialog.h"
#include "NewsItemWidget.h"
#include "Session.h"
#include "NewsService.h"
#include "PublishNewsDialog.h"
#include "TypeService.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.pb_publish->setVisible(false);
	ui.pb_publish->setEnabled(false);

	auto types = type_service->get_types();
	for (auto& type : types)
	{
		ui.cb_type->addItem(QString::fromStdString(type.type()), QVariant(type.id()));
	}
	this->reload_type_news();
}

void MainWindow::publish_news()
{
	if (!session->user)
	{
		QMessageBox::critical(this,
		                      QString::fromStdString(lang->get_value("publish_news", "Publish News")),
		                      QString::fromStdString(lang->get_value("login_first", "Login First Please")));
	}
	else if (!session->user->is_admin())
	{
		QMessageBox::critical(this,
		                      QString::fromStdString(lang->get_value("publish_news", "Publish News")),
		                      QString::fromStdString(
			                      lang->get_value("only_admin_can_publish", "Only administrator can publish news")));
	}
	else
	{
		PublishNewsDialog dialog(this);
		dialog.exec();
	}
}

void MainWindow::prev_page()
{
	int current_page = get_int_from_widget(ui.le_page, 1) - 1;
	if (current_page < 1)
	{
		current_page = 1;
	}
	ui.le_page->setText(QString::number(current_page));
	reload_page_news();
}

void MainWindow::next_page()
{
	int current_page = get_int_from_widget(ui.le_page, 0) + 1;
	int max_page = get_int_from_widget(ui.lbl_page, 1);
	if (current_page > max_page)
	{
		current_page = max_page;
	}
	if (current_page < 1)
	{
		current_page = 1;
	}
	ui.le_page->setText(QString::number(current_page));
	reload_page_news();
}

void MainWindow::login_or_logout()
{
	if (session->user)
	{
		session->user = nullptr;
		QMessageBox::information(this,
		                         QString::fromStdString(lang->get_value("logout", "Logout")),
		                         QString::fromStdString(lang->get_value("logout_success", "Logout Succeed")));
		ui.pb_login->setText(QString::fromStdString(lang->get_value("login_or_register", "Login/Register")));
		ui.lbl_user->setText(QString::fromStdString(lang->get_value("tourist", "Tourist")));
		ui.pb_publish->setEnabled(false);
		ui.pb_publish->setVisible(false);
	}
	else
	{
		LoginDialog().exec();
		if (session->user)
		{
			ui.pb_login->setText(QString::fromStdString(lang->get_value("logout", "Logout")));
			ui.lbl_user->setText(QString::fromStdString(session->user->username()));
			if (session->user->is_admin())
			{
				ui.pb_publish->setEnabled(true);
				ui.pb_publish->setVisible(true);
			}
		}
	}
}

void MainWindow::view_news(QListWidgetItem* item)
{
	const int row = item->listWidget()->currentRow();
	if (NewsDetailDialog(&news[row], this).exec())
	{
		reload_news();
	}
}

void MainWindow::reload_type_news()
{
	if (ui.cb_type->currentIndex() == 0 /* 热门 */)
	{
		set_widget_visible(8, false, ui.lbl_year, ui.de_year, ui.pb_prev, ui.pb_next, ui.le_page, ui.lbl_page_selector,
		                   ui.lbl_page, ui.lbl_page_total);
	}
	else if (ui.cb_type->currentIndex() == 2 /* 归档 */)
	{
		set_widget_visible(8, true, ui.lbl_year, ui.de_year, ui.pb_prev, ui.pb_next, ui.le_page, ui.lbl_page_selector,
		                   ui.lbl_page, ui.lbl_page_total);
		ui.de_year->setDate(QDate::currentDate());
		ui.le_page->setText("1");
		ui.lbl_page->setText("1");
		ui.pb_prev->setEnabled(false);
		ui.pb_next->setEnabled(false);
	}
	else
	{
		set_widget_visible(2, false, ui.lbl_year, ui.de_year);
		set_widget_visible(6, true, ui.pb_prev, ui.pb_next, ui.le_page, ui.lbl_page_selector, ui.lbl_page,
		                   ui.lbl_page_total);
		ui.le_page->setText("1");
		ui.lbl_page->setText("1");
		ui.pb_prev->setEnabled(false);
		ui.pb_next->setEnabled(false);
	}
	reload_news();
}

void MainWindow::reload_archived_news()
{
	ui.le_page->setText("1");
	ui.lbl_page->setText("1");
	reload_news();
}

void MainWindow::reload_page_news()
{
	reload_news();
}

void MainWindow::reload_count_news()
{
	ui.le_page->setText("1");
	ui.lbl_page->setText("1");
	ui.pb_prev->setEnabled(false);
	ui.pb_next->setEnabled(false);
	reload_news();
}

void MainWindow::search_news()
{
	ui.lst_news->clear();
	string key_words = ui.le_search->text().toStdString();
	set_widget_visible(8, false, ui.lbl_year, ui.de_year, ui.pb_prev, ui.pb_next, ui.le_page, ui.lbl_page_selector,
	                   ui.lbl_page, ui.lbl_page_total);

	news = news_service->search_news(key_words);
	for (auto& News : news)
	{
		QListWidgetItem* item = new QListWidgetItem();
		ui.lst_news->addItem(item);
		ui.lst_news->setItemWidget(item, new NewsItemWidget(&News));
	}
}

void MainWindow::reload_news()
{
	// 清空已有数据
	ui.lst_news->clear();
	// 读取每页数据量
	int count = get_int_from_widget(ui.le_count, 10, true);
	// 根据不同类型类型更新数据
	if (ui.cb_type->currentIndex() == 0 /* 热门 */)
	{
		news = news_service->get_hot_news(count);
	}
	else if (ui.cb_type->currentIndex() == 1 /* 全部 */)
	{
		int current_page = get_int_from_widget(ui.le_page, 1, true);
		int total_page = 1;
		news = news_service->get_latest_news(current_page, count, total_page);
		ui.lbl_page->setText(QString::number(total_page));
		ui.pb_prev->setEnabled(current_page > 1);
		ui.pb_next->setEnabled(current_page < total_page);
	}
	else if (ui.cb_type->currentIndex() == 2 /* 归档 */)
	{
		int year = ui.de_year->date().year();
		int current_page = get_int_from_widget(ui.le_page, 1, true);
		int total_page = 1;
		news = news_service->get_archived_news(year, current_page, count, total_page);
		ui.lbl_page->setText(QString::number(total_page));
		ui.pb_prev->setEnabled(current_page > 1);
		ui.pb_next->setEnabled(current_page < total_page);
	}
	else /* 其他类别 */
	{
		int type_id = ui.cb_type->currentData().toInt();
		int current_page = get_int_from_widget(ui.le_page, 1, true);
		int total_page = 1;
		news = news_service->get_news_by_type(type_id, current_page, count, total_page);
		ui.lbl_page->setText(QString::number(total_page));
		ui.pb_prev->setEnabled(current_page > 1);
		ui.pb_next->setEnabled(current_page < total_page);
	}
	// 将数据显示到列表中
	for (auto& News : news)
	{
		QListWidgetItem* item = new QListWidgetItem();
		ui.lst_news->addItem(item);
		ui.lst_news->setItemWidget(item, new NewsItemWidget(&News));
	}
}

int MainWindow::get_int_from_widget(QLineEdit* editor, int default_count, bool set_to_editor)
{
	bool is_int;
	const int value = editor->text().toInt(&is_int);

	if (is_int)
	{
		return value;
	}

	if (set_to_editor)
	{
		editor->setText(QString::number(default_count));
	}
	return default_count;
}

int MainWindow::get_int_from_widget(QLabel* editor, int default_count, bool set_to_editor)
{
	bool is_int;
	const int value = editor->text().toInt(&is_int);

	if (is_int)
	{
		return value;
	}

	if (set_to_editor)
	{
		editor->setText(QString::number(default_count));
	}
	return default_count;
}

void MainWindow::set_widget_visible(int n, bool visible, ...)
{
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++)
	{
		QWidget* widget = va_arg(args, QWidget*);
		widget->setVisible(visible);
	}
	va_end(args);
}
