#include "PageCreator.h"
#include "Niamtsu_Developex.h"
Page* PageCreator::CreatePage(const std::string url, Niamtsu_Developex* ui)
{
	CURL* curl = curl_easy_init();
	if (curl)                                     //if connected 
	{
		std::string buffer;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, PageCreator::writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if (buffer.length())                     //if content is not empty
		{
			return new Page(url, buffer);
		}
		else
		{
			ui->GetUi()->errorListWidget->addItem(QString::fromStdString("Can not download content from: "+url)); //Add Error message
			ui->GetUi()->errorListWidget->item(0)->setHidden(false);
			ui->GetUi()->errorListWidget->repaint();
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			return nullptr;
		}
	}
	else
	{	
		ui->GetUi()->errorListWidget->addItem(QString::fromStdString("Can not open Url: "+url));                  //Add Error message
		ui->GetUi()->errorListWidget->item(0)->setHidden(false);
		ui->GetUi()->errorListWidget->repaint();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return nullptr;
	}
}
