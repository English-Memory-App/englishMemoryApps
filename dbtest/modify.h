#ifndef MODIFY_H
#define MODIFY_H

#include <QDialog>
#include <QSqlQuery>
#include <QtSql>
#include <QtGui>
#include <QtCore>
#include <QMessageBox>
#include "mydialog.h"
namespace Ui {
class Modify;
}

class Modify : public MyDialog //public QDialog
{
    Q_OBJECT

public:
    explicit Modify(QWidget *parent = 0);
    ~Modify();

    void modify(int num);
    QSqlDatabase db;//总单词数据库
    QSqlDatabase db4;//收藏单词数据库
private slots:


    void on_upBtn_clicked();

    void on_dnBtn_clicked();

    void on_colBtn_clicked();

private:
    Ui::Modify *ui;

    int id;
};

#endif // MODIFY_H
