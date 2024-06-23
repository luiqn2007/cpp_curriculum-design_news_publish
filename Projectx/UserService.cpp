#include "UserService.h"

#include <sstream>

#include "Common.h"
#include "Session.h"

using std::stringstream;

ServiceResult UserService::register_user(string username, string password, bool is_admin)
{
	stringstream ss;
	ss << "username = '" << username << "'";
	try
	{
		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("users", true);
		auto result = table.select("*").where(ss.str()).execute();
		if (result.count() > 0)
		{
			return {"用户名已存在"};
		}

		auto insert_result = table.insert("username", "password", "is_admin")
		                          .values(username, password, is_admin)
		                          .execute();
		return insert_result.getAffectedItemsCount()
			       ? ServiceResult{}
			       : ServiceResult{"数据库插入失败"};
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
		stringstream ss;
		ss << "username = '" << username << "' and password = '" << password << "'";
		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("users");
		for (auto d : table.select("*").where(ss.str()).execute())
		{
			int id = d[0].get<int>();
			auto uname = d[1].get<string>();
			bool is_admin = d[3].get<bool>();
			session->user = new User(id, uname, is_admin);
			return {};
		}
		return {"用户名或密码错误"};
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
			stringstream ss;
			ss << "news_id = " << news.id() << " and user_id = " << session->user->id();
			auto schema = session->conn->getDefaultSchema();
			is_like = schema.getTable("news_types", true)
			                .select(ss.str())
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

bool UserService::toggle_login_user_like(News& news)
{
	if (!session->user)
	{
		return false;
	}

	try
	{
		stringstream ss;
		ss << "news_id = " << news.id() << " and user_id = " << session->user->id();
		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("news_types", true);
		if (table.select(ss.str()).execute().count() > 0)
		{
			table.remove().where(ss.str()).execute();
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
