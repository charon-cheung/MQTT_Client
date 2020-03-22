#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMqtt/qmqttglobal.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new QMqttClient(this);
    client->setProtocolVersion(QMqttClient::MQTT_3_1);
    client->setHostname("192.168.0.109");
    client->setPort(1883);
    client->connectToHost();

#if 0  // 不能在构造函数里订阅，因为客户端在connected信号对应得槽函数里才连接成功
    auto sub = client->subscribe(QLatin1String("qtmqtt/test"), 1);

    if(!sub)
        qDebug()<<"sub is nullptr";
#endif

    connect(client, SIGNAL(connected()), this, SLOT(onMQTT_Connected()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMQTT_Connected()
{
    qDebug()<<"--------------";
    ui->label_state->setText("连接状态 -> 连接成功 !");

    QString topic = ui->lineEdit_topicSub->text();
    QMqttSubscriptionProperties subProps;
    subProps.setSubscriptionIdentifier(40);
    QMqttSubscription* sub = client->subscribe(topic, subProps, 1);

    if(!sub)
        ui->plainTextEdit->appendPlainText("订阅失败!");
    else
        ui->plainTextEdit->appendPlainText("已订阅主题 -> " + topic);
    connect(sub, SIGNAL(messageReceived(const QMqttMessage &) ), this, SLOT(onMQTT_Received(const QMqttMessage &)));

}

void MainWindow::onMQTT_Received(const QMqttMessage &message)
{
    ui->plainTextEdit->appendPlainText("主题: " + message.topic().name());
    ui->plainTextEdit->appendPlainText("消息: " + message.payload());
}

// 订阅按钮槽函数
void MainWindow::on_pushButton_sub_clicked()
{
    QString topicSub = ui->lineEdit_topicSub->text();
    quint8 qosSub = ui->comboBox_qosSub->currentIndex();

    QMqttSubscriptionProperties subProps;
    subProps.setSubscriptionIdentifier(40);

    auto sub = client->subscribe(QLatin1String("test"), qosSub);
    if(!sub)
        qDebug()<<"sub is nullptr";
    else
        qDebug()<<"sub topic: "<<sub->topic().isValid();
}

// 发布按钮槽函数
void MainWindow::on_pushButton_pub_clicked()
{
    QString topicPub = ui->lineEdit_topicPub->text();
    QString messagePub = ui->lineEdit_mes->text();
    client->publish(QMqttTopicName(topicPub), messagePub.toUtf8() );
}
