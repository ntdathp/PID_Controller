#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_window();

    timer.setInterval(1000);
    check_alive_timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &MainWindow::update_port);
    connect(&check_alive_timer, &QTimer::timeout, this, &MainWindow::aliveChecking);
    timer.start();
    check_alive_timer.start();

    connect(&serial, &QSerialPort::errorOccurred, this,&MainWindow::serialError);
    connect(&serial,SIGNAL(readyRead()),this,SLOT(recieve_data()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::isPortConnected(const QSerialPortInfo &portInfo) {
    QSerialPort port;
    port.setPort(portInfo);
    if (port.isOpen()) {

        return true;
    }

    return false;
}
void MainWindow::serialError()
{
    if(serial.error())
    {
    QString errorMessage = "Error: " + serial.errorString();
    QMessageBox::critical(this, "Error", errorMessage);
    on_connect_butt_clicked();
    }
}
void MainWindow::update_port()
{
    //Find COM on Computer

    QList<QString> strPorts;
    QList<QSerialPortInfo> currentports = info.availablePorts();
    for (int i = 0; i < currentports.size(); i++) {
        const QSerialPortInfo &portInfo = currentports.at(i);
        strPorts.append(portInfo.portName());

    }
    if (currentports.size() != ports.size() || init == true )
    {
    ui->port_cb->clear();
    ui->port_cb->addItems(strPorts);
    }
    ports = currentports;

}

void MainWindow::update_baundrate()
{
    // List all Baudrates your computer support
    QList<qint32> baudRates = info.standardBaudRates();
    QList<QString> strBaudRates;
    for(int i = 0 ; i < baudRates.size() ; i++){
        strBaudRates.append(QString::number(baudRates.at(i)));
    }
    ui->baud_cb->addItems(strBaudRates);
}
void MainWindow::init_window()
{
    setFixedSize(width(), height());
    setWindowTitle("Example UART");

    ui->parity_cb->setCurrentIndex(0);
    ui->dataBit_cb->setCurrentIndex(3);
    ui->stopBit_cb->setCurrentIndex(0);

    update_port();
    update_baundrate();

    ui->send_butt->setDisabled(true);
    ui->clear_butt->setDisabled(true);
    ui->spid_tb->setDisabled(true);
    ui->vtun_bt->setDisabled(true);
    ui->ptun_bt->setDisabled(true);
    ui->stop_bt->setDisabled(true);
    ui->rec_pl_bt->setDisabled(true);
    ui->trans_pl_bt->setDisabled(true);

    plotConfig();

    init = false;
}
void MainWindow:: aliveChecking()
{
//    check_alive = "NONE\r\n";
//    if(serial.isOpen())
//     {
//    serial.write(check_alive.toUtf8());

//    QString rxrep = serial.readLine();
//    qDebug() << rxrep;
//    if(rxrep == check_alive){}
//    else
//        {
//         serial.close();
//         QMessageBox::critical(this, "Error", "Connection time out");
//        }
//     }

}


void MainWindow:: plotSetting(QCustomPlot  *plot, const char* xLabel, const char * yLabel)
{
    QFont legendFont = font();
    legendFont.setPointSize(8);
    plot  -> yAxis->setLabel(yLabel);
    plot  -> xAxis-> setLabel(xLabel);
    plot  -> setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot  -> legend -> setVisible(true);
    plot  -> legend->setFont(legendFont);
    plot  -> legend->setSelectedFont(legendFont);
    plot  -> legend->setSelectableParts(QCPLegend::spItems);
}

void MainWindow::plotConfig()
{
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    pen.setColor("#E2483E");

    QPen pen2;
    pen2.setStyle(Qt::SolidLine);
    pen2.setWidth(3);
    pen2.setColor("#5C97E3");

    ui -> plot -> addGraph();
    ui -> plot -> graph(0) -> setLineStyle(QCPGraph::lsLine);
    ui->plot->graph(0)->setPen(pen);
    ui->plot->graph(0)->setName("Set point");

    ui -> plot -> addGraph();
    ui -> plot -> graph(1) -> setLineStyle(QCPGraph::lsLine);
    ui->plot->graph(1)->setPen(pen2);
    ui->plot->graph(1)->setName("Current Value");

    plotSetting(ui-> plot, "Time", "Value");
}

void MainWindow::plotRespond() {
    if(!stop)
    {
     ui->plot->graph(0)->setData(timeBuff, refBuff);
     ui->plot->graph(1)->setData(timeBuff, valueBuff);
     ui->plot->rescaleAxes();
     ui->plot->replot();
     ui->plot->update();
    }
}

void MainWindow::tbRespond(QString mess)
{
    QString kpText = ui->kp_tb->text();
    QString kiText = ui->ki_tb->text();
    QString kdText = ui->kd_tb->text();
    QString spText = ui->sp_tb->text();

    bool kpConverted, kiConverted, kdConverted, spConverted;
    float kpValue = kpText.toFloat(&kpConverted);
    float kiValue = kiText.toFloat(&kiConverted);
    float kdValue = kdText.toFloat(&kdConverted);
    float spValue = spText.toFloat(&spConverted);

    if(mess == "VTUN " || mess == "PTUN ")
        refValue = ui->sp_tb->text().toFloat();


    if (kpText.isEmpty() || !kpConverted)
     kpValue = 0.0;
    if (kiText.isEmpty() || !kiConverted)
     kiValue = 0.0;
    if (kdText.isEmpty() || !kdConverted)
     kdValue = 0.0;
    if (spText.isEmpty() || !spConverted){
     spValue = 0.0;
     refValue = 0.0;
    }
    QString msg = mess + QString::number(kpValue) + " " + QString::number(kiValue) + " " + QString::number(kdValue) + " " + QString::number(spValue) + "\r\n";
    ui->trans_pl_tb->append(msg);
    serial.write(msg.toUtf8());

}
void MainWindow::on_connect_butt_clicked()
{
        if (ui->connect_butt->text() == "Connect")
        {
        serial.setPortName(ui->port_cb->currentText());
        serial.open(QIODevice::ReadWrite);
            if(!serial.isOpen())
            {
            QString errorMessage = "Can't open " + ui->port_cb->currentText() + ", Error:" + serial.errorString();
            QMessageBox::critical(this, "Error", errorMessage);
            }
            else
            {
                serial.setBaudRate(ui->baud_cb->currentText().toInt());
                serial.setDataBits(QSerialPort::Data8);
                serial.setStopBits(QSerialPort::OneStop);
                serial.setParity(QSerialPort::NoParity);
                ui->connect_butt->setText("Disconnect");
                ui->connect_butt->setStyleSheet("QPushButton {color: red;}");
                ui->port_cb->setDisabled(true);
                ui->baud_cb->setDisabled(true);
                ui->dataBit_cb->setDisabled(true);
                ui->stopBit_cb->setDisabled(true);
                ui->parity_cb->setDisabled(true);
                ui->send_butt->setDisabled(false);
                ui->clear_butt->setDisabled(false);
                ui->spid_tb->setDisabled(false);
                ui->vtun_bt->setDisabled(false);
                ui->ptun_bt->setDisabled(false);
                ui->stop_bt->setDisabled(false);
                ui->rec_pl_bt->setDisabled(false);
                ui->trans_pl_bt->setDisabled(false);
                timer.stop();
            }
        }
        else
        {
            serial.close();
            ui->connect_butt->setText("Connect");
            ui->connect_butt->setStyleSheet("QPushButton {color: green;}");
            ui->port_cb->setDisabled(false);
            ui->baud_cb->setDisabled(false);
            ui->dataBit_cb->setDisabled(false);
            ui->stopBit_cb->setDisabled(false);
            ui->parity_cb->setDisabled(false);
            ui->send_butt->setDisabled(true);
            ui->clear_butt->setDisabled(true);
            ui->spid_tb->setDisabled(true);
            ui->vtun_bt->setDisabled(true);
            ui->ptun_bt->setDisabled(true);
            ui->stop_bt->setDisabled(true);
            ui->rec_pl_bt->setDisabled(true);
            ui->trans_pl_bt->setDisabled(true);
            timer.start();
        }
}

void MainWindow::on_send_butt_clicked()
{
    QString mess = ui->lineEdit->text();
        if(ui->add_tb->isChecked())
        {
            mess.append("\r\n");
        }
    serial.write(mess.toUtf8());
}


void MainWindow::on_clear_butt_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::recieve_data()
{
    if (serial.canReadLine()) {
            QByteArray data = serial.readLine();

            QString    mess   = QString::fromUtf8(data);
            ui->textBrowser->append(mess);

            QString line(data);

            QStringList parts = line.split(' ');
            if (parts.size() == 2) {
                QString stringPart = parts[0];
                stringPart = stringPart.left(4);

                float floatPart = parts[1].toFloat();

                qDebug() << "String:" << stringPart << ", Float:" << floatPart;
                QString stringFloat = QString::number(floatPart);

                ui->rec_pl_tb->append(stringPart + " " + stringFloat);

                valueBuff.append(floatPart);
                avalueBuff.append(floatPart);
                if (valueBuff.size() >= 800)
                    valueBuff.removeFirst();

                quint64 currentTime = tick_timer.elapsed() + preTime;
                timeBuff.append(currentTime);
                atimeBuff.append(currentTime);
                if (timeBuff.size() >= 800)
                    timeBuff.removeFirst();

                preTime = currentTime;
                tick_timer.start();

                refBuff.append(refValue);
                arefBuff.append(refValue);
                if (refBuff.size() >= 800)
                    refBuff.removeFirst();

                plotRespond();
            }
            serial.clear();
    }
}

void MainWindow::on_vtun_bt_clicked()
{
     tbRespond("VTUN ");
     tick_timer.start();
}


void MainWindow::on_ptun_bt_clicked()
{
    tbRespond("PTUN ");
    tick_timer.start();
}


void MainWindow::on_stop_bt_clicked()
{
    stop = true;
    QString mess = "STOP \r\n";
    serial.write(mess.toUtf8());
    ui->trans_pl_tb->append(mess);
    tick_timer.elapsed();
    preTime = 0;
    ui->plot->graph(0)->setData(atimeBuff, arefBuff);
    ui->plot->graph(1)->setData(atimeBuff, avalueBuff);
    ui->plot->rescaleAxes();
    ui->plot->replot();
    ui->plot->update();
}


void MainWindow::on_spid_tb_clicked()
{
    stop = false;
    tbRespond("SPID ");

    refBuff.clear();
    timeBuff.clear();
    valueBuff.clear();
    arefBuff.clear();
    atimeBuff.clear();
    avalueBuff.clear();
}


void MainWindow::on_trans_pl_bt_clicked()
{
    ui->trans_pl_tb->clear();
}


void MainWindow::on_rec_pl_bt_clicked()
{
    ui->rec_pl_tb->clear();
}

