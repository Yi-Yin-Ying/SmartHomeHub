#pragma once


#include <QString>
class MqttConfig
{

public:
    explicit MqttConfig()=default;

    void setHostname(const QString &Hostname);
    void setPort(const quint16 Port);
    void setClientId(const QString &ClientId);
    
    void setUsername(const QString &Username);
    void setPassword(const QString &Password);

    QString getHostname() const;
    quint16 getPort() const;
    QString getClientId() const;
    QString getUsername() const;
    QString getPassword() const;
private:
    QString m_hostname;   // 主机名
    quint16 m_port;       // 端口号
    QString m_clientId;   // 客户端ID
    QString m_username;   // 用户名
    QString m_password;   // 密码
};
