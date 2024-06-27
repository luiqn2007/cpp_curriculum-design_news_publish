#include "NewsService.h"

#include <mysql/jdbc.h>

#include "Common.h"
#include "Session.h"

using namespace std;

vector<News> NewsService::get_latest_news(int page, int count, int& total)
{
	// TODO
	auto schema = session->conn->getDefaultSchema();
	auto table = schema.getTable("news", true);
	auto rows = table.select("*").orderBy("publish desc").limit(count).offset(count * (page - 1)).execute();

	auto sum_page = table.select("*").execute().count();
	total = ceil(sum_page*1.0 / count) ;

	vector<News>latest_news;
	for (auto row : rows) {
		auto likes_table = schema.getTable("likes", true);

		stringstream sql;
		sql << "news_id=" << row[0].get<int>() << "";

		auto likes_count = likes_table.select("*")
			.where(sql.str())
			.execute()
			.count();
		latest_news.emplace_back(row[0].get<int>(), row[1].get<string>(), row[2].get<string>(), row[3].get<string>(), row[4].get<string>(), likes_count);
	}
	return latest_news;
}

vector<News> NewsService::get_news_by_type(int type, int page, int count, int& total)
{
	// TODO
	stringstream sql;
	sql << "type_id=" << type << "";
	auto schema = session->conn->getDefaultSchema();
	auto news_types_table = schema.getTable("news_types", true);

	auto total_count_result = news_types_table.select("*").where(sql.str()).execute().count();
	total = ceil(total_count_result *1.0/count);

	auto rows = news_types_table.select("*").where(sql.str()).limit(count).offset(count * (page - 1)).execute();

	vector<News>NewsByType;
	for (auto row : rows) {
		auto news_table = schema.getTable("news", true);
		stringstream sql;
		sql << "news_id=" << row[0].get<int>() << "";

		auto Rows = news_table.select("*").where(sql.str()).execute().fetchOne();

		auto likes_table = schema.getTable("likes", true);
		auto likes_count = likes_table.select("*")
			.where(sql.str())
			.execute()
			.count();

		NewsByType.emplace_back(Rows[0].get<int>(), Rows[1].get<string>(), Rows[2].get<string>(), Rows[3].get<string>(), Rows[4].get<string>(), likes_count);
	}
	return NewsByType;
}
vector<News> NewsService::get_archived_news(int year, int page, int count, int& total)
{
	// TODO

	auto schema = session->conn->getDefaultSchema();
	auto news_table = schema.getTable("news", true);

	auto result = news_table.select("*").where(format("publish between '{}-01-01' and '{}-12-31'", year, year));
	auto total1=result.execute().count();
	total = ceil(total1*1.0/ count);
	
	auto rows = result.limit(count).offset(count * (page - 1)).execute();

	vector<News>NewsType;
	for (auto row : rows) {
	
		stringstream sql;
		sql << "news_id=" << row[0].get<int>() << "";

		auto Rows = news_table.select("*").where(sql.str()).execute().fetchOne();

		auto likes_table = schema.getTable("likes", true);
		auto likes_count = likes_table.select("*")
			.where(sql.str())
			.execute()
			.count();

		NewsType.emplace_back(Rows[0].get<int>(), Rows[1].get<string>(), Rows[2].get<string>(), Rows[3].get<string>(), Rows[4].get<string>(), likes_count);
	}
	return NewsType;
}


vector<News> NewsService::get_hot_news(int count)
{
	// TODO
	auto schema = session->conn->getDefaultSchema();
	auto table = schema.getTable("likes", true);
	auto Rows = table.select("news_id", "count(*)").groupBy("news_id").orderBy("count(*) desc").execute();
	vector<News>hot_news;
	while (count--) {
		for (auto row : Rows) {
			auto likes_count = row[1].get<int>();
			auto news_table = schema.getTable("news", true);
			stringstream sql;
			sql << "news_id=" << row[0].get<int>() << "";

			auto rows1 = news_table.select("*").where(sql.str()).execute().fetchOne();
			hot_news.emplace_back(rows1[0].get<int>(), rows1[1].get<string>(), rows1[2].get<string>(), rows1[3].get<string>(), rows1[4].get<string>(), likes_count);
		}
	}
	return hot_news;
}

/*
*design by chenxiang
*/
News NewsService::get_news_by_id(int id)
{
	string empty;
	stringstream sql;
	sql << "news_id='" << id << "'";
	auto schema = session->conn->getDefaultSchema();
	auto table = schema.getTable("news", true);
	auto likes_result = schema.getTable("likes", true);
	int like_count = likes_result.select("count(*)").where(sql.str()).execute().fetchOne()[0].get<int>();
	auto select_result = table.select("*").
		where(sql.str()).execute();
	auto result = select_result.fetchOne();
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

		auto result=table.insert("title", "author", "news", "publish").
			values(news.title(), news.author(), news.content(), news.publish_date()).execute();
		int id=result.getAutoIncrementValue();
		for (auto type : news.types())
		{
			table_types.insert("news_id", "type_id")
				.values(id, type.id()).execute();
		}
		return ServiceResult{};
	}
	catch (mysqlx::Error& e)
	{
		return { e.what() };
	}
}

ServiceResult NewsService::update_news(News& news)
{
	try
	{
		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("news", true);
		table.update().
			set("title",news.title()).
			where(format("news_id={}",news.id())).execute();
		table.update().
			set("author", news.author()).
			where(format("news_id={}", news.id())).execute();
		table.update().
			set("news", news.content()).
			where(format("news_id={}", news.id())).execute();
		table.update().
			set("publish", news.publish_date()).
			where(format("news_id={}", news.id())).execute();
		return ServiceResult{};
	}
	catch (mysqlx::Error& e)
	{
		return { e.what() };
	}
}

ServiceResult NewsService::delete_news(News& news)
{
	try
	{
		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("news", true);
		table.remove().where(format("news_id={}", news.id())).execute();
		return ServiceResult{};
	}
	catch (mysqlx::Error& e)
	{
		return { e.what() };
	}
}

vector<News> NewsService::search_news(string key)
{
	auto schema = session->conn->getDefaultSchema();
	auto table = schema.getTable("news", true);
	auto table_likes = schema.getTable("likes", true);
	auto results=table.select("*").where(format("news like '%{}%'", key)).execute();

	vector<News> search;
	for (auto result : results)
	{
		int like_count = table_likes.select("*").where(format("news_id={}", result[0].get<int>())).execute().count();
		search.emplace_back(result[0].get<int>(), result[1].get<string>(), result[2].get<string>(), result[3].get<string>(), result[4].get<string>(),like_count);
	}
	return search;
}