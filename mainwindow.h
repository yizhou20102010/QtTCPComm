#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plccomm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ProactiveConnect_clicked();

    void on_ReactiveConnect_clicked();

    void on_CloseProactiveConnect_clicked();

    void on_CloseReactiveConnect_clicked();

    void on_WriteDatas_clicked();

    void on_TCPComm_error(int code, QString str);

private:
    Ui::MainWindow *ui;
    PLCComm *m_plcDevice;
};

#endif // MAINWINDOW_H
