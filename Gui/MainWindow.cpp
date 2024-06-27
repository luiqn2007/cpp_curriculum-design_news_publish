#include "MainWindow.h"

#include <cstdarg>
#include <QMessageBox>
#include <QMenu>

#include "Common.h"
#include "LoginDialog.h"
#include "NewsDetailDialog.h"
#include "NewsEditDialog.h"
#include "Session.h"
#include "NewsService.h"
#include "PublishNewsDialog.h"
#include "TypeService.h"
#include "UserService.h"

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

	list_menu_ = new QMenu;
	list_menu_->addAction(lang->qt("edit"), this, SLOT(edit_news()));
	list_menu_->addAction(lang->qt("delete"), this, SLOT(delete_news()));
}

void MainWindow::set_news_like(int news_id, bool is_like)
{
	for (int i = 0; i < ui.lst_news->count(); ++i)
	{
		const auto item = ui.lst_news->item(i);
		if (news_id == news_[get<int>(item->data(Qt::UserRole))].id())
		{
			string icon_res = (*lang)[is_like ? "img_like" : "img_unlike"];
			item->setIcon(QIcon(QPixmap::fromImage(QImage(icon_res.c_str()))));
		}
	}
}

void MainWindow::publish_news()
{
	if (!session->user)
	{
		QMessageBox::critical(this, lang->qt("publish_news"), lang->qt("login_first"));
	}
	else if (!session->user->is_admin())
	{
		QMessageBox::critical(this, lang->qt("publish_news"), lang->qt("only_admin_can_publish"));
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
		QMessageBox::information(this, lang->qt("logout"), lang->qt("logout_success"));
		ui.pb_login->setText(lang->qt("login_or_register"));
		ui.lbl_user->setText(lang->qt("tourist"));
		ui.pb_publish->setEnabled(false);
		ui.pb_publish->setVisible(false);
	}
	else
	{
		LoginDialog().exec();
		if (session->user)
		{
			ui.pb_login->setText(lang->qt("logout"));
			ui.lbl_user->setText(QString::fromStdString(session->user->username()));
			if (session->user->is_admin())
			{
				ui.pb_publish->setEnabled(true);
				ui.pb_publish->setVisible(true);
			}
		}
		apply_news();
	}
}

void MainWindow::view_news(QListWidgetItem* item)
{
	if (NewsDetailDialog(&news_[item->listWidget()->currentRow()], this).exec())
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
	set_widget_visible(8, false, ui.lbl_year, ui.de_year, ui.pb_prev, ui.pb_next, ui.le_page, ui.lbl_page_selector,
	                   ui.lbl_page, ui.lbl_page_total);

	news_ = news_service->search_news(ui.le_search->text().toStdString());
	apply_news();
}

void MainWindow::open_right_menu(QPoint pos)
{
	if (session->user && session->user->is_admin())
	{
		option_item_ = ui.lst_news->itemAt(pos);
		if (option_item_)
		{
			list_menu_->exec(pos + ui.lst_news->pos() + this->geometry().topLeft());
		}
	}
}

void MainWindow::edit_news()
{
	if (!session->user)
	{
		QMessageBox::critical(this, lang->qt("edit_news"), lang->qt("error_status"));
	}
	else if (!session->user->is_admin())
	{
		QMessageBox::critical(this, lang->qt("edit_news"), lang->qt("only_admin_can_edit"));
	}
	else
	{
		const auto i = get<int>(option_item_->data(Qt::UserRole));
		if (NewsEditDialog(&news_[i], dynamic_cast<QWidget*>(parent())).exec())
		{
			option_item_->setData(Qt::DisplayRole, QString::fromStdString(news_[i].title()));
		}
	}
	option_item_ = nullptr;
}

void MainWindow::delete_news()
{
	if (!session->user)
	{
		QMessageBox::critical(this, lang->qt("delete_news"), lang->qt("error_status"));
	}
	else if (!session->user->is_admin())
	{
		QMessageBox::critical(this, lang->qt("delete_news"), lang->qt("only_admin_can_edit"));
	}
	else
	{
		const auto i = get<int>(option_item_->data(Qt::UserRole));
		const auto result = news_service->delete_news(news_[i]);
		if (result.success)
		{
			QMessageBox::information(this, lang->qt("delete_news"), lang->qt("deleted"));
		}
		else
		{
			QMessageBox::critical(this, lang->qt("delete_news"), QString::fromStdString(result.err));
		}
		option_item_ = nullptr;
		reload_news();
	}
}

void MainWindow::reload_news()
{
	// 读取每页数据量
	int count = get_int_from_widget(ui.le_count, 10, true);
	// 根据不同类型类型更新数据
	if (ui.cb_type->currentIndex() == 0 /* 热门 */)
	{
		news_ = news_service->get_hot_news(count);
		if (news_.empty())
		{
			int tt;
			auto list = news_service->get_latest_news(1, count, tt);
			news_.insert(news_.end(), list.begin(), list.end());
		}
	}
	else if (ui.cb_type->currentIndex() == 1 /* 全部 */)
	{
		int current_page = get_int_from_widget(ui.le_page, 1, true);
		int total_page = 1;
		news_ = news_service->get_latest_news(current_page, count, total_page);
		ui.lbl_page->setText(QString::number(total_page));
		ui.pb_prev->setEnabled(current_page > 1);
		ui.pb_next->setEnabled(current_page < total_page);
	}
	else if (ui.cb_type->currentIndex() == 2 /* 归档 */)
	{
		int year = ui.de_year->date().year();
		int current_page = get_int_from_widget(ui.le_page, 1, true);
		int total_page = 1;
		news_ = news_service->get_archived_news(year, current_page, count, total_page);
		ui.lbl_page->setText(QString::number(total_page));
		ui.pb_prev->setEnabled(current_page > 1);
		ui.pb_next->setEnabled(current_page < total_page);
	}
	else /* 其他类别 */
	{
		int type_id = ui.cb_type->currentData().toInt();
		int current_page = get_int_from_widget(ui.le_page, 1, true);
		int total_page = 1;
		news_ = news_service->get_news_by_type(type_id, current_page, count, total_page);
		ui.lbl_page->setText(QString::number(total_page));
		ui.pb_prev->setEnabled(current_page > 1);
		ui.pb_next->setEnabled(current_page < total_page);
	}
	apply_news();
}

void MainWindow::apply_news()
{
	// 清空已有数据
	ui.lst_news->clear();
	// 将数据显示到列表中
	for (int i = 0; i < news_.size(); ++i)
	{
		QListWidgetItem* item = new QListWidgetItem();
		News& news = news_[i];
		item->setText(QString::fromStdString(news.title()));
		item->setData(Qt::UserRole, QVariant(i));
		item->setToolTip(QString::fromStdString(format("author {} publish at {}", news.author(), news.publish_date())));
		if (user_service->is_login_user_like(news))
		{
			item->setIcon(QIcon(QPixmap::fromImage(QImage((*lang)["img_like"].c_str()))));
		}
		else
		{
			item->setIcon(QIcon(QPixmap::fromImage(QImage((*lang)["img_unlike"].c_str()))));
		}
		ui.lst_news->addItem(item);
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
