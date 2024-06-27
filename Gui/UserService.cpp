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
		// ����û����������Ƿ�Ϊ��
		if (username.empty() || password.empty())
		{
			return {(*lang)["name_pwd_empty"]};
		}

		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("users", true);

		// ����û����Ƿ��Ѵ���
		auto result = table.select("*").where(format("username = '{}'", username)).execute();
		if (result.count() > 0)
		{
			return {(*lang)["username_existed"]};
		}

		// ��������
		auto insert_result = table.insert("username", "password", "is_admin")
		                          .values(username, password, is_admin)
		                          .execute();
		return insert_result.getAffectedItemsCount()
			       ? ServiceResult{}
			       : ServiceResult{(*lang)["db_insert_failed"]};
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
		auto rows = table.select("*").where(format("username = '{}' and password = '{}'", username, password)).
		                  execute();
		for (auto d : rows)
		{
			int id = d[0].get<int>();
			auto uname = d[1].get<string>();
			bool is_admin = d[3].get<bool>();
			session->user = new User(id, uname, is_admin);
			return {};
		}
		return {(*lang)["error_user_pwd"]};
	}
	catch (mysqlx::Error& e)
	{
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
			is_like = session->conn->getDefaultSchema()
			                 .getTable("likes", true)
			                 .select("*")
			                 .where(format("news_id = {} and user_id = {}", news.id(), session->user->id()))
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
	// δ��¼��ֱ�ӷ��� false
	if (!session->user)
	{
		return false;
	}

	try
	{
		auto schema = session->conn->getDefaultSchema();
		auto table = schema.getTable("likes", true);

		// ��������
		auto sql = format("news_id = {} and user_id = {}", news.id(), session->user->id());

		// ��ѯ�Ƿ��Ѿ�����
		auto result = table.select("*").where(sql).execute();
		if (result.count() > 0)
		{
			// ȡ������
			table.remove().where(sql).execute();
			return false;
		}
		// ����
		table.insert("news_id", "user_id")
		     .values(news.id(), session->user->id())
		     .execute();
		return true;
	}
	catch (mysqlx::Error& e)
	{
		// ִ�д��� ֱ�ӷ��� false
		return false;
	}
}

User* UserService::get_user_by_id(const int id)
{
	// ��鱾�ػ���
	if (user_cache_.contains(id))
	{
		return user_cache_[id];
	}

	auto schema = session->conn->getDefaultSchema();
	auto table = schema.getTable("users");
	// ��ѯ�û���Ϣ
	auto rows = table.select("*").where(format("user_id = {}", id)).execute();
	if (rows.count())
	{
		// ���ݱ��������ػ���
		auto d = rows.fetchOne();
		user_cache_[id] = new User(id, d[1].get<string>(), d[3].get<bool>());
		return user_cache_[id];
	}
	// δ�ҵ�������δ֪�û�
	return unknown_user_;
}
