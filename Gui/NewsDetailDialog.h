#pragma once

#include <QDialog>

#include "Comment.h"
#include "ui_NewsDetailDialog.h"

class NewsDetailDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewsDetailDialog(News* news, QWidget* parent = nullptr);
	~NewsDetailDialog() override = default;
	
	/**
	 * ��������
	 * @param comment ��������
	 * @param reply_to �ظ����۱�ţ�0 ��ʾ���ǻظ�
	 */
	void insert_comment(Comment comment, int reply_to = 0);

public slots:
	/**
	 * �л�����״̬
	 */
	void toggle_like();

	/**
	 * ��������
	 */
	void comment_news();

	/**
	 * �ظ�����
	 */
	void comment_comment(QListWidgetItem* item);

private:
	Ui::NewsDetailDialogClass ui;
	News* news_;
	vector<Comment> comments_;
};
