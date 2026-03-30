#pragma once

#include <QObject>

class MqttConfig: public QObject
{
    Q_OBJECT

public:
    explicit MqttConfig(QObject *parent = nullptr);

signals:
};
