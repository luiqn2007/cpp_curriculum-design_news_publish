#pragma once
#include <utility>
#include <vector>

#include "DbData.h"
#include "User.h"

using std::vector;

class News;

/**
 * ���ۺͻظ�
 */
class Comment :
	public DbData
{
	string content_;
	string publish_;
	User user_;
	News* news_;
	Comment* reply_to_;
	vector<Comment> replies_;

public:
	Comment(const int id, string content, string publish, User user, News* news,
	        Comment* reply_to)
		: DbData(id),
		  content_(std::move(content)),
		  publish_(std::move(publish)),
		  user_(std::move(user)),
		  news_(news),
		  reply_to_(reply_to)
	{
	}

	/**
	 * @return ��������
	 */
	string content() const
	{
		return content_;
	}

	/**
	 * @return ����ʱ��
	 */
	string publish_date() const
	{
		return publish_;
	}

	/**
	 * @return �����û���
	 */
	string publisher() const
	{
		return user_.username();
	}

	/**
	 * @return �����۵�����
	 */
	News* news() const
	{
		return news_;
	}

	/**
	 * ��������Ϊ��һ�����۵Ļظ����򷵻�ָ����һ�����۵�ָ�룬����Ϊ nullptr
	 * @return ���ظ������ۻ� nullptr
	 */
	Comment* reply_to() const
	{
		return reply_to_;
	}

	/**
	 * @return �����۵����лظ�
	 */
	vector<Comment>& replies()
	{
		return replies_;
	}
};
