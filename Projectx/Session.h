#pragma once

#include <string>
#include <sstream>

#include "Property.h"
#include "User.h"

#include <mysqlx/xdevapi.h>

using std::string;
using std::stringstream;

/**
 * 模拟一个会话
 */
class Session
{
public:
	/**
	 * 当前登录的用户，未登录则为 nullptr
	 */
	User* user = nullptr;

	/**
	 * 当前数据库链接
	 */
	mysqlx::Session *conn;

	Session()
	{
		Property property;
		property.read("config.property");

		stringstream url_stream;
		const string host = property.get_value("host", "127.0.0.1");
		unsigned port = std::stoul(property.get_value("port", "33060"));
		const string username = property.get_value("username", "root");
		const string password = property.get_value("password", "root");
		const string database = property.get_value("database", "news_system");
		try
		{
			conn = new mysqlx::Session(mysqlx::SessionSettings(host, port, username, password, database));
		}
		catch (mysqlx::Error &e)
		{
			printf("%s", e.what());
			exit(0);
		}
	}

	~Session()
	{
		if (conn)
		{
			conn->close();
			delete conn;
			conn = nullptr;
		}
	}
};
