#pragma once
#include <vector>

#include "Type.h"

using std::vector;

class TypeService
{
public:
	/**
	 * 获取所有新闻类型
	 */
	vector<Type> get_types();

	/**
	 * 根据类型名称，创建或获取现有类型
	 * @param type_name 类型名称
	 */
	Type get_or_create_by_name(string type_name);
};

