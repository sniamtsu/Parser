#pragma once
#include <iostream>
#include "Page.h"
#define CURL_STATICLIB
#include <curl/curl.h>
class Niamtsu_Developex;


class PageCreator
{
public:
	static Page* CreatePage(const std::string url, Niamtsu_Developex* ui);
private:
	static int writer(char* data, size_t size, size_t nmemb, std::string* buffer)
	{
		int result = 0;
		if (buffer != NULL)
		{
			buffer->append(data, size * nmemb);
			result = size * nmemb;
		}
		return result;
	}
};

