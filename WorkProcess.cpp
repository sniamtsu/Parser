#include "WorkProcess.h"
#include "Niamtsu_Developex.h"
#include <chrono>
WorkProcess::WorkProcess(int maxThreads, int maxPages): m_maxThreads(maxThreads), m_maxPages(maxPages){}

void WorkProcess::AddPage(Page* page)
{
	if (page)                              //If not nullptr 
	{			
		m_nextBuffer.push_back(page);     
		m_createdPageCount++;
	}
}

void WorkProcess::SearchUrl(std::string content, Niamtsu_Developex* ui)
{
	int i = 0;
	while (m_createdPageCount < m_maxPages && m_activeWork)
	{
		if (content.find("https://", i) != std::string::npos)                            //Find "https://" in content string
		{ 
			i = content.find("https://", i);                                            //Take start position of "https://"
			std::string buffer ="";
			while (content[i] != ' ' && content[i] != '\"' && content[i] != '\'')
			{
				buffer += content[i];                                                  //Copy Url to temporary baffer
				i++;
			}
			mtx.lock();
			if (m_createdPageCount < m_maxPages)
			{
				WorkProcess::AddPage(PageCreator::CreatePage(buffer, ui));               //Create page 
			}
			mtx.unlock();
		}
		else
		{			
			break;
		}
	}
}

void WorkProcess::SearchWord(std::string content, std::string word, Niamtsu_Developex* ui)
{
	int i = 0;	
	while (true && m_activeWork)
	{		
		if (content.find(word, i) != std::string::npos)                                          //Find word in content string
		{
			i = content.find(word, i + word.length());                                           //Take start position for continue searching

			mtx.lock();

			m_searchWordCount++;

			ui->GetUi()->findWordsLabel_2->setText(QString::number(m_searchWordCount));         //Update text label
			ui->GetUi()->findWordsLabel_2->repaint();                                          //Repaint text label

			std::this_thread::sleep_for(std::chrono::milliseconds(10));                        //Pause for correct display

			mtx.unlock();
			
		}
		else
		{
			mtx.lock();
			m_parsedPagecount++;
			ui->GetUi()->progressBar->setValue(m_parsedPagecount);                           // Set value for progress bar
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			mtx.unlock();
			break;
		}	
	}
}

void WorkProcess::PagesParse(std::string word, int start, int finish, Niamtsu_Developex* ui)
{	
	while(start <= finish&& m_activeWork)
	{  
		
		mtx.lock();
		std::string buffer = m_currentBuffer[start]->GetContent();                                       //Take content string from page to temporary buffer				
		
		ui->GetUi()->urlListWidget->addItem(QString::fromStdString(m_currentBuffer[start]->GetUrl()));		//Add Url to ListWidget
		ui->GetUi()->urlListWidget->item(0)->setHidden(false);                                             //Set Hide status to false
		ui->GetUi()->urlListWidget->repaint();                                                             //Repaint ListWidget
		 
		std::this_thread::sleep_for(std::chrono::milliseconds(50));                                     //Pause for correct display
		
		mtx.unlock();

		WorkProcess::SearchUrl(buffer, ui);
		WorkProcess::SearchWord(buffer, word, ui);
		start++;
	}
}

void WorkProcess::Threadsmanage(std::string word, Niamtsu_Developex *ui)
{
	
	while (m_nextBuffer.size()&& m_activeWork)
	{
		m_threads = new std::thread[m_maxThreads];                                           //Create threads array

		for (int i = 0; i < m_currentBuffer.size(); i++)                                    //Free memory
		{
			delete m_currentBuffer[i];
		}
		m_currentBuffer.clear();

		m_currentBuffer = m_nextBuffer;

		m_nextBuffer.clear();

		if (m_currentBuffer.size() <= m_maxThreads)                                           //If count of pages less than threads                
		{
			for (int i = 0; i < m_currentBuffer.size(); i++)
			{
				m_threads[i] = std::thread(&WorkProcess::PagesParse, this, word, i, i, ui);    //Create threads with start and stop position of taking pages 
			}

			for (int i = 0; i < m_currentBuffer.size(); i++)                             
			{
				m_threads[i].join();
			}                                                       

		}
		else
		{
			int remainder = m_currentBuffer.size() % m_maxThreads;			
			int step = m_currentBuffer.size() / m_maxThreads;
			int start = 0;
			for (int i = 0; i < m_maxThreads; i++)
			{						
				m_threads[i] = std::thread(&WorkProcess::PagesParse, this, word, start, step + start - (1 - (i < remainder)), ui);  //Create threads with start and stop position of taking pages
				start = step + start - (1 - (i < remainder)) +1;
			}

			for (int i = 0; i < m_maxThreads; i++)  
			{
				m_threads[i].join();
			}

		}
		delete[] m_threads;                                                                 //Free memory
	}
}

int WorkProcess::GetNextBufferSize()const { return m_nextBuffer.size(); }

void WorkProcess::SetActiveWork(bool activeWork){ m_activeWork = activeWork; }

WorkProcess::~WorkProcess()
{
	for (int i = 0; i < m_currentBuffer.size(); i++)                                    //Free memory
	{
		delete m_currentBuffer[i];
	}
	m_currentBuffer.clear();


	if (m_nextBuffer.size())                                                           //Free memory
	{
		for (int i = 0; i < m_nextBuffer.size(); i++)
		{
			delete m_nextBuffer[i];
		}
	}
	m_nextBuffer.size();

}
