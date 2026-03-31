#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <QTimer>
#include <QMap>
#include <QDateTime>

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
    m_client=new QMqttClient(this);

    hostname="s6b22c1f.ala.cn-hangzhou.emqxsl.cn";
    port=8883;
    clientID="emqx_cloudf37bc1";
    username="QT";
    password="123456";

    ui->hostnamelineEdit->setText(hostname);
    ui->portlineEdit->setText(QString::number(port));
    ui->IDlineEdit->setText(clientID);
    ui->usernamelineEdit->setText(username);
    ui->passwordlineEdit->setText(password);

    connect(m_client,&QMqttClient::connected,this,&Widget::onConnected);
    connect(m_client,&QMqttClient::disconnected,this,&Widget::onDisconnected);
    connect(m_client,&QMqttClient::errorChanged,this,&Widget::onErrorChanged);
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
    if (error != QMqttClient::NoError) {
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
        QString errorMsg = errorMap.value(error, "错误代码: " + QString::number(error));
        QMessageBox::critical(this, "连接失败", errorMsg);
    }
}

void Widget::onSslErrors(const QList<QSslError> &errors)
{
    QStringList errorList;
    for (const QSslError &error : errors) {
        errorList << error.errorString();
    }
    QMessageBox::warning(this, "SSL/TLS 错误", "SSL 传输层错误:\n" + errorList.join("\n- "));
}

void Widget::connectMqtt()
{
    if(m_client->state()==QMqttClient::Disconnected){
        // 设置 MQTT 客户端属性
        m_client->setHostname(hostname);
        m_client->setPort(port);
        m_client->setClientId(clientID);
        m_client->setUsername(username);
        m_client->setPassword(password);

        // EMQX Serverless 推荐使用 MQTT 5.0
        m_client->setProtocolVersion(QMqttClient::MQTT_5_0);

        // 创建并配置 SSL socket
        QSslSocket *sslSocket = new QSslSocket(this);
        sslSocket->setProtocol(QSsl::TlsV1_2OrLater);
        sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);

        // 关键：必须先让 socket 连接到服务器
        sslSocket->connectToHostEncrypted(hostname, port);

        // 设置传输层
        m_client->setTransport(sslSocket, QMqttClient::SecureSocket);

        // 使用小延迟确保 socket SSL 握手完成
        QTimer::singleShot(100, this, [this]() {
            m_client->connectToHost();
        });
    }else{
        m_client->disconnectFromHost();
    }
}

void Widget::subscribeTopic()
{
    if (m_client->state() != QMqttClient::Connected) {
        QMessageBox::warning(this, "警告", "请先连接到 MQTT 服务器");
        return;
    }

    topic = ui->topicLineEdit->text();
    if (topic.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入主题");
        return;
    }

    // 创建订阅，QoS 0
    m_subscription = m_client->subscribe(topic, 0);

    if (m_subscription) {
        connect(m_subscription, &QMqttSubscription::messageReceived,
                this, &Widget::onMessageReceived);
        QMessageBox::information(this, "成功", "已订阅主题: " + topic);
    } else {
        QMessageBox::critical(this, "错误", "订阅失败");
    }
}

void Widget::sendMessage()
{
    if (m_client->state() != QMqttClient::Connected) {
        QMessageBox::warning(this, "警告", "请先连接到 MQTT 服务器");
        return;
    }

    QString topic = ui->sendTopiclineEdit->text();
    QString payload = ui->sendEdit->toPlainText();

    if (topic.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入主题");
        return;
    }

    if (payload.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入消息内容");
        return;
    }

    m_client->publish(topic, payload.toUtf8(), 0);

    QString msg = QString("[%1] 已发送: %2")
                    .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                    .arg(payload);
    ui->messageTextEdit->append(msg);
}

void Widget::onMessageReceived(const QMqttMessage &message)
{
    QString msg = QString("[%1] %2: %3")
                    .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                    .arg(message.topic().name())
                    .arg(QString::fromUtf8(message.payload()));

    ui->messageTextEdit->append(msg);
}