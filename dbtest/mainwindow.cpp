#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QVariant>
#include <QStringList>
#include<QTableView>
#include<QHBoxLayout>
#include <QStack>
#include <QVariantList>
QList<QSqlRecord> deletedRecords;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("单词本");
    //QSqlTableModel为读写模型,QSqlQueryModel为用来查询的只读模型
    model = new QSqlQueryModel(ui->tableView);
    sqlconn();
    sqlshow();
}

void MainWindow::sqlconn(){
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
            db.setHostName("myhost");
            //.setDatabaseName("C:\\Users\\Lenovo\\Desktop\\SE\\dbtest_end2\\dbtest\\test");
            //获取数据库的路径dbPath
            QString dirPath = QCoreApplication::applicationDirPath();
            dirPath.replace("/", "\\");
            int index = dirPath.indexOf("\\build-");
            QString dbPath = dirPath.left(index) + "\\dbtest\\test.db";

            db.setDatabaseName(dbPath);
            if (!db.open())
               {
               qDebug("数据库不能打开");
               }
}


void MainWindow::sqlshow(){
    QSqlQuery query(db);
    query.exec("select levels,words,pronounce,chinses,senences from around10k ");
    model->setQuery(query);   //把从数据库查询到的结果集存到model中
    show_table();
    sum=0;
    query.exec("select * from around10k ");
    while(1){
        if(query.next())
            ++sum;
        else
            break;
    }
    ui->label->setText("单词总量："+QString::number(sum));
}

void MainWindow::show_table()      //设置表格的显示内容和格式
{
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));  //设置tableview的表头
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("单词"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("词性"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("释义"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("例句"));
    //Qt中用model/view模式来显示数据，将tableView与存储数据的model关联后才有显示的内容
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setVisible(false);//隐藏表格的行号
    //设置选中时为整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //列宽随窗口大小改变而改变，每列平均分配，充满整个表，但是此时列宽不能拖动进行改变
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}


MainWindow::~MainWindow()
{
    db.close();      //关闭数据库
    delete ui;
}

// 定义计算编辑距离的函数
int calculateEditDistance(const QString& word1, const QString& word2)
{
    int m = word1.length();
    int n = word2.length();

    // 创建一个二维数组来保存编辑距离
    QVector<QVector<int>> dp(m + 1, QVector<int>(n + 1, 0));

    // 初始化第一行和第一列
    for (int i = 1; i <= m; ++i)
        dp[i][0] = i;

    for (int j = 1; j <= n; ++j)
        dp[0][j] = j;

    // 动态规划计算编辑距离
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (word1[i - 1] == word2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + qMin(dp[i - 1][j - 1], qMin(dp[i][j - 1], dp[i - 1][j]));
        }
    }

    return dp[m][n];
}



void MainWindow::on_learnBtn_clicked()
{
     //获取鼠标在tableView中选中的单元格的index即索引
    QModelIndex index = ui->tableView->currentIndex();
    //不知道未选中时currentIndex()的返回值是什么，所以另外新建一个index_2来判断index是否赋值了
    QModelIndex index_2;
    if( index == index_2 )
    {
        QMessageBox::warning(this,"打开失败","没有选中单词！",QMessageBox::Yes);
        return;
    }
    int row = ui->tableView->currentIndex().row();
    int num = index.sibling(row,0).data().toInt();
    d3.modify(num);
    d3.show();
    d3.raise(); //只能同时存在一个对话框，再按增加按钮会让对话框显示到最上层
}


//void MainWindow::on_search_nBtn_clicked()
//{
//    QString input = ui->searchlineEdit->text();
//    if(input.isEmpty())      //判断是否有输入查询内容
//    {
//        QMessageBox::warning(this,"查询失败","请输入要查询的id！",QMessageBox::Yes);
//        return;
//    }
//    QSqlQuery query;
//    query.prepare(QString("select levels,words,pronounce,chinses,senences from around10k where levels = %1").arg(input));
//    query.exec();
//    if(!query.next())
//    {
//        QMessageBox::warning(this,"查询失败","没有查找到该id！",QMessageBox::Yes);
//        return;
//    }
//    model->setQuery(query);   //重新设置model的内容
//    show_table();
//}

