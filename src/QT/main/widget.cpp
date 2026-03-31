#include "widget.h"
#include "ui_widget.h"
#include "qmessagebox.h"
#include <QTimer>

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
    this->hostname=ui->hostnamelineEdit->text();
    this->port=ui->portlineEdit->text().toInt();
    this->clientID=ui->IDlineEdit->text();
    this->username=ui->usernamelineEdit->text();
    this->password=ui->passwordlineEdit->text();
    QMessageBox::information(this,"提示","保存成功");
}
void Widget::onConnected()
{
    QMessageBox::information(this,"提示","连接成功");
    ui->connectButton->setText("断开连接");
}

void Widget::onDisconnected()
{
    QMessageBox::information(this,"提示","断开连接");
    ui->connectButton->setText("连接");
}

void Widget::onErrorChanged(QMqttClient::ClientError error)
{
    if (error != QMqttClient::NoError) {
        QString errorMsg = "";
        switch (error) {
            case QMqttClient::InvalidProtocolVersion:
                errorMsg = "无效的协议版本";
                break;
            case QMqttClient::IdRejected:
                errorMsg = "客户端ID被拒绝";
                break;
            case QMqttClient::ServerUnavailable:
                errorMsg = "服务器不可用";
                break;
            case QMqttClient::BadUsernameOrPassword:
                errorMsg = "用户名或密码错误";
                break;
            case QMqttClient::NotAuthorized:
                errorMsg = "未授权";
                break;
            case QMqttClient::TransportInvalid:
                errorMsg = "传输层无效";
                break;
            case QMqttClient::ProtocolViolation:
                errorMsg = "协议违规";
                break;
            case QMqttClient::UnknownError:
                errorMsg = "未知错误";
                break;
            default:
                errorMsg = "错误代码: " + QString::number(error);
                break;
        }
        QMessageBox::critical(this, "连接失败", errorMsg);
    }
}

void Widget::onSslErrors(const QList<QSslError> &errors)
{
    QString errorDetails = "SSL 传输层错误:\n";
    for (const QSslError &error : errors) {
        errorDetails += "- " + error.errorString() + "\n";
    }
    QMessageBox::warning(this, "SSL/TLS 错误", errorDetails);
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