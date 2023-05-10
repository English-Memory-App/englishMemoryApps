#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include "modify.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void sqlconn();
    void sqlshow();
    QSqlDatabase db;        //用QSqlDatabase声明一个数据库类，名为db

    void show_table();
    QSqlQueryModel *model;
    Modify d3;    //学习属性对话框
    int sum;


    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_learnBtn_clicked();

    void on_search_nBtn_clicked();

    void on_wordBtn_clicked();

    void on_collBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
