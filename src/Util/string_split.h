#ifndef STRING_SPLIT_H
#define STRING_SPLIT_H

#include <string>
#include <vector>

inline std::vector<std::string> string_split(const std::string &str, const std::string &delimiter) {

	std::vector<std::string> parts;

	// TODO : optimize?
	std::string strCopy(str);

	size_t pos = 0;
	std::string token;
	while ((pos = strCopy.find(delimiter)) != std::string::npos) {
    token = strCopy.substr(0, pos);
		parts.push_back(token);
    strCopy.erase(0, pos + delimiter.length());
	}

	return parts;
}


#endif
