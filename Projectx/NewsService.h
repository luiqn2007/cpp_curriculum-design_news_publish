#pragma once

#include "News.h"
#include "ServiceResult.h"

class NewsService
{
public:
	/**
	 * ��ȡ�������ţ�һҳ��
	 * @param page ��ѯ�ڼ�ҳ
	 * @param count ÿҳ��ʾ����������
	 * @param total ������������
	 * @return ��ǰҳ��ʾ������
	 */
	vector<News> get_latest_news(int page, int count, int& total);

	/**
	 * �������ͻ�ȡ����
	 * @param type ��������
	 * @param page ��ѯ�ڼ�ҳ
	 * @param count ÿҳ��ʾ����������
	 * @param total ��������������
	 * @return ��ǰҳ��ʾ������
	 */
	vector<News> get_news_by_type(int type, int page, int count, int& total);

	/**
	 * ��ȡ�鵵������
	 * @param year ��ѯ�������
	 * @param page ��ѯ�ڼ�ҳ
	 * @param count ÿҳ��ʾ����������
	 * @param total ������������
	 * @return ��ǰҳ��ʾ������
	 */
	vector<News> get_archived_news(int year, int page, int count, int& total);

	/**
	 * ��ȡ��������
	 * @param count �������� 
	 * @return ��������
	 */
	vector<News> get_hot_news(int count);

	/**
	 * ���� id ��ȡ����
	 * @param id ���� id
	 * @return ���Ŷ���
	 */
	News get_news_by_id(int id);

	/**
	 * ��������
	 * @param news ����
	 * @return �������
	 */
	ServiceResult save_news(News& news);

	/**
	 * �޸�����
	 * @param news ����
	 * @return �޸Ľ��
	 */
	ServiceResult update_news(News& news);

	/**
	 * ɾ������
	 * @param news ����
	 * @return ɾ�����
	 */
	ServiceResult delete_news(News& news);
};
