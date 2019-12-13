#include "taskmanager.h"
#include "ui_taskmanager.h"


TaskManager::TaskManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskManager)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateUsage()));
    timer->start(10);

}



void TaskManager::UpdateUsage(){
    UpdateMenUsage();

    UpdateCpuUsage();

    UpdateDiskUsage();

    UpdateSysMessage();

    UpdateCPUinfo();

    UpdateProcess();


}



void TaskManager::UpdateMenUsage(){
    QFile file;
    QString str;
    double total, used, free;

    //内存信息
    file.setFileName("/proc/meminfo");
    if(!file.open(QIODevice::ReadOnly)) return;
    //
    str = file.readLine();
    total = str.mid(10, str.size() - 13).trimmed().toInt()/(1024*1024.0);
    ui->label_memtotal->setText("内存大小:" + QString::number(total, 'f', 1) + "GB");
    //
    str = file.readLine();
    used = total - str.mid(10, str.size() - 13).trimmed().toInt()/(1024*1024.0);
    ui->label_memused->setText("内存used:" + QString::number(used, 'f', 1) + "GB");
    //
    ui->progressBar_mem->setValue(used*100/total);
    file.close();
}

void TaskManager::UpdateCpuUsage(){

    QString tempStr; //读取文件信息字符串
    QFile tempFile; //用于打开系统文件

    int cpu_info[2][9];
    double total_idle;
    double total_cpu_time[2];
    double usage;
    memset(cpu_info,0,sizeof(int)*18);
    memset(total_cpu_time,0,sizeof(double)*2);

    for(int i = 0;i < 2;i++)
    {
        tempFile.setFileName("/proc/stat");
        if ( !tempFile.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("The stat file can not open!"), QMessageBox::Yes);
            return;
        }
        tempStr = tempFile.readLine();
        for(int j = 0;j < 7;j++)
            cpu_info[i][j] = tempStr.section(" ",j+2,j+2).trimmed().toInt();
        //cpu_info[i][6] = tempStr.section(" ",8,8).trimmed().toInt();


        printf("cpu_info[0]:%s\n",tempStr.section(" ",2,2).trimmed().toStdString().c_str());
        tempFile.close();

        if(i == 1)
               break;

        usleep(1000000);
    }

    total_idle = cpu_info[1][3] - cpu_info[0][3];
    for(int i = 0;i < 7;i++)
    {
        total_cpu_time[0] += cpu_info[0][i];
        printf("%d:%d ", i, cpu_info[0][i]);
        total_cpu_time[1] += cpu_info[1][i];
        printf("%d\n", cpu_info[1][i]);
        printf("totl1: %f totl2: %f\n",total_cpu_time[0],total_cpu_time[1]);
    }

    usage = 1-total_idle/(total_cpu_time[1] - total_cpu_time[0]);
    printf("1-usage %f\n",1-usage);

    ui->progressBar_cpu->setValue(usage*100);

    //ui->check->setText(QString::number(total_cpu_time[1]-total_cpu_time[0],10));


}



