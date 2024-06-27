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
	 * ע���û�
	 * @param username �û���
	 * @param password ����
	 * @param is_admin �Ƿ��ǹ���Ա
	 * @return ע����
	 */
	ServiceResult register_user(string username, string password, bool is_admin);

	/**
	 * ��¼�û�
	 * @param username �û���
	 * @param password ����
	 * @return ��¼���
	 */
	ServiceResult login(string username, string password);

	/**
	 * ��ѯ��ǰ��¼���û��Ƿ������ĳ������
	 * @param news ��ѯ����
	 * @return �Ƿ����
	 */
	bool is_login_user_like(News& news);

	/**
	 * �л���ǰ��¼���û��Ը����ŵ��޵�״̬
	 * @param news ����
	 * @return �л����û��Ƿ�����˸�����
	 */
	bool toggle_login_user_like(const News& news);

	/**
	 * �����û���Ż�ȡ�û�
	 * @param id �û����
	 * @return �û�
	 */
	User* get_user_by_id(int id);
};
