#include "plccomm.h"
#include <QHostAddress>
#include <QDebug>
#include <iostream>


using namespace std;

PLCComm::PLCComm(QObject *parent) : QObject(parent)
{
    //创建读写的Server
    tcpServerRead = new QTcpServer(this);
    tcpServerWrite = new QTcpServer(this);
    //监听过程中检测到
    connect(tcpServerRead, SIGNAL(newConnection()), this, SLOT(acceptConnectionRead()));
    connect(tcpServerWrite, SIGNAL(newConnection()), this, SLOT(acceptConnectionWrite()));

    //新建读写Socket
    tcpSocketRead = new QTcpSocket(this);
    tcpSocketWrite = new QTcpSocket(this);

    connect(tcpSocketRead, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(tcpSocketRead, SIGNAL(error(QAbstractSocket::SocketError)), this ,SLOT(displayErrorRead(QAbstractSocket::SocketError)));
    connect(tcpSocketWrite, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayErrorWrite(QAbstractSocket::SocketError)));
    connect(tcpSocketRead, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
}

//连接网络设备：true作为服务端等待连接，false作为客户端主动连接；
bool PLCComm::ConnectToDevice(const bool isServer)
{
    if(tcpSocketRead->state()==QAbstractSocket::ConnectedState&&tcpSocketWrite->state()==QAbstractSocket::ConnectedState)
    {
        emit Messages(100, "Device is Already Connected");
        return true;
    }
    if(isServer)//服务端，被动连接
    {
        if(!tcpServerRead->isListening())//如果没有被监听，则启动指定监听
        {
            //监听指定事件：ip为192.168.0.1写端口为3000的网络设备向ip为192.169.0.3端口为2000网络设备
//            if(!tcpServerRead->listen(QHostAddress("192.168.0.1:3000"), 2000))//192.169.0.3为本地IP地址
            if(!tcpServerRead->listen(QHostAddress("192.168.0.1:3000"), 2000))//192.169.0.3为本地IP地址
            {
                tcpServerRead->close();
                emit Messages(0,"Read server listen is failed.");
                return false;
            }
        }
        if(!tcpServerWrite->isListening())//如果没有被监听，则启动指定监听
        {
            //监听指定事件：ip为192.168.0.1读端口为3001的网络设备向ip为192.169.0.3端口为2001网络设备
            if(!tcpServerWrite->listen(QHostAddress("192.168.0.1:3001"), 2001))//192.169.0.3为本地IP地址
            {
                tcpServerWrite->close();
                emit Messages(0, "Write server listen is failed.");
                return false;
            }
        }
    }
    else //客户端 主动连接
    {
        tcpSocketRead->close();
        if(!tcpSocketRead->bind(3001))//绑定本地读端口
        {
            tcpSocketRead->abort();
            emit Messages(0, "Read Socket Bind Local Port Failed.");
            return false;
        }
        tcpSocketRead->connectToHost("192.168.0.2",2001);
        if(!tcpSocketRead->waitForConnected(5000))//
        {
            tcpSocketRead->abort();
            emit Messages(0, "Read socket connect to host is timeout.");
            return false;
        }

        tcpSocketWrite->close();
        if(!tcpSocketWrite->bind(3000))//绑定本地写端口
        {
            tcpSocketWrite->abort();
            emit Messages(0, "Read Socket Bind Local Port Failed.");
            return false;
        }
        tcpSocketWrite->connectToHost("192.168.0.2",2000);
        if(!tcpSocketWrite->waitForConnected(5000))
        {
            tcpSocketWrite->abort();
            emit Messages(0,"Read socket connect to host is timeout.");
            return false;
        }
    }
   return true;
}

bool PLCComm::DisconnectToPLC(const bool isServer)
{    
    tcpSocketRead->close();
    tcpSocketWrite->close();
    if(isServer)
    {
        tcpServerRead->close();
        tcpServerWrite->close();
    }
    return true;
}
//获取监听得到Socket
void PLCComm::acceptConnectionRead()
{
    tcpSocketRead->abort();
    tcpSocketRead = tcpServerRead->nextPendingConnection();
    if(tcpSocketRead==0)
        emit Messages(0, "There are no read pending connections.");
    cout<<"Read Connected"<<endl;
    connect(tcpSocketRead, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(tcpSocketRead, SIGNAL(error(QAbstractSocket::SocketError)), this ,SLOT(displayErrorRead(QAbstractSocket::SocketError)));
}
//获取监听得到Socket
void PLCComm::acceptConnectionWrite()
{
    tcpSocketWrite->abort();
    tcpSocketWrite = tcpServerWrite->nextPendingConnection();
    if(tcpSocketWrite==0)
        emit Messages(0, "There are no write pending connections.");
    cout<<"Write Connected"<<endl;
    connect(tcpSocketWrite, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayErrorWrite(QAbstractSocket::SocketError)));
}

//读取网络设备数据
void PLCComm::receiveData()
{
    if(tcpSocketRead->isValid())
    {
        m_ReadBytes = tcpSocketRead->readAll();
    }
    else
        emit Messages(0, "Read socket is invalid!");

    cout<<"Receive nums: "<<m_ReadBytes.count()<<" Datas: ";
    for(int i=0;i<m_ReadBytes.count();i++)
        cout<<(int)m_ReadBytes.at(i)<<" ";
    cout<<endl;
}
//向网络设备发送数据
bool PLCComm::wirteData(const QByteArray datas)
{
    if(tcpSocketWrite->isValid())
    {
        qint64 nums=tcpSocketWrite->write(datas);
        if(nums==datas.count())//数据实际写入字节数等于需要写入的字节数
            return true;
        emit Messages(0, "Writint bytes is failed!");
        return false;
    }
    else
    {
        emit Messages(0, "Write socket is invalid!");
        return false;
    }
}

void PLCComm::displayErrorRead(QAbstractSocket::SocketError)
{
    emit Messages(10, tcpSocketRead->errorString());
//    tcpSocketRead->close();
}

void PLCComm::displayErrorWrite(QAbstractSocket::SocketError)
{
    emit Messages(10, tcpSocketWrite->errorString());
//    tcpSocketWrite->close();
}

void PLCComm::socketStateChanged(QAbstractSocket::SocketState state)
{
    qDebug()<<"Socket State: "<<state;
}

void PLCComm::ontimeout()
{

}

