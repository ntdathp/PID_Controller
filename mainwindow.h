#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include "qcustomplot.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_butt_clicked();

    void init_window();

    void update_port();

    void update_baundrate();

    bool isPortConnected(const QSerialPortInfo &portInfo);

    void serialError();


    void on_send_butt_clicked();

    void on_clear_butt_clicked();

    void recieve_data();

    void on_vtun_bt_clicked();

    void on_ptun_bt_clicked();

    void on_stop_bt_clicked();

    void on_spid_tb_clicked();

    void on_trans_pl_bt_clicked();

    void on_rec_pl_bt_clicked();

    void plotSetting(QCustomPlot  *plot, const char* xLabel, const char * yLabel);

    void plotConfig();

    void plotRespond();

    void tbRespond(QString mess);

    void aliveChecking();

private:
    Ui::MainWindow *ui;

    QSerialPort serial;
    QSerialPortInfo info;
    QTimer timer;
    QTimer check_alive_timer;
    QList<QSerialPortInfo> ports;
    bool init = true;
    bool stop = false;
    QList<double> timeBuff, valueBuff, refBuff;
    QList<double> atimeBuff, avalueBuff, arefBuff;
    QElapsedTimer tick_timer;
    quint64       preTime;
    float         refValue;
    QString       check_alive;
};
#endif // MAINWINDOW_H
