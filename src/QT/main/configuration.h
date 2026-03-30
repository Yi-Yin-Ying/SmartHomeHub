#pragma once

#include <QWidget>
#include "MqttConfig.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Form; }
QT_END_NAMESPACE


class Configuration: public QWidget
{
    Q_OBJECT

public:
    explicit Configuration(QWidget *parent = nullptr);
    ~Configuration();
    void setConfig(MqttConfig mqttConfig);
private:
    Ui::Form *ui;
    MqttConfig m_mqttConfig;
};
