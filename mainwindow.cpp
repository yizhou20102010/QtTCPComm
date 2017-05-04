#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_plcDevice = new PLCComm(this);
    connect(m_plcDevice, SIGNAL(Messages(int,QString)), this, SLOT(on_TCPComm_error(int, QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//建立主动连接
void MainWindow::on_ProactiveConnect_clicked()
{
    if(m_plcDevice->ConnectToDevice(false))
    {
        ui->statusBar->showMessage("Success");
    }
    else ui->statusBar->showMessage("Failed");
}
//建立被动连接
void MainWindow::on_ReactiveConnect_clicked()
{
    if(m_plcDevice->ConnectToDevice(true))
    {
        ui->statusBar->showMessage("Success");
    }
    else ui->statusBar->showMessage("Failed");
}

void MainWindow::on_CloseProactiveConnect_clicked()
{
    m_plcDevice->DisconnectToPLC();
}

void MainWindow::on_CloseReactiveConnect_clicked()
{
    m_plcDevice->DisconnectToPLC(true);
}

void MainWindow::on_WriteDatas_clicked()
{
    QByteArray datas;
    unsigned int byte;
    for(unsigned int i=0;i<11;i++)
    {
        byte=0x00;
        datas.append(byte);
        byte=i+100;
        datas.append(byte);

    }
    m_plcDevice->wirteData(datas);
}

void MainWindow::on_TCPComm_error(int code, QString str)
{
    qDebug()<<"Error: "<<code<<" "<<str;
}
