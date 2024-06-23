#pragma once

#include <QWidget>
#include "ui_NewsItemWidget.h"
#include "News.h"

class NewsItemWidget : public QWidget
{
	Q_OBJECT

public:
	NewsItemWidget(News *news, QWidget *parent = nullptr);
	~NewsItemWidget() = default;

	News *news()
	{
		return news_;
	}

private:
	Ui::NewsItemWidgetClass ui;
	News* news_;
};
