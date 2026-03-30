#include "configuration.h"
#include "ui_configuration.h"
#include "qmessagebox.h"
Configuration::Configuration(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);

    ui->hostnameLineEdit->setText(m_mqttConfig->getHostname());
    ui->portLineEdit->setText(QString::number(m_mqttConfig->getPort()));
    ui->IDLineEdit->setText(m_mqttConfig->getClientId());
    ui->usernamelineEdit->setText(m_mqttConfig->getUsername());
    ui->passwordlineEdit->setText(m_mqttConfig->getPassword());
}

Configuration::~Configuration()
{
    delete ui;
}

void Configuration::setConfig(MqttConfig &mqttConfig)
{
    this->m_mqttConfig=&mqttConfig;
}

void Configuration::save(){
    m_mqttConfig->setHostname(ui->hostnameLineEdit->text());
    m_mqttConfig->setPort(ui->portLineEdit->text().toInt());
    m_mqttConfig->setClientId(ui->IDLineEdit->text());
    m_mqttConfig->setUsername(ui->usernamelineEdit->text());
    m_mqttConfig->setPassword(ui->passwordlineEdit->text());
    QMessageBox(QMessageBox::Information, "提示", "保存成功");
}
void Configuration::quit()
{
    this->close();

}
