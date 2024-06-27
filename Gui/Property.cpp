#include "Property.h"

#include <fstream>
#include <qimage.h>
#include <qstring.h>

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
			const auto key = line.substr(0, ep);
			const auto value = line.substr(ep + 1);
			properties_[key] = value;
		}
	}
	f.close();
}

string Property::get_value(const string& key, string default_value)
{
	if (properties_.contains(key))
	{
		return properties_[key];
	}
	return default_value;
}

QString Property::qt(const string& key)
{
	return QString::fromStdString(properties_[key]);
}

string Property::operator[](const string& key)
{
	return properties_[key];
}
