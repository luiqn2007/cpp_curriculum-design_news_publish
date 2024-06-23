#pragma once

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
};
