#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtMqtt/qmqtttype.h>
#include <QtMqtt/qmqttclient.h>
#include <QtMqtt/qmqttmessage.h>
#include <QtMqtt/qmqttsubscription.h>

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
    void onMQTT_Connected();

    void onMQTT_Received(const QMqttMessage &message);

    void on_pushButton_sub_clicked();

    void on_pushButton_pub_clicked();

private:
    Ui::MainWindow *ui;
    QMqttClient *client;
//    QMqttSubscription *sub;
};

#endif // MAINWINDOW_H
