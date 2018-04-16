#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    zero.InitKeyPair();
    zero.Connect("118.178.127.35","zc");
    QObject::connect(&zero,&NZeroInterface::OnMessage,this,&MainWindow::OnZeroRcv);
    //zero.GetBalanceOf();
    zero.Send("transfer",zero.GetPubKey()+"?0");
    ui->lineEditMyPubKey->setText(zero.GetPubKey());
    ui->lineEdit_Taget->setText(zero.GetPubKey());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSend_clicked()
{
    zero.Send("transfer",ui->lineEdit_Taget->text()+"?1");
}

void MainWindow::OnZeroRcv(QString msg)
{
    QJsonDocument jdom = QJsonDocument::fromJson(msg.toLatin1());
    auto obj = jdom.object();
    auto reciver = obj["reciver"].toString();
    auto rBalance = obj["reciverBalance"].toDouble();
    if(reciver == zero.GetPubKey()){
        ui->lcdNumber->display(rBalance);
    }
}
