

8:31:52
陈世华 2018/4/15 8:31:52
    static QString getTaget(QString type,QString pubkey,QString name,QString code,QString arg,QString sig) {
        QString data = type + "$" + pubkey + "$" + name + "$" + code + "$" + arg;
        QString block = sig + "&" + data;
        return block;
    }

    static QString docmd(QString type,QString pubkey,QString prikey,QString name,QString func,QString arg) {
        QString msg = type + "$" + pubkey + "$" + name + "$" + func + "$" + arg;
        QByteArray hash = GETSHA256(msg.toLatin1());
        sign(prikey.toLatin1().data(),hash.data());
        QByteArray sig = getSign();
        QString block = getTaget(type, pubkey, name, func, arg, sig);
        return block;
    }

    static QJsonObject doMethodGet(Password &psd,QString url,QString pContract,QString pMethod,QStringList pArg){
        QString arg;
        for(auto cur:pArg)
            arg.append(cur).append("?");
        arg.remove(arg.count()-1,1);
        QString block = pContract+"$"+pMethod+"$"+arg+"$"+psd.pubkey;
        QByteArray result = get(url+"/"+block);
        QJsonDocument jsonDoc = QJsonDocument::fromJson(result);
        QJsonObject   jsonObj = jsonDoc.object();
        BUG << jsonObj;
        return jsonObj;
    }

    static QByteArray doMethodSet(Password &psd,QString url,QString pContract,QString pMethod,QStringList pArg){
        QString arg;
        for(auto cur:pArg)
            arg.append(cur).append("?");
        arg.remove(arg.count()-1,1);
        QString block = docmd("method",psd.pubkey,psd.prikey,pContract,pMethod,arg);
        return get(url+"/"+block);
    }
9:27:16
陈世华 2018/4/15 9:27:16
transfer
9:43:04
陈世华 2018/4/15 9:43:04
-platform webgl:port=800
10:34:00
陈世华 2018/4/15 10:34:00
getBalanceOf

陈世华 2018/4/15 10:34:07
transfer
10:36:23
陈世华 2018/4/15 10:36:23
https://github.com/
10:55:01
陈世华 2018/4/15 10:55:01
#ifndef PASSWORD_H
#define PASSWORD_H

#include "Define.h"
#include "emcc/uECC.h"

class Password {
public:
    Password(){
        passwdFile = "config.ini";
        setting = new QSettings(passwdFile,QSettings::IniFormat);
        if(hasAppkey()){
            appkey = setting->value("appkey").toString().toLatin1();
        }
        if(hasPubkey()){
            pubkey = setting->value("pubkey").toString().toLatin1();
            prikey = setting->value("prikey").toString().toLatin1();
        }else{
            makeKey();
        }
    }
    ~Password(){}

    bool hasAppkey(){
        if (!setting->contains("appkey")){
            return false;
        }
        return true;
    }

    void updAppkey(QByteArray pKey){
        appkey = pKey;
        setting->setValue("appkey",appkey);
    }

    bool hasPubkey(){
        if (!setting->contains("pubkey")){
            return false;
        }
        return true;
    }

    void makeKey(){
        getkey();
        pubkey = getPublicKey();
        prikey = getPrivateKey();
        prikey = prikey.left(64);   //need use AES

        setting->setValue("pubkey",pubkey);
        setting->setValue("prikey",prikey);
    }

    QString passwdFile;
    QByteArray pubkey;
    QByteArray prikey;
    QByteArray appkey;
    QByteArray signmsg;
    QSettings *setting;
};


#endif // PASSWORD_H
11:18:35
陈世华 2018/4/15 11:18:35
zeroclick
11:22:25
陈世华 2018/4/15 11:22:25
    QJsonDocument jsonDoc = QJsonDocument::fromJson(pMsg.toLatin1());
    QJsonObject   jsonObj = jsonDoc.object();
11:37:16
陈世华 2018/4/15 11:37:16
BFAFD50EFC6EC63A0AA1C762ECF4A7FA9F8F0603F56B0A7C7F26B9D4B189E8B67298EFFD31AC5B7CCB7F995D9EBB14F0B00B3F20DE46030616F6BD56B1859A11

┣Nix┫ 2018/4/15 11:37:21
03C4EE03EA635EC29417E1F8C5F4E98FD9399FEEBF5B4F10555115ED4475A826387D4D5B08AC702EB082DA7FEFCA56791929E85B441AD04E7F40506CBB27BF8D
11:41:29
陈世华 2018/4/15 11:41:29
#define GETMD5(A) QCryptographicHash::hash(A,QCryptographicHash::Md5).toHex()
#define GETADDR(A) QByteArray("0x")+QCryptographicHash::hash(A,QCryptographicHash::Keccak_256).toHex().left(40)
#define GETSHA256(A) QCryptographicHash::hash(A,QCryptographicHash::Sha256).toHex()

陈世华 2018/4/15 11:42:26
#ifndef DEFINE_H
#define DEFINE_H

#include <QFile>
#include <QDebug>
#include <QMutex>
#include <QSettings>
#include <QtConcurrent>
#include <QThreadPool>
#include <QTcpServer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>

//Public Key new Edition
#define BUG qDebug()<<__FUNCTION__<<__LINE__
#define SETXF(A,B) QString("%1").arg(A, 0, 'f', B)

#define GETMD5(A) QCryptographicHash::hash(A,QCryptographicHash::Md5).toHex()
#define GETADDR(A) QByteArray("0x")+QCryptographicHash::hash(A,QCryptographicHash::Keccak_256).toHex().left(40)
#define GETSHA256(A) QCryptographicHash::hash(A,QCryptographicHash::Sha256).toHex()

#endif // DEFINE_H
