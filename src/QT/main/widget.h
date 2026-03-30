#pragma once

#include <QWidget>
#include <QMqttClient>
#include <QMqttSubscription>
#include "Configuration.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    // void connect(const string& hostname){

    // }
private slots: 
    void openConfiguration();
    void connectMqtt();

private:
    Ui::Widget *ui;
    MqttConfig *m_mqttConfig;
    QMqttClient *m_mqttClient;           // MQTT 客户端
    QMqttSubscription *m_subscription;   // 订阅对象（可选）
};
