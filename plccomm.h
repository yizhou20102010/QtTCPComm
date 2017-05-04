#ifndef PLCCOMM_H
#define PLCCOMM_H

#include<QObject>
#include <QtNetwork>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QAbstractSocket>
#include <QTimer>

class PLCComm : public QObject
{
    Q_OBJECT

    QTcpSocket *tcpSocketRead;
    QTcpSocket *tcpSocketWrite;

    QTcpServer *tcpServerRead;
    QTcpServer *tcpServerWrite;
public:
    explicit PLCComm(QObject *parent=0);
    bool ConnectToDevice(const bool isServer=false);//连接网络设备：0：作为服务端等待连接，1：作为客户端主动连接；
    bool DisconnectToPLC(const bool isServer=false);
    bool wirteData(const QByteArray datas);//发送数据
private slots:
    void acceptConnectionRead(void);//作为服务端使用时，接受设备
    void acceptConnectionWrite(void);//作为服务端使用时，接受设备

    void receiveData();//接受数据
    void displayErrorRead(QAbstractSocket::SocketError);//显示错误
    void displayErrorWrite(QAbstractSocket::SocketError);//显示错误
    void socketStateChanged(QAbstractSocket::SocketState state);//Socket状态变化
    void ontimeout();

public:
signals:
    void Messages(int, QString);//发送信号代码及提示0:Error, 100:Tip

public:
    QByteArray m_ReadBytes;//读取的数据
};

#endif // PLCCOMM_H
