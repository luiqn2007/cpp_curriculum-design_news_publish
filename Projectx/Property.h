#pragma once

#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

/**
 * 读取 properties 配置信息
 */
class Property
{
	unordered_map<string, string> properties_;

public:
	/**
	 * 加载文件
	 * @param file 文件路径
	 */
	void read(string file);

	/**
	 * 读取数据
	 * @param key 键
	 * @param default_value 键不存在时的默认值
	 * @return 值
	 */
	string get_value(string key, string default_value);
};
