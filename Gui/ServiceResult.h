#pragma once

#include <string>

using std::string;

class ServiceResult
{
public:
	bool success;
	string err;

	/**
	 * 执行成功
	 */
	ServiceResult(): success(true)
	{
	}

	/**
	 * 执行失败
	 * @param err 错误信息
	 */
	ServiceResult(string err): success(false), err(std::move(err))
	{
	}
};
