#include "widget.h"
#include "ui_widget.h"
#include "QMessageBox.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::openConfiguration()
{
    QMessageBox::information(nullptr, "调试", "程序运行到这里了");
}
