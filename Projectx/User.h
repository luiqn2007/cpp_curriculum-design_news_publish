#pragma once
#include "DbData.h"

/**
 * �û�
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
	 * @return �û���
	 */
	string username() const
	{
		return username_;
	}

	/**
	 * @return �û��Ƿ�Ϊ����Ա
	 */
	bool is_admin() const
	{
		return is_admin_;
	}
};