void TaskManager::UpdateDiskUsage(){

    QString tempStr; //读取文件信息字符串
    QFile tempFile; //用于打开系统文件
    int pos = -1; //读取文件的位置
    int io_time[2][2];
    int usage[2];
    int sda_num = 0;//0 mains whole sda ; 1 mains sda1

    tempFile.setFileName("/proc/diskstats");
    if ( !tempFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("The stat file can not open!"), QMessageBox::Yes);
        return;
    }

    while (1)
    {
        tempStr = tempFile.readLine();
        pos = tempStr.indexOf("sda");


        if (pos != -1 &&sda_num == 0)//find sda(there are 2)
        {
            //memTotal = tempStr.mid(pos+10, tempStr.length()-13);
            //memTotal = memTotal.trimmed();
            //nMemTotal = memTotal.toInt()/1024;
            //take the 13th number(IOtime)
            io_time[0][0] = tempStr.section(' ',21,21).trimmed().toInt();
            sda_num++;

        }
        else if (sda_num == 1)//find sda1
        {
            io_time[0][1] = tempStr.section(' ',21,21).trimmed().toInt();
            break;
        }
    }

    tempFile.close();
    usleep(1000000);

    sda_num = 0;
    pos = -1;
    tempFile.setFileName("/proc/diskstats");
    if ( !tempFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("The stat file can not open!"), QMessageBox::Yes);
        return;
    }

    while (1)
    {
        tempStr = tempFile.readLine();
        pos = tempStr.indexOf("sda");


        if (pos != -1 && sda_num == 0)//find sda(there are 2)
        {
            //memTotal = tempStr.mid(pos+10, tempStr.length()-13);
            //memTotal = memTotal.trimmed();
            //nMemTotal = memTotal.toInt()/1024;
            //take the 13th number(IOtime)
            io_time[1][0] = tempStr.section(' ',21,21).trimmed().toInt();
            sda_num++;

        }
        else if (sda_num == 1)//find sda1
        {
            io_time[1][1] = tempStr.section(' ',21,21).trimmed().toInt();
            break;
        }
    }

    tempFile.close();

    usage[0] = (io_time[1][0] - io_time[0][0])/1000;
    usage[1] = (io_time[1][1] - io_time[1][0])/1000;
    //QString IoTime_1;
    //QString IoTime_2;
    //IoTime_1 = QString::number(io_time[1][0],10);
   // IoTime_2 = QString::number(io_time[0][0],10);
    ui->progressBar_disk1->setValue( usage[0]);
    ui->progressBar_disk2->setValue(usage[1]);
   //ui->io_time_1->setText( IoTime_1);
   //ui->io_time_2->setText( IoTime_2);

}

void TaskManager::UpdateCPUinfo(){
    QString tempStr; //读取文件信息字符串
    QFile tempFile; //用于打开系统文件
    int pos;

    tempFile.setFileName("/proc/cpuinfo");
    if ( !tempFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("The stat file can not open!"), QMessageBox::Yes);
        return;
    }

    QString producer;
    QString CpuName;
    QString CpuHz;
    QString CpuCores;

    while(1)
    {
        tempStr = tempFile.readLine();
        pos = tempStr.indexOf("vendor_id");


        if(pos != -1)
            producer = tempStr.section(":",1,1).trimmed();
        else if(pos = tempStr.indexOf("model name"),pos != -1)
            CpuName = tempStr.section(":",1,1);
        else if(pos = tempStr.indexOf("cpu MHz"),pos != -1)
            CpuHz = tempStr.section(":",1,1);
        else if(pos = tempStr.indexOf("cpu cores"),pos != -1 )
        {
            CpuCores = tempStr.section(":",1,1);
            break;
        }
    }

    tempFile.close();
    ui->cores->setText(CpuCores);
    ui->hz->setText(CpuHz);
    ui->cpu_type->setText(CpuName);
    ui->produce->setText(producer);

}

void TaskManager::UpdateSysMessage(){

    QString tempStr; //读取文件信息字符串
    QFile tempFile; //用于打开系统文件
    int pos = -1; //读取文件的位置
    double RunTime;

    tempFile.setFileName("/proc/uptime");
    if ( !tempFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("The stat file can not open!"), QMessageBox::Yes);
        return;
    }

    tempStr = tempFile.readLine();

    RunTime = tempStr.section(" ",0,0).trimmed().toDouble();

    QString hour;
    QString minute;
    QString second;
    QString TimeRes;

    hour = QString::number(((int) RunTime)/3600,10);
    minute = QString::number(((int)RunTime%3600)/60,10);
    second = QString::number((int)RunTime%60,10);
    TimeRes = TimeRes.append(hour).append(" hours ");
    TimeRes = TimeRes.append(minute).append(" mins ");
    TimeRes = TimeRes.append(second).append(" s ");
    ui->running_time->setText(TimeRes);
    tempFile.close();

    tempFile.setFileName("/proc/sys/kernel/hostname");
    if ( !tempFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("The stat file can not open!"), QMessageBox::Yes);
        return;
    }

    tempStr = tempFile.readLine();

    ui->host_name->setText(tempStr);
    tempFile.close();

    QString OsType;
    QString Osrelease;
    tempFile.setFileName("/proc/sys/kernel/ostype");
    if ( !tempFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("The stat file can not open!"), QMessageBox::Yes);
        return;
    }

    tempStr = tempFile.readLine();
    OsType = tempStr.trimmed();
    tempFile.close();

    tempFile.setFileName("/proc/sys/kernel/osrelease");
    if ( !tempFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("The stat file can not open!"), QMessageBox::Yes);
        return;
    }

    tempStr = tempFile.readLine();
    Osrelease = tempStr.trimmed();
    tempFile.close();

    OsType.append(" ");
    OsType.append(Osrelease);

    ui->version->setText(OsType);

}

