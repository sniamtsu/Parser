#include "Page.h"

Page::Page(std::string url, std::string content) :m_url(url), m_content(content){}

std::string Page::GetUrl()const { return m_url; }

std::string Page::GetContent()const { return m_content; }

