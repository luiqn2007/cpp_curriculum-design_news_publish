#pragma once
#include <vector>
#include "DbData.h"
#include "Type.h"
#include "Comment.h"

using std::vector;

/**
 * 新闻
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
	 * 新闻标题
	 */
	[[nodiscard]] string title() const
	{
		return title_;
	}

	/**
	 * 新闻作者
	 */
	[[nodiscard]] string author() const
	{
		return author_;
	}

	/**
	 * 新闻内容
	 */
	[[nodiscard]] string content() const
	{
		return content_;
	}

	/**
	 * 新闻类型
	 */
	vector<Type>& types()
	{
		return types_;
	}

	/**
	 * 新闻评论
	 */
	vector<Comment>& comments()
	{
		return comments_;
	}

	/**
	 * 新闻发布时间
	 */
	[[nodiscard]] string publish_date() const
	{
		return publish_;
	}

	/**
	 * 为该新闻点赞的用户数量
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
