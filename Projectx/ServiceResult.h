#pragma once

#include <string>

using std::string;

class ServiceResult
{
public:
	bool success;
	string err;

	/**
	 * ִ�гɹ�
	 */
	ServiceResult(): success(true)
	{
	}

	/**
	 * ִ��ʧ��
	 * @param err ������Ϣ
	 */
	ServiceResult(string err): success(false), err(std::move(err))
	{
	}
};
