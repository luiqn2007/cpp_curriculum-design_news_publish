#pragma once

#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

/**
 * ��ȡ properties ������Ϣ
 */
class Property
{
	unordered_map<string, string> properties_;

public:
	/**
	 * �����ļ�
	 * @param file �ļ�·��
	 */
	void read(string file);

	/**
	 * ��ȡ����
	 * @param key ��
	 * @param default_value ��������ʱ��Ĭ��ֵ
	 * @return ֵ
	 */
	string get_value(string key, string default_value);
};
