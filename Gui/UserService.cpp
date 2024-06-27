#include "UserService.h"

#include <format>

#include "Common.h"
#include "Session.h"

using std::format;

UserService::UserService()
{
	unknown_user_ = new User(0, "unknown", false);
}

UserService::~UserService()
{
	if (unknown_user_)
	{
		delete unknown_user_;
		unknown_user_ = nullptr;
	}
}

ServiceResult UserService::register_user(string username, string password, bool is_admin)
{
	try
	{
		// 检查用户名与密码是否为空
		if (username.empty() || password.empty())
		{
			return { lang->get_value("name_pwd_empty", "Empty username or password") };
		}

		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("users", true);

		// 检查用户名是否已存在
		auto result = table.select("*").where(format("username = '{}'", username)).execute();
		if (result.count() > 0)
		{
			return {lang->get_value("username_existed", "Username is existed")};
		}

		// 插入数据
		auto insert_result = table.insert("username", "password", "is_admin")
		                          .values(username, password, is_admin)
		                          .execute();
		return insert_result.getAffectedItemsCount()
			       ? ServiceResult{}
		           // 影响条数为 0 - 没有插入数据
			       : ServiceResult{lang->get_value("db_insert_failed", "Database insert failed")};
	}
	catch (mysqlx::Error& e)
	{
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
	// 未登录，直接返回 false
	if (!session->user)
	{
		return false;
	}

	try
	{
		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("news_types", true);

		// 点赞数据
		auto sql = format("news_id = {} and user_id = {}", news.id(), session->user->id());

		// 查询是否已经点赞
		if (table.select(sql).execute().count() > 0)
		{
			// 取消点赞
			table.remove().where(sql).execute();
			return false;
		}
		// 点赞
		table.insert("news_id", "user_id")
		     .values(news.id(), session->user->id())
		     .execute();
		return true;
	}
	catch (mysqlx::Error& e)
	{
		// 执行错误 直接返回 false
		return false;
	}
}

User* UserService::get_user_by_id(const int id)
{
	// 检查本地缓存
	if (user_cache_.contains(id))
	{
		return user_cache_[id];
	}

	auto schema = session->conn->getDefaultSchema();
	auto table = schema.getTable("users");
	// 查询用户信息
	auto rows = table.select("*").where(format("user_id = {}", id)).execute();
	if (rows.count())
	{
		// 数据保存至本地缓存
		auto d = rows.fetchOne();
		user_cache_[id] = new User(id, d[1].get<string>(), d[3].get<bool>());
		return user_cache_[id];
	}
	// 未找到：返回未知用户
	return unknown_user_;
}
