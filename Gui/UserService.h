#pragma once
#include <string>
#include <unordered_map>

#include "News.h"
#include "ServiceResult.h"

using std::string;
using std::unordered_map;

class UserService
{
	unordered_map<int, User*> user_cache_;
	User* unknown_user_;

public:
	UserService();
	~UserService();

	/**
	 * 注册用户
	 * @param username 用户名
	 * @param password 密码
	 * @param is_admin 是否是管理员
	 * @return 注册结果
	 */
	ServiceResult register_user(string username, string password, bool is_admin);

	/**
	 * 登录用户
	 * @param username 用户名
	 * @param password 密码
	 * @return 登录结果
	 */
	ServiceResult login(string username, string password);

	/**
	 * 查询当前登录的用户是否点赞了某个新闻
	 * @param news 查询新闻
	 * @return 是否点赞
	 */
	bool is_login_user_like(News& news);

	/**
	 * 切换当前登录的用户对该新闻点赞的状态
	 * @param news 新闻
	 * @return 切换后，用户是否点赞了该新闻
	 */
	bool toggle_login_user_like(const News& news);

	/**
	 * 根据用户编号获取用户
	 * @param id 用户编号
	 * @return 用户
	 */
	User* get_user_by_id(int id);
};
