#pragma once

#include <QDialog>

#include "Comment.h"
#include "ui_NewsDetailDialog.h"

class NewsDetailDialog : public QDialog
{
	Q_OBJECT

public:
	NewsDetailDialog(News* news, QWidget* parent = nullptr);
	~NewsDetailDialog() override = default;

	/**
	 * ��������
	 */
	void reload_comments();

public slots:
	/**
	 * �л�����״̬
	 */
	void toggle_like();

	/**
	 * �༭����
	 */
	void edit_news();

	/**
	 * ɾ������
	 */
	void delete_news();

	/**
	 * �ظ�����
	 */
	void comment_news();

private:
	Ui::NewsDetailDialogClass ui;
	News* news_;

	void reload_news();

	int insert_comment(int index, Comment& comment, int reply_to);
};
