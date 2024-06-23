#include "Property.h"

#include <fstream>

using std::fstream;
using std::getline;

void Property::read(string file)
{
	auto f = fstream(file, std::ios_base::in);
	string line;
	while (!f.eof())
	{
		getline(f, line);
		const auto ep = line.find_first_of('=');
		if (ep != string::npos)
		{
			auto key = line.substr(0, ep);
			auto value = line.substr(ep + 1);
			properties_[key] = value;
		}
	}
	f.close();
}

string Property::get_value(string key, string default_value)
{
	if (properties_.count(key))
	{
		return properties_[key];
	}
	return default_value;
}