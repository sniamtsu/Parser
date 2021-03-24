#pragma once
#include <string>
#include <vector>
class Page
{
public:
	
	Page(std::string url, std::string content);
	std::string GetUrl()const;
	std::string GetContent()const;
	
private:

	std::string m_url;
	std::string m_content;

};

