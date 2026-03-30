#include "widget.h"
#include "ui_widget.h"
#include "QMessageBox.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_mqttConfig=new MqttConfig();
    m_mqttConfig->setClientId("test");
    m_mqttConfig->setHostname("127.0.0.1");
    m_mqttConfig->setPassword("<PASSWORD>");
    m_mqttConfig->setPort(1883);
    m_mqttConfig->setUsername("test");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::connectMqtt()
{
}

void Widget::openConfiguration()
{
    // QMessageBox::information(nullptr, "调试", "程序运行到这里了");
    Configuration *configuration = new Configuration();
    configuration->setAttribute(Qt::WA_DeleteOnClose);  // 窗口关闭时自动删除
    configuration->setConfig(*m_mqttConfig);
    configuration->show();
}