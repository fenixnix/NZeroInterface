#include "nzerointerface.h"
#include "nemcc.h"

NZeroInterface::NZeroInterface(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
    emcc = new NEmcc();
}

NZeroInterface::~NZeroInterface()
{
    delete emcc;
}

void NZeroInterface::GenerateKeyPair()
{
    ((NEmcc*)emcc)->GenerateKeyPair();
}

QString configFileName = "nzeroConfig";

void NZeroInterface::SaveKeyPair()
{
    QSettings settings(configFileName,QSettings::IniFormat);
    settings.setValue("PrivateKey",((NEmcc*)emcc)->privateKey);
    settings.setValue("PublicKey",((NEmcc*)emcc)->publicKey);
}

void NZeroInterface::LoadKeyPair()
{
    QString fileName = "nzeroConfig";
    QSettings settings(fileName,QSettings::IniFormat);
    ((NEmcc*)emcc)->privateKey = settings.value("PrivateKey").toString();
    ((NEmcc*)emcc)->publicKey = settings.value("PublicKey").toString();
}

bool NZeroInterface::HasKeyPairFile()
{
    QFile file(configFileName);
    return file.exists();
}

QString NZeroInterface::GetPubKey()
{
    return ((NEmcc*)emcc)->publicKey;
}

void NZeroInterface::InitKeyPair()
{
    if(HasKeyPairFile()){
        LoadKeyPair();
    }else{
        GenerateKeyPair();
        SaveKeyPair();
    }
}

void NZeroInterface::Connect(QString ip, QString contract, int readP, int writeP, int wsP)
{
    this->ip = ip;
    this->contract = contract;
    writePort = writeP;
    readPort = readP;
    wsReadPort = wsP;

    auto qUrl = QUrl("ws://"+ip+":"+QString::number(wsReadPort));
    qDebug()<<"Connect To:"<<qUrl.url();
    ws.open(qUrl);
    QObject::connect(&ws, &QWebSocket::connected, this, &NZeroInterface::OnConnect);
}

void NZeroInterface::Close()
{
    ws.close();
}

void NZeroInterface::OnConnect()
{
    qDebug()<<"WS Connected!";
    QObject::connect(&ws, &QWebSocket::textMessageReceived,
                     this, &NZeroInterface::OnWS);
}

void NZeroInterface::OnWS(QString msg)
{
    qDebug()<<"Rcv WS Msg:"<<msg;
    emit OnMessage(msg);
}

void NZeroInterface::Send(QString func, QString argv)
{
    QString sendData;
    QString pubKey = ((NEmcc*)emcc)->publicKey;
    QString url = "http://"+ip+":";
    if(func.startsWith("get")){
        sendData = url + QString::number(readPort) +"/"+ contract +"$"+func+"$"+ argv + "$"+pubKey;
    }else{
        QString rawData = "method$" + pubKey +"$"+ contract + "$" +func +"$"+ argv ;
        QString sign = ((NEmcc*)emcc)->Sign(rawData);
        sendData = url + QString::number(writePort) +"/"+ sign +"&" +rawData;
    }

    qDebug()<<"Send:"<<sendData;
    request.setUrl(QUrl(sendData));
//    QByteArray postData;
//    QNetworkReply* reply = manager->post(*request,postData);
    manager->get(request);
}

void NZeroInterface::finishedSlot(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        qDebug()<<bytes;
        QString string = QString::fromUtf8(bytes);
        qDebug()<<string;
    }
    else
    {
        qDebug()<<"handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        qDebug(qPrintable(reply->errorString()));
    }
    reply->deleteLater();
}
