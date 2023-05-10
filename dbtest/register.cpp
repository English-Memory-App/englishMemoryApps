#include "register.h"
#include "ui_register.h"

registerlog::registerlog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerlog)
{
    ui->setupUi(this);
    setWindowTitle("注册");
    setModal(true);
    // 将用户名和密码插入数据库
    db2 = QSqlDatabase::addDatabase("QSQLITE","db2");
    db2.setDatabaseName("users");
    db2.open();
    QSqlQuery query(db2);
    query.exec("CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT)");
}


void registerlog::registered()
{
    // 获取用户名和密码
    QString username = ui->lineEdit1->text();
    QString password = ui->lineEdit2->text();
    QString confirmPassword = ui->lineEdit3->text();

    // 检查确认密码是否匹配
    if (password != confirmPassword)
    {
        QMessageBox::critical(this, tr("Error"), tr("Passwords do not match."));
        return;
    }
    db2.open();
    QSqlQuery query(db2);
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (!query.exec())
    {
        QMessageBox::critical(this, tr("Error"), tr("Failed to register."));
        return;
    }
    QMessageBox::information(this, tr("Success"), tr("Registered successfully."));
}

registerlog::~registerlog()
{
    delete ui;
    db2.close();
}

void registerlog::on_upBtn_clicked()
{
    registered();
}
