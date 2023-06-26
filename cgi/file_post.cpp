#include "cgi.hpp"

std::string bufsaveBody(char *buf){
	const char* find = "\r\n\r\n";
	const char* rvalue = std::strstr(buf, find);
	if (rvalue == NULL)
		return "";
	const char* valueStart = rvalue + std::strlen(find);
	if (valueStart == NULL)
		return "";
	const char* rvalue2 = std::strstr(valueStart, find);
	if (rvalue2 == NULL)
		return "";
	const char* valueStart2 = rvalue2 + std::strlen(find);
	if (valueStart2 == NULL)
		return "";
	std::string savebuf = valueStart2;
	return savebuf;
}

std::string getPostFileName(char *buf){
	const char* str =  "filename=\"";
	const char* rvalue = std::strstr(buf, str);
	std::string filename;
	if (rvalue != NULL){
		const char* valueStart = rvalue + std::strlen(str);
        const char* valueEnd = std::strstr(valueStart, "\"");
		if (valueStart == valueEnd)
			return "";
        if (valueEnd != NULL)
            filename = std::string(valueStart, valueEnd - valueStart);
        else{
			return "";
		}
	}
    else{
		return "";
	}
	return filename;
}

void POST_FILE(char *buf, MemberRepository *mr){
	std::string filename = getPostFileName(buf);
	if (filename.length() == 0)
		;
	else {
		std::string savebody = bufsaveBody(buf);
		size_t pos = savebody.find("\r\n");
    	if (pos != std::string::npos) {
    	    std::string result = savebody.substr(0, pos);
			savebody.clear();
			savebody = result;
			result.clear();
    	}
		std::ofstream savefile("./html/file/" + filename);
		// std::cerr << "filename: " << filename << std::endl;
		savefile << savebody;
		// std::cerr << "savebody: " << savebody << std::endl;
		savefile.close();
	}
	std::ifstream file("./html/home.html");
	std::string result = getHtml(file);
	head_plus(result, 200, buf, mr);
	std::cout << result << std::endl;
}
