#pragma once

#include <QWidget>
#include <QMqttClient>
#include <QSslSocket>
#include <QSslConfiguration> // SSL 配置（可选，用于高级设置）
#include <QSslCertificate>   // 证书处理（用于加载 CA 证书）
#include <QFile>             // 读取证书文件
#include <Qstring>

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
    void init();
private slots:
void connectMqtt();
void saveConfiguration();
void onConnected();
void onDisconnected();
void onErrorChanged(QMqttClient::ClientError error);
void onSslErrors(const QList<QSslError> &errors);
void subscribeTopic();
void sendMessage();
void onMessageReceived(const QMqttMessage &message);
private:
    Ui::Widget *ui;

    QMqttClient *m_client;
    QMqttSubscription *m_subscription = nullptr;

    QString hostname="";
    uint16_t port=0;
    QString clientID="";
    QString username="";
    QString password="";

    QString topic="test";
};
