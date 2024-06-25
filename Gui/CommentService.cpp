#include "CommentService.h"

#include "Common.h"
#include "News.h"
#include "Session.h"

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
