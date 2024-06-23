#pragma once

#include <string>

using std::string;

/**
 * 数据库数据对象
 */
class DbData
{
protected:
	int id_;

public:
	explicit DbData(const int id): id_(id)
	{
	}

	/**
	 * @return 对象 id
	 */
	[[nodiscard]] int id() const
	{
		return id_;
	}
};
