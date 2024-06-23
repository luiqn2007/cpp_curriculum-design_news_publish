#pragma once
#include "DbData.h"

/**
 * ��������
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
     * @return ��������
     */
    string type() const
    {
	    return type_;
    }
};

