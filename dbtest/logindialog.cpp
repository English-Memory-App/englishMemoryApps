#include "logindialog.h"
#include "ui_logindialog.h"
#include "register.h"


logindialog::logindialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logindialog)
{
    ui->setupUi(this);
    setWindowTitle("登录");
}

logindialog::~logindialog()
{
    delete ui;
}


void logindialog::on_loginBtn_clicked()
{
    QString username = ui->usrLineEdit->text();
    QString password = ui->pwdLineEdit->text();
    // 从数据库中查询用户
    re.db2.open();
    QSqlQuery query(re.db2);
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (!query.exec())
    {
        QMessageBox::critical(this, tr("Error"), tr("Failed to query database."));
        return;
    }

    // 如果查询到用户，则允许登录
    if (query.next())
    {
        QMessageBox::information(this, tr("Success"), tr("Login successful."));
         accept();
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Username or password is incorrect."));
    }
}



void logindialog::on_exitBtn_clicked()
{
    close();
}

void logindialog::on_registerBnt_clicked()
{
    // 显示注册窗口
    re.show();
    re.raise(); //只能同时存在一个对话框，再按增加按钮会让对话框显示到最上层
    // 连接到re界面的槽函数
     connect(&re, &registerlog::registered, this, [=]() {
         // 在这里执行注册成功后的操作
         // 例如关闭re界面并返回到logindialog界面
         re.close();
         this->show();
     });
}
