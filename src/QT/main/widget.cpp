#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <QTimer>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QCoreApplication>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    m_client = new QMqttClient(this);

    // 从 JSON 文件读取配置
    QString configPath = QCoreApplication::applicationDirPath() + "/../configuration/连接信息.json";
    QFile file(configPath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isNull()) {
            QJsonObject obj = doc.object();
            if (obj.contains("hostname")) hostname = obj["hostname"].toString();
            if (obj.contains("port")) port = obj["port"].toInt();
            if (obj.contains("clientID")) clientID = obj["clientID"].toString();
            if (obj.contains("username")) username = obj["username"].toString();
            if (obj.contains("password")) password = obj["password"].toString();
            if (obj.contains("datatopic")) datatopic = obj["datatopic"].toString();
            if (obj.contains("controltopic")) controltopic = obj["controltopic"].toString();
        }
        file.close();
    }

    ui->hostnamelineEdit->setText(hostname);
    ui->portlineEdit->setText(QString::number(port));
    ui->IDlineEdit->setText(clientID);
    ui->usernamelineEdit->setText(username);
    ui->passwordlineEdit->setText(password);
    ui->topicLineEdit->setText(datatopic);
    // ui->se->setText(controltopic);

    connect(m_client, &QMqttClient::connected, this, &Widget::onConnected);
    connect(m_client, &QMqttClient::disconnected, this, &Widget::onDisconnected);
    connect(m_client, &QMqttClient::errorChanged, this, &Widget::onErrorChanged);
}

void Widget::saveConfiguration()
{
    hostname = ui->hostnamelineEdit->text();
    port = ui->portlineEdit->text().toUShort();
    clientID = ui->IDlineEdit->text();
    username = ui->usernamelineEdit->text();
    password = ui->passwordlineEdit->text();
    QMessageBox::information(this, "提示", "保存成功");
}

void Widget::onConnected()
{
    QMessageBox::information(this, "提示", "连接成功");
    ui->connectButton->setText("断开连接");
}

void Widget::onDisconnected()
{
    QMessageBox::information(this, "提示", "断开连接");
    ui->connectButton->setText("连接");
}

void Widget::onErrorChanged(QMqttClient::ClientError error)
{
    if (error == QMqttClient::NoError) return;

    static const QMap<QMqttClient::ClientError, QString> errorMap = {
        {QMqttClient::InvalidProtocolVersion, "无效的协议版本"},
        {QMqttClient::IdRejected, "客户端ID被拒绝"},
        {QMqttClient::ServerUnavailable, "服务器不可用"},
        {QMqttClient::BadUsernameOrPassword, "用户名或密码错误"},
        {QMqttClient::NotAuthorized, "未授权"},
        {QMqttClient::TransportInvalid, "传输层无效"},
        {QMqttClient::ProtocolViolation, "协议违规"},
        {QMqttClient::UnknownError, "未知错误"}
    };
    QMessageBox::critical(this, "连接失败", errorMap.value(error, "错误代码: " + QString::number(error)));
}

void Widget::connectMqtt()
{
    if (m_client->state() == QMqttClient::Disconnected) {
        m_client->setHostname(hostname);
        m_client->setPort(port);
        m_client->setClientId(clientID);
        m_client->setUsername(username);
        m_client->setPassword(password);
        m_client->setProtocolVersion(QMqttClient::MQTT_5_0);

        QSslSocket *sslSocket = new QSslSocket(this);
        sslSocket->setProtocol(QSsl::TlsV1_2OrLater);
        sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
        sslSocket->connectToHostEncrypted(hostname, port);

        m_client->setTransport(sslSocket, QMqttClient::SecureSocket);

        QTimer::singleShot(100, this, [this]() {
            m_client->connectToHost();
        });
    } else {
        m_client->disconnectFromHost();
    }
}

void Widget::subscribeTopic()
{
    if (m_client->state() != QMqttClient::Connected) {
        QMessageBox::warning(this, "警告", "请先连接到 MQTT 服务器");
        return;
    }

    datatopic = ui->topicLineEdit->text();
    if (datatopic.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入主题");
        return;
    }

    m_subscription = m_client->subscribe(datatopic, 0);
    if (m_subscription) {
        connect(m_subscription, &QMqttSubscription::messageReceived, this, &Widget::onMessageReceived);
        QMessageBox::information(this, "成功", "已订阅主题: " + datatopic);
    } else {
        QMessageBox::critical(this, "错误", "订阅失败");
    }
}

void Widget::onMessageReceived(const QMqttMessage &message)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(message.payload(), &error);
    if (error.error != QJsonParseError::NoError) return;

    QJsonObject obj = doc.object();

    if (obj.contains("temperature")) {
        ui->TemperatureNumber->display(obj["temperature"].toDouble());
    }
    if (obj.contains("humidity")) {
        ui->HumidityNumber->display(obj["humidity"].toDouble());
    }
    if (obj.contains("fan")) {
        QString path = obj["fan"].toInt() ? ":device/picture/FAN_ON.jpg" : ":device/picture/FAN_OFF.jpg";
        ui->FAN_pic->setPixmap(QPixmap(path));
    }
    if (obj.contains("humidifier")) {
        QString path = obj["humidifier"].toInt() ? ":device/picture/Humidifier_ON.png" : ":device/picture/Humidifier_OFF.png";
        ui->humidifier_pic->setPixmap(QPixmap(path));
    }
}

void Widget::publishDeviceState(const QString &device, int state)
{
    QJsonObject obj;
    obj["device"] = device;
    obj["state"] = state;
    m_client->publish(controltopic, QJsonDocument(obj).toJson(), 0);
}

void Widget::openFAN() { publishDeviceState("fan", 1); }
void Widget::closeFAN() { publishDeviceState("fan", 0); }
void Widget::openHumidifier() { publishDeviceState("humidifier", 1); }
void Widget::closeHumidifier() { publishDeviceState("humidifier", 0); }
