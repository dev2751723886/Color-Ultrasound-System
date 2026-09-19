#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QBuffer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();

    model=new QSqlTableModel(this);
    model->setTable("basic_inf");
    model->select();
    model_d=new QSqlTableModel(this);
    model_d->setTable("details_inf");
    model_d->select();
    ui->basicTableView->setModel(model);
    onTableSelectChange(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{
    QString ctImgPath="Tumor.jpg";
    Mat ctImg=imread(ctImgPath.toLatin1().data());
    cvtColor(ctImg,ctImg,COLOR_BGR2RGB);
    myCtImg=ctImg;
    myCtQImage=QImage((const unsigned char*)(ctImg.data),ctImg.cols,ctImg.rows,QImage::Format_RGB888);
    ctImgShow();

    QDate date=QDate::currentDate();
    int year=date.year();
    ui->yearLcdNumber->display(year);
    int month=date.month();
    ui->monthLcdNumber->display(month);
    int day=date.day();
    ui->dayLcdNumber->display(day);
    myTimer=new QTimer();
    myTimer->setInterval(1000);
    myTimer->start();
    connect(myTimer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
}

void MainWindow::ctImgShow()
{
    ui->CT_Img_Label->setPixmap(QPixmap::fromImage(myCtQImage.scaled(ui->CT_Img_Label->size(),Qt::KeepAspectRatio)));
}

void MainWindow::onTableSelectChange(int row)
{
    int r=1;
    if(row!=0) r=ui->basicTableView->currentIndex().row();
    QModelIndex index;
    index=model->index(r,1);
    ui->nameLabel->setText(model->data(index).toString());
    index=model->index(r,2);
    QString sex=model->data(index).toString();
    (sex.compare("男")==0)?ui->maleRadioButton->setChecked(true):ui->femaleRadioButton->setChecked(true);
    index=model->index(r,4);
    QDate date;
    int now=date.currentDate().year();
    int bir=model->data(index).toDate().year();
    ui->ageSpinBox->setValue(now-bir);
    index=model->index(r,3);
    QString ethnic=model->data(index).toString();
    ui->ethniComboBox->setCurrentText(ethnic);
    index=model->index(r,0);
    QString ssn=model->data(index).toString();
    ui->ssnLineEdit->setText(ssn);
    showUserPhoto();
}

void MainWindow::ctImgRead()
{
    QString ctImgName=QFileDialog::getOpenFileName(this,u8"载入CT相片",".","Image File(*.png *.jpg *.jpeg *.bmp)");
    if(ctImgName.isEmpty()) return;
    Mat ctRgbImg,ctGrayImg;
    Mat ctImg=imread(ctImgName.toLatin1().data());
    cvtColor(ctImg,ctRgbImg,COLOR_BGR2RGB);
    cvtColor(ctRgbImg,ctGrayImg,COLOR_RGB2GRAY);
    myCtImg=ctRgbImg;
    myCtGrayImg=ctGrayImg;
    myCtQImage=QImage((const unsigned char*)(ctRgbImg.data),ctRgbImg.cols,ctRgbImg.rows,QImage::Format_RGB888);
    ctImgShow();

}

void MainWindow::ctImgProc()
{
    QTime time;
    time.start();
    ui->progressBar->setValue(19);
    while(time.elapsed()<2000) QCoreApplication::processEvents();
    ctImgHoughCircles();
    while (time.elapsed()<2000) QCoreApplication::processEvents();
    ui->progressBar->setValue(ui->progressBar->value()+20);
    ctImgShow();
    while (time.elapsed()<2000) QCoreApplication::processEvents();
    ui->progressBar->setValue(ui->progressBar->maximum());
    QMessageBox::information(this,u8"完毕",u8"子宫内壁见椭球形阴影，疑似子宫肌瘤");
}

void MainWindow::ctImgSave()
{
    QFile imgae("Tumor_proced.jpg");
    if(!imgae.open(QIODevice::ReadWrite)) return;
    QByteArray qba;
    QBuffer buf(&qba);
    buf.open(QIODevice::WriteOnly);
    myCtQImage.save(&buf,"JPG");
    imgae.write(qba);
}

void MainWindow::ctImgHoughCircles()
{
    Mat ctGrayImg=myCtGrayImg.clone();
    Mat ctColorImg;
    cvtColor(ctGrayImg,ctColorImg,COLOR_GRAY2BGR);
    GaussianBlur(ctGrayImg,ctGrayImg,Size(9,9),2,2);
    vector<Vec3f> h_circles;
    HoughCircles(ctGrayImg,h_circles,HOUGH_GRADIENT,2,ctGrayImg.rows/8,200,100);
    int processValue=45;
    ui->progressBar->setValue(processValue);
    QTime time;
    time.start();
    while(time.elapsed()<2000) QCoreApplication::processEvents();
    for(size_t i=0;i<h_circles.size();i++)
    {
        Point center(cvRound(h_circles[i][0]),cvRound(h_circles[i][1]));
        int h_radius=cvRound(h_circles[i][2]);
        circle(ctColorImg,center,h_radius,Scalar(238,0,238),3,8,0);
        circle(ctColorImg,center,3,Scalar(238,0,0),-1,8,0);
        processValue+=1;
        ui->progressBar->setValue(processValue);
    }
    myCtImg=ctColorImg;
    myCtQImage=QImage((const unsigned char*)(myCtImg.data),myCtImg.cols,myCtImg.rows,QImage::Format_RGB888);
}

void MainWindow::showUserPhoto()
{
    QPixmap photo;
    QModelIndex index;
    for(int i=0;i<model_d->rowCount();i++)
    {
        index=model_d->index(i,0);
        QString current_name=model_d->data(index).toString();
        if(current_name.compare(ui->nameLabel->text())==0){
            index=model_d->index(i,2);
            break;
        }
    }
    //photo.loadFromData(model_d->data(index).toByteArray(),"JPG");
    QByteArray base64ImageData = model_d->data(index).toByteArray();
    QByteArray imageData = QByteArray::fromBase64(base64ImageData);
    photo.loadFromData(imageData,"JPG");
    ui->photoLabel->setPixmap(photo);
}

void MainWindow::onTimeOut()
{
    QTime time=QTime::currentTime();
    ui->timeEdit->setTime(time);
}


void MainWindow::on_startPushButton_clicked()
{
    ctImgRead();
    QTime time;
    time.start();
    ui->progressBar->setMaximum(0);
    ui->progressBar->setMaximum(0);
    while (time.elapsed()<5000) {
        QCoreApplication::processEvents();
    }
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ctImgProc();
    ui->progressBar->setValue(0);
    ctImgSave();
}

void MainWindow::on_basicTableView_clicked(const QModelIndex &index)
{
    onTableSelectChange(1);//?
}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index==1)
    {
        QModelIndex index;
        for(int i=0;i<model_d->rowCount();i++)
        {
            index=model_d->index(i,0);
            QString current_name=model_d->data(index).toString();
            if(current_name.compare(ui->nameLabel->text())==0)
            {
                index=model_d->index(i,1);
                break;
            }
        }
        ui->caseTextEdit->setText(model_d->data(index).toString());
        ui->caseTextEdit->setFont(QFont("楷体",12));//为什么要特别设置成楷体？？
    }
}
