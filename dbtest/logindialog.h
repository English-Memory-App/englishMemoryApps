#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "register.h"

namespace Ui {
class logindialog;
}

class logindialog : public QDialog
{
    Q_OBJECT

public:
    explicit logindialog(QWidget *parent = 0);
    ~logindialog();
    registerlog re;

private slots:
    void on_exitBtn_clicked();
    void on_loginBtn_clicked();

    void on_registerBnt_clicked();

private:
    Ui::logindialog *ui;
};

#endif // LOGINDIALOG_H
