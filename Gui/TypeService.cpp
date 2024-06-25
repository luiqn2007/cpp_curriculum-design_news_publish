#include "TypeService.h"

#include <format>

#include "Common.h"
#include "Session.h"

using std::format;

vector<Type> TypeService::get_types()
{
	auto schema = session->conn->getDefaultSchema();
	auto table = schema.getTable("types", true);
	auto rows = table.select("*").execute();
	vector<Type> types;
	for (auto row : rows)
	{
		types.emplace_back(row[0].get<int>(), row[1].get<string>());
	}
	return types;
}

Type TypeService::get_or_create_by_name(string type_name)
{
	auto schema = session->conn->getDefaultSchema();
	auto table = schema.getTable("types", true);
	auto existed = table.select("*").where(format("type = '{}'", type_name)).execute();
	if (existed.count() > 0)
	{
		auto row = existed.fetchOne();
		return {row[0].get<int>(), row[1].get<string>()};
	}
	auto insert = table.insert("type").values(type_name).execute();
	auto id = stoi(*insert.getGeneratedIds().begin());
	return { id, type_name };
}
