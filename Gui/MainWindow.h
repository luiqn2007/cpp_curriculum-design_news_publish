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

	void set_news_like(int news_id, bool is_like);

public slots:
	/**
	 * ��������
	 */
	void publish_news();

	/**
	 * ��һҳ
	 */
	void prev_page();

	/**
	 * ��һҳ
	 */
	void next_page();

	/**
	 * ��¼��ע�ᡢ�˳�
	 */
	void login_or_logout();

	/**
	 * �鿴����
	 */
	void view_news(QListWidgetItem* item);

	/**
	 * �л���������
	 */
	void reload_type_news();

	/**
	 * �л��鵵�������
	 */
	void reload_archived_news();

	/**
	 * �л�����ҳ��
	 */
	void reload_page_news();

	/**
	 * �л�ÿҳ����
	 */
	void reload_count_news();

	/**
	 * ��������
	 */
	void search_news();

	/**
	 * ���б��Ҽ��˵�
	 * @param pos ���λ��
	 */
	void open_right_menu(QPoint pos);

	/**
	 * �༭����
	 */
	void edit_news();

	/**
	 * ɾ������
	 */
	void delete_news();

private:
	Ui::MainWindowClass ui;

	QMenu* list_menu_;
	vector<News> news_;
	QListWidgetItem* option_item_;

	void reload_news();

	void apply_news();

	static int get_int_from_widget(QLineEdit* editor, int default_count, bool set_to_editor = false);

	static int get_int_from_widget(QLabel* editor, int default_count, bool set_to_editor = false);

	static void set_widget_visible(int n, bool visible, ...);
};
