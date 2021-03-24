#include "Niamtsu_Developex.h"

Niamtsu_Developex::Niamtsu_Developex(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.progressBar->setValue(0);                         //Set progress bar to 0
}
void Niamtsu_Developex::on_startBtn_clicked()
{
    ui.errorListWidget->clear();                       
    ui.errorListWidget->repaint();

    ui.urlListWidget->clear();
    ui.urlListWidget->repaint();

    ui.findWordsLabel_2->setText("0");

    ui.progressBar->setValue(0);

    int maxThreads = 0;
    int maxPages = 0;
    std::string startUrl = ui.urlLineEdit->text().toStdString();
    std::string word = ui.wordLineEdit->text().toStdString();
    maxThreads = ui.maxThreadsLineEdit->text().toInt();
    maxPages = ui.maxPagesLineEdit->text().toInt();

    ui.progressBar->setRange(0, maxPages);              //Set range for progress bar

    if (maxThreads <= 0)
    {
        QMessageBox::critical(this, "Error", "Wrong threads number");
    }
    else if (maxPages <= 0)
    {
        QMessageBox::critical(this, "Error", "Wrong pages number");
    }
    else if (word == "")
    {
        QMessageBox::critical(this, "Error", "Enter word");
    }
    else
    {
        work = new WorkProcess(maxThreads, maxPages);
        work->AddPage(PageCreator::CreatePage(startUrl, this));

        if (work->GetNextBufferSize() == 0)
        {
            QMessageBox::critical(this, "Error", "Wrong Url");
            delete work;
            work = nullptr;
        }
        else
        {       
            work->SetActiveWork(true);
            work->Threadsmanage(word, this);
            QMessageBox::about(this, "Message", "Word search ended");
            delete work;
            work = nullptr;
        }
    }
}



Ui::Niamtsu_DevelopexClass* Niamtsu_Developex::GetUi()
{
    return &ui;
}

void Niamtsu_Developex::on_stopBtn_clicked()
{
    
    if (work)
    {
        work->SetActiveWork(false);
    }
    
}
