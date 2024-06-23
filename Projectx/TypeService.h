#pragma once
#include <vector>

#include "Type.h"

using std::vector;

class TypeService
{
public:
	/**
	 * ��ȡ������������
	 */
	vector<Type> get_types();

	/**
	 * �����������ƣ��������ȡ��������
	 * @param type_name ��������
	 */
	Type get_or_create_by_name(string type_name);
};