void TaskManager::UpdateProcess(){


    QString tempStr; //读取文件信息字符串
    QFile tempFile; //用于打开系统文件
    ui->listWidget_process->clear();
    QDir qd("/proc");
    QStringList qsList = qd.entryList();
    QString qs = qsList.join("\n");
    QString id_of_pro;
    bool ok;
    int find_start = 3;
    int a, b;
    int nProPid; //进程PID
    int number_of_sleep = 0, number_of_run = 0, number_of_zombie = 0;
    int totalProNum = 0; //进程总数
    QString proName; //进程名
    QString proState; //进程状态
    QString proPri; //进程优先级
    QString proMem; //进程占用内存
    QListWidgetItem *title = new QListWidgetItem("PID\t" + QString::fromUtf8("名称") + "\t\t" +
                                                 QString::fromUtf8("状态") + "\t" +
                                                 QString::fromUtf8("优先级") + "\t" +
                                                 QString::fromUtf8("占用内存"), ui->listWidget_process);
    //循环读取进程
    while (1)
    {
        //获取进程PID
        a = qs.indexOf("\n", find_start);
        b = qs.indexOf("\n", a+1);
        find_start = b;
        id_of_pro = qs.mid(a+1, b-a-1);
        totalProNum++;
        nProPid = id_of_pro.toInt(&ok, 10);
        if(!ok)
        {
            break;
        }

        //打开PID所对应的进程状态文件
        tempFile.setFileName("/proc/" + id_of_pro + "/stat");
        if ( !tempFile.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("The pid stat file can not open!"), QMessageBox::Yes);
            return;
        }
        tempStr = tempFile.readLine();
        if (tempStr.length() == 0)
        {
            break;
        }
        a = tempStr.indexOf("(");
        b = tempStr.indexOf(")");
        proName = tempStr.mid(a+1, b-a-1);
        proName.trimmed(); //删除两端的空格
        proState = tempStr.section(" ", 2, 2);
        proPri = tempStr.section(" ", 17, 17);
        proMem = tempStr.section(" ", 22, 22);

        switch ( proState.at(0).toLatin1() )
        {
            case 'S':   number_of_sleep++; break; //Sleep
            case 'R':   number_of_run++; break; //Running
            case 'Z':   number_of_zombie++; break; //Zombie
            default :   break;
        }

        if (proName.length() >= 12)
        {
            QListWidgetItem *item = new QListWidgetItem(id_of_pro + "\t" +
                                                        proName + "\t" +
                                                        proState + "\t" +
                                                        proPri + "\t" +
                                                        proMem, ui->listWidget_process);
        }
        else
        {
            QListWidgetItem *item = new QListWidgetItem(id_of_pro + "\t" +
                                                        proName + "\t\t" +
                                                        proState + "\t" +
                                                        proPri + "\t" +
                                                        proMem, ui->listWidget_process);
        }

        tempFile.close(); //关闭该PID进程的状态文件
    }
    QString temp;
    temp = QString::number(totalProNum, 10);
    ui->label_pNum->setText(temp);
    temp = QString::number(number_of_run, 10);
    ui->label_pRun->setText(temp);
    temp = QString::number(number_of_sleep, 10);
    ui->label_pSleep->setText(temp);
    temp = QString::number(number_of_zombie, 10);
    ui->label_pZombie->setText(temp);


}


void TaskManager::UpdateProc(int pid){
    string name, stat;
    int tmp, nice, priority;
    stringstream ss;
    ss << pid;

    ifstream fin("/proc/" + ss.str() + "/stat", ios::in);
    fin >> tmp >> name;
    fin.close();

    fin.open("/proc/" + ss.str() + "/stat", ios::in);
}

TaskManager::~TaskManager()
{
    delete ui;
    delete timer;
}