void MainWindow::on_search_nBtn_clicked()
{
    QString input = ui->searchlineEdit->text();
    if (input.isEmpty())      //判断是否有输入查询内容
    {
        QMessageBox::warning(this, "查询失败", "请输入要查询的单词！", QMessageBox::Yes);
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT words FROM around10k");

    if (query.exec()) {
        QStringList results;

        while (query.next()) {
            QString word = query.value(0).toString();
            int distance = calculateEditDistance(input, word);

            // 如果单词与搜索词的编辑距离小于等于指定的最大距离，则将其添加到结果列表中
            if (distance <= 1) {
                results.append(word);
            }
        }

        if (results.isEmpty()) {
            QMessageBox::warning(this, "查询失败", "没有查找到相关结果！", QMessageBox::Yes);
            return;
        }

        // 创建一个新的模型，并将结果列表中的单词作为条件重新查询
        //QSqlQueryModel* resultModel = new QSqlQueryModel(this);
        model->setQuery("SELECT levels, words, pronounce, chinses, senences FROM around10k WHERE words IN ('" + results.join("', '") + "')");
        show_table();
    }
}

void MainWindow::on_wordBtn_clicked()
{
    sqlshow();
}


void MainWindow::on_collBtn_clicked()
{
    // 连接到数据库
    //d3.db4= QSqlDatabase::addDatabase("QSQLITE","db4");
   // d3.db4.setDatabaseName("collection");

    d3.db4.open();
    if (!d3.db4.open())
    {
        qDebug() << "Failed to open database!";
        return;
    }
    // 查询所有行并从表中选择四个字段
    QSqlQuery query(d3.db4);


//            if (query.exec("SELECT COUNT(*) FROM collection")) {
//                if (query.next() && query.value(0).toInt() > 0) {
//                       QMessageBox::warning(this,"收藏执行！","表不为空",QMessageBox::Yes);             // 表不为空
//                } else {
//                       QMessageBox::warning(this,"收藏执行！","empty",QMessageBox::Yes);// 表为空
//                }
//            } else {
//                        QMessageBox::warning(this,"收藏执行！","fail to query",QMessageBox::Yes);// 查询失败
//            }


    //删除收藏表中的数据
//    query.exec("delete from collection where 1=1");

    if (!query.exec("SELECT word, pos, definition, example FROM collection")) {
        qDebug() << "Failed to query database!";
        d3.db4.close();
        return;
    }


// int rowCount = 0;

//    // 遍历查询结果，将每行的四个字段的值添加到QStringList中
//    QStringList wordList;
//    while (query.next()) {
//         rowCount++;
//        QString word = query.value(0).toString();
//        QString pos = query.value(1).toString();
//        QString definition = query.value(2).toString();
//        QString example = query.value(3).toString();
//        wordList.append(word + " (" + pos + "): " + definition + "\n" + example + "\n");
//    }

//    // 将QStringList中的值连接为一个QString，并在QMessageBox中显示它
//    QString message;
//    if (rowCount == 0) {
//        message = "rowCount为0";
//    } else {
//        message = wordList.join("\n");
//    }

//    if (message.isEmpty()) {
//        message = "No words have been collected yet.";
//    }
//    QMessageBox::information(this, "Collection", message);


//    // 在QTableView中显示结果
//    QMainWindow *mainWindow = new QMainWindow(this);
//    QTableView *view = new QTableView(mainWindow);
//    view->setModel(model);
//    mainWindow->setCentralWidget(view);
//    mainWindow->setWindowState(Qt::WindowMaximized);

    QSqlTableModel *model = new QSqlTableModel(this, d3.db4);
    model->setTable("collection");
    if (!model->select()) {
        qDebug() << "Failed to select data from database!";
        d3.db4.close();
        return;
    }

    QTableView *view = new QTableView();
    view->setModel(model);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers); // 设置表格不可编辑
    //view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QMainWindow *mainWindow = new QMainWindow();
    mainWindow->setCentralWidget(view);
    mainWindow->setWindowState(Qt::WindowMaximized);





    // 获取表格的水平表头
    QHeaderView *header = view->horizontalHeader();

//    // 设置每一列的宽度为相等的
//    header->setSectionResizeMode(QHeaderView::Stretch);
    // 设置第一列的宽度为100
    header->resizeSection(0, 100);

    // 设置第二列的宽度为200
    header->resizeSection(1, 200);

    // 设置第三列的宽度为300
    header->resizeSection(2, 300);

    // 将剩余的列的宽度拉伸以填满整个表格
    header->setStretchLastSection(true);

//    // 添加删除行按钮
//    QToolBar *toolBar = new QToolBar();
//    QAction *deleteAction = new QAction("取消收藏", toolBar);
//    toolBar->addAction(deleteAction);
//    mainWindow->addToolBar(toolBar);

//    // 连接删除行按钮的槽函数
//    QObject::connect(deleteAction, &QAction::triggered, [=]() {
//        QModelIndexList selection = view->selectionModel()->selectedRows();
//        if (!selection.empty()) {
//            int row = selection.at(0).row();
//            model->removeRow(row);
//            model->submitAll();
//        }
//    });

//    mainWindow->show();


//    // 添加删除行和刷新按钮
//    QToolBar *toolBar = new QToolBar();
//    QAction *deleteAction = new QAction("取消收藏", toolBar);
//    QAction *refreshAction = new QAction("刷新", toolBar);
//    toolBar->addAction(deleteAction);
//    toolBar->addAction(refreshAction);
//    mainWindow->addToolBar(toolBar);

//    // 连接删除行按钮的槽函数
//    QObject::connect(deleteAction, &QAction::triggered, [=]() {
//        QModelIndexList selection = view->selectionModel()->selectedRows();
//        if (!selection.empty()) {
//            int row = selection.at(0).row();
//            model->removeRow(row);
//            model->submitAll();
//            model->select(); // 重新查询数据
//        }
//    });

//    // 连接刷新按钮的槽函数
//    QObject::connect(refreshAction, &QAction::triggered, [=]() {
//        model->select(); // 重新查询数据
//    });

//    mainWindow->show();


    // 声明 QItemSelectionModel 对象
    QItemSelectionModel *selectionModel = view->selectionModel();

    // 添加删除行和撤销按钮
    QToolBar *toolBar = new QToolBar();
    QAction *deleteAction = new QAction("删除行", toolBar);
    QAction *undoAction = new QAction("撤销删除", toolBar);
    toolBar->addActions({deleteAction, undoAction});
    mainWindow->addToolBar(toolBar);

    // 连接删除行按钮的槽函数
    QObject::connect(deleteAction, &QAction::triggered, [=]() {
        QModelIndexList selection = selectionModel->selectedRows();
        if (!selection.isEmpty()) {
            // 保存被删除的数据
            deletedRecords.append(model->record(selection.at(0).row()));
            // 从表格中删除该行
            model->removeRow(selection.at(0).row());
            model->submitAll();
            model->select(); // 重新查询数据
        }
    });

    // 连接撤销删除按钮的槽函数
    QObject::connect(undoAction, &QAction::triggered, [=]() {
        if (!deletedRecords.isEmpty()) {
            // 从删除记录中取出最后一条数据，并在表格中添加该行
            QSqlRecord record = deletedRecords.takeLast();
            model->insertRecord(-1, record);
            model->submitAll();
           // model->select(); // 重新查询数据
        }
    });

    mainWindow->show();




    // 断开数据库连接
    if (!query.exec("SELECT word, pos, definition, example FROM collection")) {
        qDebug() << "Failed to query database: " << query.lastError().text();
        d3.db4.close();
        return;
    }

}
