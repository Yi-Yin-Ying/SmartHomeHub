#pragma once

#include <QWidget>
#include <QtMqtt>
QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    // void connect(const string& hostname){

    // }
private slots: 
    void openConfiguration();

private:
    Ui::Widget *ui;
};
