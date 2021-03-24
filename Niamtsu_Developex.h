#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Niamtsu_Developex.h"
#include "WorkProcess.h"
#include <qmessagebox.h>

class Niamtsu_Developex : public QMainWindow
{
    Q_OBJECT

public:
    Niamtsu_Developex(QWidget *parent = Q_NULLPTR);

    Ui::Niamtsu_DevelopexClass* GetUi();

private slots:

    void on_startBtn_clicked();

    void on_stopBtn_clicked();

private:
    Ui::Niamtsu_DevelopexClass ui;
    WorkProcess* work;
};
