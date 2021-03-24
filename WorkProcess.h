#pragma once
#include<thread>
#include<mutex>
#include "PageCreator.h"
class Niamtsu_Developex;

class WorkProcess
{
public:
	
	WorkProcess(int maxThreads, int maxPages);	

	void AddPage(Page* page);

	void SearchUrl(std::string content, Niamtsu_Developex* ui);

	void SearchWord(std::string content, std::string word, Niamtsu_Developex* ui);

	void PagesParse(std::string word, int start, int finish, Niamtsu_Developex* ui);

	void Threadsmanage(std::string word, Niamtsu_Developex* ui);
	
	int GetNextBufferSize()const;

	void SetActiveWork(bool activeWork);

	~WorkProcess();
	
private:	
	std::mutex mtx;
	int m_maxThreads;
	int m_maxPages;
	int m_searchWordCount = 0;
	int m_createdPageCount = 0;
	int m_parsedPagecount = 0;
	bool m_activeWork = false;

	std::vector<Page*> m_currentBuffer;  //Buffer for current parsed pages

	std::vector<Page*> m_nextBuffer;     //Buffer for next parsed pages

	std::thread* m_threads;
};

