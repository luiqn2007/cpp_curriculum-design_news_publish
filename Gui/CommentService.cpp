#include "CommentService.h"

#include <format>

#include "Common.h"
#include "News.h"
#include "Session.h"
#include "UserService.h"

using std::format;

ServiceResult CommentService::comment(const Comment& comment)
{
	try
	{
		if (session->user)
		{
			auto schema = session->conn->getDefaultSchema();
			auto table = schema.getTable("comments");

			if (comment.reply_to())
			{
				table.insert("comment", "publish", "user_id", "news_id", "reply_to")
				     .values(comment.content(), comment.publish_date(), session->user->id(), comment.news()->id(),
				             comment.reply_to()->id())
				     .execute();
				return {};
			}
			else
			{
				table.insert("comment", "publish", "user_id", "news_id")
				     .values(comment.content(), comment.publish_date(), session->user->id(), comment.news()->id())
				     .execute();
				return {};
			}
		}
		else
		{
			return {lang->get_value("error_status", "Invalid login status")};
		}
	}
	catch (mysqlx::Error& ex)
	{
		return {ex.what()};
	}
}

vector<Comment> CommentService::get_comment_by_news(News& news)
{
	vector<Comment> comments;
	auto schema = session->conn->getDefaultSchema();
	auto table = schema.getTable("comments");

	auto rows = table.select("*").where(format("news_id = {} and reply_to is null", news.id())).execute();
	for (auto row : rows)
	{
		User *user = user_service->get_user_by_id(row[3].get<int>());
		comments.emplace_back(row[0].get<int>(), row[1].get<string>(), row[2].get<string>(), *user, &news, nullptr);
		get_sub_comments(comments.back(), table);
	}
	return comments;
}

void CommentService::get_sub_comments(Comment& parent, mysqlx::Table& table)
{
	auto rows = table.select("*").where(format("reply_to = {}", parent.id())).execute();
	for (auto row : rows)
	{
		User *user = user_service->get_user_by_id(row[3].get<int>());
		parent.replies().emplace_back(row[0].get<int>(), row[1].get<string>(), row[2].get<string>(), *user, parent.news(), &parent);
		get_sub_comments(parent.replies().back(), table);
	}
}
