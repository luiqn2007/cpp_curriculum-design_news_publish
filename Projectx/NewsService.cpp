#include "NewsService.h"

vector<News> NewsService::get_latest_news(int page, int count, int& total)
{
	// TODO
	return vector<News>();
}

vector<News> NewsService::get_news_by_type(int type, int page, int count, int& total)
{
	// TODO
	return vector<News>();
}

vector<News> NewsService::get_archived_news(int year, int page, int count, int& total)
{
	// TODO
	return vector<News>();
}

vector<News> NewsService::get_hot_news(int count)
{
	// TODO
	return vector<News>();
}

News NewsService::get_news_by_id(int id)
{
	// TODO
	string empty;
	return News(0, empty, empty, empty, empty, 0);
}

ServiceResult NewsService::save_news(News& news)
{
	// TODO
	return {};
}

ServiceResult NewsService::update_news(News& news)
{
	// TODO
	return {};
}

ServiceResult NewsService::delete_news(News& news)
{
	// TODO
	return {};
}
