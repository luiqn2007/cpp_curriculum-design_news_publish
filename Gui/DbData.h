#pragma once

#include <string>

using std::string;

/**
 * ���ݿ����ݶ���
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
	 * @return ���� id
	 */
	[[nodiscard]] int id() const
	{
		return id_;
	}
};
