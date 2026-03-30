#include "MqttConfig.h"


// ========== Getter 实现 ==========
QString MqttConfig::getHostname() const
{
    return m_hostname;
}

quint16 MqttConfig::getPort() const
{
    return m_port;
}

QString MqttConfig::getClientId() const
{
    return m_clientId;
}

QString MqttConfig::getUsername() const
{
    return m_username;
}

QString MqttConfig::getPassword() const
{
    return m_password;
}

// ========== Setter 实现 ==========
void MqttConfig::setHostname(const QString &Hostname)
{
    m_hostname = Hostname;
}

void MqttConfig::setPort(quint16 Port)
{
    m_port = Port;
}

void MqttConfig::setClientId(const QString &ClientId)
{
    m_clientId = ClientId;
}

void MqttConfig::setUsername(const QString &Username)
{
    m_username = Username;
}

void MqttConfig::setPassword(const QString &Password)
{
    m_password = Password;
}