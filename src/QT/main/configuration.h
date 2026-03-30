#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Form; }
QT_END_NAMESPACE


class Configuration: public QWidget
{
    Q_OBJECT

public:
    explicit Configuration(QWidget *parent = nullptr);
    ~Configuration();

private:
    Ui::Form *ui;
};
