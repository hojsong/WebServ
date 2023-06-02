#ifndef __LOCATION_HPP__
#define __LOCATION_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Location
{
	private:
		std::string _rootValue;
		std::map<std::string, std::string> _options;
	public:
		Location();
		void addLocation(const std::vector<std::string>& data);
		std::string getValue(std::string& value);
		std::string getRootValue() const;
};

#endif