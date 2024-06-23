#include "CommentWidget.h"

#include <format>

#include <qboxlayout.h>

#include "Common.h"
#include "Property.h"
#include "ReplyDialog.h"
#include "NewsDetailDialog.h"

using std::format;

CommentWidget::CommentWidget(int index, int reply, Comment* comment, NewsDetailDialog* dialog, QWidget* parent)
	: QWidget(parent), comment_(comment), dialog_(dialog)
{
	setFixedWidth(280);

	const auto layout = new QGridLayout();

	layout->addWidget(new QLabel(QString::fromStdString(format("#{}", index))), 0, 0);
	layout->addWidget(new QLabel(QString::fromStdString(comment->publisher())), 0, 1);
	layout->addWidget(new QLabel(QString::fromStdString(comment->publish_date())), 0, 2);

	const auto browser = new QTextBrowser();
	browser->setText(QString::fromStdString(comment->content()));
	layout->addWidget(browser, 1, 0, 1, 3);

	if (reply)
	{
		const auto lb_reply = new QLabel(QString::fromStdString(format("#{}", reply)));
		lb_reply->setPixmap(QBitmap(":/images/reply.png"));
		layout->addWidget(lb_reply, 2, 0);
	}

	const auto btn_reply = new QPushButton(QString::fromStdString(lang->get_value("reply", "Reply")));
	connect(btn_reply, SIGNAL(clicked()), this, SLOT(reply()));
	layout->addWidget(btn_reply, 2, 2);

	setLayout(layout);
}

void CommentWidget::reply()
{
	if (ReplyDialog(comment_, this).exec())
	{
		dialog_->reload_comments();
	}
}
