#pragma once

#include <string>
#include <vector>

#include "News.h"
#include "ui_MainWindow.h"

using std::string;
using std::vector;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override = default;

public slots:

	/**
	 * 发布新闻
	 */
	void publish_news();

	/**
	 * 上一页
	 */
	void prev_page();

	/**
	 * 下一页
	 */
	void next_page();

	/**
	 * 登录、注册、退出
	 */
	void login_or_logout();

	/**
	 * 查看新闻
	 */
	void view_news(QListWidgetItem* item);

	/**
	 * 切换新闻类型
	 */
	void reload_type_news();

	/**
	 * 切换归档新闻年号
	 */
	void reload_archived_news();

	/**
	 * 切换新闻页码
	 */
	void reload_page_news();

	/**
	 * 切换每页条数
	 */
	void reload_count_news();

	/**
	 * 搜索新闻
	 */
	void search_news();

private:
	Ui::MainWindowClass ui;
	vector<News> news;

	void reload_news();

	static int get_int_from_widget(QLineEdit* editor, int default_count, bool set_to_editor = false);

	static int get_int_from_widget(QLabel* editor, int default_count, bool set_to_editor = false);

	static void set_widget_visible(int n, bool visible, ...);
};
