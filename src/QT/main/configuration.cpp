#include "configuration.h"
#include "ui_configuration.h"
Configuration::Configuration(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
}

Configuration::~Configuration()
{
    delete ui;
}

void Configuration::setConfig(MqttConfig mqttConfig)
{
    this->m_mqttConfig=mqttConfig;
}
