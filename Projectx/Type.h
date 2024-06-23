#pragma once
#include "DbData.h"

/**
 * 新闻类型
 */
class Type :
    public DbData
{
    string type_;

public:
    Type(const int id, string type)
	    : DbData(id),
	      type_(std::move(type))
    {
    }

    /**
     * @return 类型名称
     */
    string type() const
    {
	    return type_;
    }
};

