#include "modify.h"
#include "ui_modify.h"

Modify::Modify(QWidget *parent) :
    MyDialog(parent),   //QDialog(parent),
    ui(new Ui::Modify)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("单词卡"));

    db4 = QSqlDatabase::addDatabase("QSQLITE","db4");
    db4.setDatabaseName("collection.db");
    db4.open();
    QSqlQuery query2(db4);
    query2.exec("CREATE TABLE collection (word TEXT, pos TEXT, definition TEXT, example TEXT)");
}

Modify::~Modify()
{
    delete ui;
}




void Modify::modify(int num)   //显示选中的单词的属性
{
    id = num;
    QSqlQuery query(db);
    //找到指定id的单词，%1是保证值为整数
    query.prepare(QString("select * from around10k where levels = %1").arg(id));
    //query.bindValue(":id",id);
    query.exec();
    query.next();
    QString word = query.value(0).toString();
    QString type = query.value(2).toString();
    QString definition = query.value(3).toString();
    QString example = query.value(4).toString();
    ui->lineEdit_3->setText(word);
    ui->lineEdit_4->setText(type);
    ui->textEdit_3->setText(definition);
    ui->textEdit_4->setText(example);
    ui->lineEdit_3->setReadOnly(true);      //设置单词对应的输入栏不可编辑
    ui->lineEdit_4->setReadOnly(true);
    ui->textEdit_3->setReadOnly(true);
    ui->textEdit_4->setReadOnly(true);
}

void Modify::on_upBtn_clicked()
{
    id++;
    if( id==14342 )
    {
        QMessageBox::warning(this,"这是最后一个单词！！","将返回第一个单词",QMessageBox::Yes);
        id=1;
    }
    modify(id);
}

void Modify::on_dnBtn_clicked()
{
    id--;
    if( id==0 )
    {
        QMessageBox::warning(this,"这是第一个单词！","将返回最后一个单词",QMessageBox::Yes);
        id=14341;
    }
    modify(id);
}

void Modify::on_colBtn_clicked()
{

    // 获取四个lineEdit的文本内容
        QString word = ui->lineEdit_3->text();
        QString pos = ui->lineEdit_4->text();
        QString definition = ui->textEdit_3->toPlainText();
        QString example = ui->textEdit_4->toPlainText();

        if (!db4.open()) {
            qDebug() << "Failed to open database!";
            return;
        }

        // 创建一个表来存储单词、词性、释义和例句四个字段
        QSqlQuery query(db4);
        if (!query.exec("CREATE TABLE IF NOT EXISTS collection ("
                        "word TEXT PRIMARY KEY,"
                        "pos TEXT,"
                        "definition TEXT,"
                        "example TEXT"
                        ")")) {
            qDebug() << "Failed to create table!";
            db4.close();
            return;
        }

        // 将四个lineEdit的文本内容插入到该表中
        query.prepare("INSERT INTO collection (word, pos, definition, example) "
                      "VALUES (:word, :pos, :definition, :example)");
        query.bindValue(":word", word);
        query.bindValue(":pos", pos);
        query.bindValue(":definition", definition);
        query.bindValue(":example", example);
        if (!query.exec()) {
            qDebug() << "Failed to insert data!";
        }
        if (query.exec("SELECT COUNT(*) FROM collection")) {
            if (query.next() && query.value(0).toInt() > 0) {
                   QMessageBox::information(this, tr("成功"), tr("收藏成功."));             // 表不为空
            } else {
                   QMessageBox::warning(this,"收藏执行！","empty",QMessageBox::Yes);// 表为空
            }
        } else {
                    QMessageBox::warning(this,"收藏执行！","fail to query",QMessageBox::Yes);// 查询失败
        }
//        else if(query.exec())
//        {
//            QMessageBox::warning(this,"收藏执行！","***",QMessageBox::Yes);
//        }
        // 断开数据库连接
        db4.close();
}
