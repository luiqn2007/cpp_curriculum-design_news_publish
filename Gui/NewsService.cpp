#include "NewsService.h"

#include <mysql/jdbc.h>

#include "Common.h"
#include "Session.h"

using namespace std;

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

/*
*design by chenxiang
*/
News NewsService::get_news_by_id(int id)
{
	string empty;
	stringstream sql;
	sql << "new_id='" << id << "'";
	auto schema = session->conn->getDefaultSchema();
	auto table = schema.getTable("news", true);
	auto likes_result = schema.getTable("likes", true);
	int like_count = likes_result.select("count(*)").where(sql.str()).execute().fetchOne()[0].get<int>();
	auto select_result = table.select("*").
	where(sql.str()).execute();
	auto result=select_result.fetchOne();
	string title = result[1].get<string>();
	string author = result[2].get<string>();
	string content = result[3].get<string>();
	string publish = result[4].get<string>();
	return News(id, title, author, content, publish, like_count);
}

ServiceResult NewsService::save_news(News& news)
{
	try
	{
		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("news", true);
		auto table_types = schema.getTable("news_types", true);
		for (auto type : news.types())
		{
			table_types.insert("news_id", "type_id")
				.values(news.id(), type.id()).execute();
		}
		 table.insert("news_id", "title", "author", "news", "publish").
			values(news.id(), news.title(), news.author(), news.content(), news.publish_date()).execute();
	}
	catch (mysqlx::Error& e)
	{
		return { e.what() };
	}
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
