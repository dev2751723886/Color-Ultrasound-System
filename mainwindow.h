#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include<QMessageBox>
#include<vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<QSqlTableModel>
#include<QTimer>
using namespace cv;
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initMainWindow();
    void ctImgShow();
    void onTableSelectChange(int row);
    void ctImgRead();
    void ctImgProc();
    void ctImgSave();
    void ctImgHoughCircles();
    void showUserPhoto();

private slots:
    void onTimeOut();

    void on_startPushButton_clicked();

    void on_basicTableView_clicked(const QModelIndex &index);

    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::MainWindow *ui;
    Mat myCtImg;
    Mat myCtGrayImg;
    QImage myCtQImage;
    QSqlTableModel *model;
    QSqlTableModel *model_d;
    QTimer *myTimer;
};

#include <QDebug>
#include <QSqlError>
static bool createMySqlConn()
{

    QSqlDatabase sqldb=QSqlDatabase::addDatabase("QMYSQL");

    sqldb.setHostName("127.0.0.1");
    sqldb.setPort(3306);
    sqldb.setDatabaseName("patient");
    sqldb.setUserName("root");
    sqldb.setPassword("123456");

    qDebug() << sqldb.connectOptions();
    if(!sqldb.open())
    {
        qDebug() << sqldb.lastError();
        QMessageBox::critical(0,u8"后台数据库连接失败",u8"无法创建连接！请检查排除故障后重启程序。",QMessageBox::Cancel);
        exit(-1);
        return  false;
    }
    return  true;
}

#endif // MAINWINDOW_H
