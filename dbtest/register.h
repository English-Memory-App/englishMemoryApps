#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include "ui_register.h"
#include <QtSql>
#include <QtGui>
#include <QtCore>
#include <QMessageBox>


namespace Ui {
class registerlog;
}

class registerlog : public QDialog
{
    Q_OBJECT

public:
    explicit registerlog(QWidget *parent = nullptr);
    ~registerlog();

    QSqlDatabase db2;
    void registered();

private slots:
    void on_upBtn_clicked();

private:
    Ui::registerlog *ui;
};

#endif // REGISTER_H
