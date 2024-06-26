#pragma once

#include <mysqlx/xdevapi.h>

#include "Comment.h"
#include "ServiceResult.h"

class CommentService
{
public:
	/**
	 * ����ظ�
	 * @param comment �ظ���Ϣ
	 * @return ������
	 */
	ServiceResult comment(const Comment& comment);

	/**
	 * ��ȡĳ�����ŵ�����
	 * @param news ������Ϣ
	 * @return ������Ϣ
	 */
	vector<Comment> get_comment_by_news(News& news);

private:

	void get_sub_comments(Comment& parent, mysqlx::Table &table);
};
