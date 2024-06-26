#include "UserService.h"

#include <format>

#include "Common.h"
#include "Session.h"

using std::format;

ServiceResult UserService::register_user(string username, string password, bool is_admin)
{
	try
	{
		if (username.empty() || password.empty())
		{
			return { lang->get_value("name_pwd_empty", "Empty username or password") };
		}
		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("users", true);
		auto result = table.select("*").where(format("username = '{}'", username)).execute();
		if (result.count() > 0)
		{
			return {lang->get_value("username_existed", "Username is existed")};
		}

		auto insert_result = table.insert("username", "password", "is_admin")
		                          .values(username, password, is_admin)
		                          .execute();
		return insert_result.getAffectedItemsCount()
			       ? ServiceResult{}
			       : ServiceResult{lang->get_value("db_insert_failed", "Database insert failed")};
	}
	catch (mysqlx::Error& e)
	{
		printf_s("%s\n", e.what());
		return {e.what()};
	}
}

ServiceResult UserService::login(string username, string password)
{
	try
	{
		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("users");
		auto rows = table.select("*").where(format("username = '{}' and password = '{}'", username, password)).execute();
		for (auto d : rows)
		{
			int id = d[0].get<int>();
			auto uname = d[1].get<string>();
			bool is_admin = d[3].get<bool>();
			session->user = new User(id, uname, is_admin);
			return {};
		}
		return {lang->get_value("error_user_pwd", "Error: Wrong username or password")};
	}
	catch (mysqlx::Error& e)
	{
		printf_s("%s\n", e.what());
		return {e.what()};
	}
}

bool UserService::is_login_user_like(News& news)
{
	bool is_like = false;
	if (session->user)
	{
		try
		{
			auto schema = session->conn->getDefaultSchema();
			is_like = schema.getTable("news_types", true)
			                .select(format("news_id = {} and user_id = {}", news.id(), session->user->id()))
			                .execute()
			                .count() > 0;
		}
		catch (mysqlx::Error& e)
		{
			printf_s("%s\n", e.what());
		}
	}
	return is_like;
}

bool UserService::toggle_login_user_like(const News& news)
{
	if (!session->user)
	{
		return false;
	}

	try
	{
		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("news_types", true);
		auto sql = format("news_id = {} and user_id = {}", news.id(), session->user->id());
		if (table.select(sql).execute().count() > 0)
		{
			table.remove().where(sql).execute();
			return false;
		}
		return true;
	}
	catch (mysqlx::Error& e)
	{
		printf_s("%s\n", e.what());
		return false;
	}
}

User* UserService::get_user_by_id(const int id)
{
	if (user_cache_.count(id))
	{
		return user_cache_[id];
	}

	auto schema = session->conn->getDefaultSchema();
	auto table = schema.getTable("users");
	auto rows = table.select("*").where(format("user_id = {}", id)).execute();
	auto d = rows.fetchOne();
	user_cache_[id] = new User(id, d[1].get<string>(), d[3].get<bool>());
	return user_cache_[id];
}
