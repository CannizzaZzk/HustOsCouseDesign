#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
using namespace std;

namespace Ui {
class TaskManager;
}

class TaskManager : public QWidget
{
    Q_OBJECT

public:
    explicit TaskManager(QWidget *parent = nullptr);
    ~TaskManager();

private:
    Ui::TaskManager *ui;
    QTimer *timer;
    void UpdateProc(int pid);
    void UpdateCpuUsage();
    void UpdateMenUsage();
    void UpdateSysMessage();
    void UpdateDiskUsage();
    void UpdateCPUinfo();
    void UpdateProcess();

public slots:

    void UpdateUsage();
};

#endif // TASKMANAGER_H
