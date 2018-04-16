#ifndef NZEROINTERFACE_H
#define NZEROINTERFACE_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QWebSocket>
#include <QJsonObject>
#include <QFile>

#if defined(NZEROINTERFACE_LIBRARY)
#  define SHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHARED_EXPORT Q_DECL_IMPORT
#endif

class SHARED_EXPORT NZeroInterface : public QObject
{
    Q_OBJECT
public:
    explicit NZeroInterface(QObject *parent = nullptr);
    ~NZeroInterface();

    void GenerateKeyPair();
    void SaveKeyPair();
    void LoadKeyPair();
    bool HasKeyPairFile();
    QString GetPubKey();
    void InitKeyPair();

    void Connect(QString ip = "118.178.127.35",QString contract = "click",int readP = 3001, int writeP = 3002, int wsP = 3003);
    void Close();

    void Send(QString func,QString argv = "null");

signals:
    void OnMessage(QString msg);

private slots:
    void OnConnect();
    void OnWS(QString msg);
    void finishedSlot(QNetworkReply* reply);

private:
    QString ip = "118.178.127.35";
    int readPort = 3001;
    int writePort = 3002;
    int wsReadPort = 3003;
    QString contract = "click";
    void* emcc;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QWebSocket ws;
};

#endif // NZEROINTERFACE_H
