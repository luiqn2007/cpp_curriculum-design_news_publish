#pragma once

#include <QWidget>

#include "Comment.h"

class NewsDetailDialog;

class CommentWidget : public QWidget
{
	Q_OBJECT

public:
	CommentWidget(int index, int reply, Comment* comment, NewsDetailDialog* dialog, QWidget* parent = nullptr);
	~CommentWidget() override = default;

public slots:

	/**
	 * »Ø¸´
	 */
	void reply();

private:
	Comment* comment_;
	NewsDetailDialog* dialog_;
};
