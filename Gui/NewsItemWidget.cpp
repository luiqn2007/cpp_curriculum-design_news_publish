#include "NewsItemWidget.h"

NewsItemWidget::NewsItemWidget(News* news, QWidget* parent)
	: QWidget(parent), news_(news)
{
	ui.setupUi(this);
	ui.lbl_author->setText(QString::fromStdString(news->author()));
	ui.lbl_date->setText(QString::fromStdString(news->publish_date()));
	ui.lbl_title->setText(QString::fromStdString(news->title()));
}