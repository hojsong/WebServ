#include "../includes/Location.hpp"

Location::Location() {
	this->_rootValue = "";
}

void
Location::addLocation(const std::vector<std::string>& data) {
	std::string key;
	std::string value;

	key = data[0];
	value = data[1];
	value.pop_back();
	if (key == "root") {
		this->_rootValue = value;
	}
	else {
		key = "/" + key;
		value = "/" + value;
	}
	this->_options.insert(std::pair<std::string, std::string>(key, value));
}

std::string
Location::getValue(std::string& value) {
	std::map<std::string, std::string>::iterator it = this->_options.find(value);
	if (it != this->_options.end()) {
		return it->second;
	}
	else {
		return "";
	}
}

std::string
Location::getRootValue() const {
	return this->_rootValue;
}