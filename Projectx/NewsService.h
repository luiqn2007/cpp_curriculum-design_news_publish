#pragma once

#include "News.h"
#include "ServiceResult.h"

class NewsService
{
public:
	/**
	 * 获取最新新闻（一页）
	 * @param page 查询第几页
	 * @param count 每页显示的新闻数量
	 * @param total 所有新闻总数
	 * @return 当前页显示的新闻
	 */
	vector<News> get_latest_news(int page, int count, int& total);

	/**
	 * 根据类型获取新闻
	 * @param type 新闻类型
	 * @param page 查询第几页
	 * @param count 每页显示的新闻数量
	 * @param total 该类型新闻总数
	 * @return 当前页显示的新闻
	 */
	vector<News> get_news_by_type(int type, int page, int count, int& total);

	/**
	 * 获取归档的新闻
	 * @param year 查询新闻年号
	 * @param page 查询第几页
	 * @param count 每页显示的新闻数量
	 * @param total 当年新闻总数
	 * @return 当前页显示的新闻
	 */
	vector<News> get_archived_news(int year, int page, int count, int& total);

	/**
	 * 获取热门新闻
	 * @param count 新闻数量 
	 * @return 热门新闻
	 */
	vector<News> get_hot_news(int count);

	/**
	 * 根据 id 获取新闻
	 * @param id 新闻 id
	 * @return 新闻对象
	 */
	News get_news_by_id(int id);

	/**
	 * 发布新闻
	 * @param news 新闻
	 * @return 发布结果
	 */
	ServiceResult save_news(News& news);

	/**
	 * 修改新闻
	 * @param news 新闻
	 * @return 修改结果
	 */
	ServiceResult update_news(News& news);

	/**
	 * 删除新闻
	 * @param news 新闻
	 * @return 删除结果
	 */
	ServiceResult delete_news(News& news);
};
