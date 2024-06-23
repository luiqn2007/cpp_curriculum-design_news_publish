#pragma once
#include "DbData.h"

/**
 * 用户
 */
class User :
	public DbData
{
	string username_;
	bool is_admin_;

public:
	User(const int id, string username, const bool is_admin)
		: DbData(id),
		  username_(std::move(username)),
		  is_admin_(is_admin)
	{
	}

	/**
	 * @return 用户名
	 */
	string username() const
	{
		return username_;
	}

	/**
	 * @return 用户是否为管理员
	 */
	bool is_admin() const
	{
		return is_admin_;
	}
};
