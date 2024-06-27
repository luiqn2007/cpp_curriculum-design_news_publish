#pragma once
#include <utility>
#include <vector>

#include "DbData.h"
#include "User.h"

using std::vector;

class News;

/**
 * 评论和回复
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
	 * @return 评论内容
	 */
	string content() const
	{
		return content_;
	}

	/**
	 * @return 发布时间
	 */
	string publish_date() const
	{
		return publish_;
	}

	/**
	 * @return 发布用户名
	 */
	string publisher() const
	{
		return user_.username();
	}

	/**
	 * @return 被评论的新闻
	 */
	News* news() const
	{
		return news_;
	}

	/**
	 * 若该评论为另一个评论的回复，则返回指向另一个评论的指针，否则为 nullptr
	 * @return 被回复的评论或 nullptr
	 */
	Comment* reply_to() const
	{
		return reply_to_;
	}

	/**
	 * @return 该评论的所有回复
	 */
	vector<Comment>& replies()
	{
		return replies_;
	}
};
