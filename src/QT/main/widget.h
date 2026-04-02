#pragma once

#include <QWidget>
#include <QMqttClient>
#include <QSslSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void connectMqtt();
    void saveConfiguration();
    void onConnected();
    void onDisconnected();
    void onErrorChanged(QMqttClient::ClientError error);
    void subscribeTopic();
    void onMessageReceived(const QMqttMessage &message);
    void openFAN();
    void closeFAN();
    void openHumidifier();
    void closeHumidifier();

private:
    void init();
    void publishDeviceState(const QString &device, int state);

    Ui::Widget *ui;
    QMqttClient *m_client;
    QMqttSubscription *m_subscription = nullptr;

    QString hostname;
    uint16_t port = 8883;
    QString clientID;
    QString username;
    QString password;
    QString datatopic = "Sensor/Data";
    QString controltopic = "Device/Control";
};
