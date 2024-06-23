#pragma once
#include <vector>
#include "DbData.h"
#include "Type.h"
#include "Comment.h"

using std::vector;

/**
 * ����
 */
class News :
	public DbData
{
	string title_, author_, content_, publish_;
	vector<Type> types_ = vector<Type>();
	vector<Comment> comments_ = vector<Comment>();
	int like_count_;

public:
	News(const int id, string title, string author, string content, string publish, const int like_count)
		: DbData(id),
		  title_(std::move(title)),
		  author_(std::move(author)),
		  content_(std::move(content)),
		  publish_(std::move(publish)),
		  like_count_(like_count)
	{
	}

	/**
	 * ���ű���
	 */
	[[nodiscard]] string title() const
	{
		return title_;
	}

	/**
	 * ��������
	 */
	[[nodiscard]] string author() const
	{
		return author_;
	}

	/**
	 * ��������
	 */
	[[nodiscard]] string content() const
	{
		return content_;
	}

	/**
	 * ��������
	 */
	vector<Type>& types()
	{
		return types_;
	}

	/**
	 * ��������
	 */
	vector<Comment>& comments()
	{
		return comments_;
	}

	/**
	 * ���ŷ���ʱ��
	 */
	[[nodiscard]] string publish_date() const
	{
		return publish_;
	}

	/**
	 * Ϊ�����ŵ��޵��û�����
	 */
	[[nodiscard]] int like_count() const
	{
		return like_count_;
	}

	void set_title(const string& title)
	{
		title_ = title;
	}

	void set_content(const string& content)
	{
		content_ = content;
	}
};
