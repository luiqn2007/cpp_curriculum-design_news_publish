#pragma once

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
};
