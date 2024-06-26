#pragma once

#include <mysqlx/xdevapi.h>

#include "Comment.h"
#include "ServiceResult.h"

class CommentService
{
public:
	/**
	 * 保存回复
	 * @param comment 回复信息
	 * @return 保存结果
	 */
	ServiceResult comment(const Comment& comment);

	/**
	 * 获取某个新闻的评论
	 * @param news 新闻信息
	 * @return 评论信息
	 */
	vector<Comment> get_comment_by_news(News& news);

private:

	void get_sub_comments(Comment& parent, mysqlx::Table &table);
};
