#ifndef _SYRAH__Syrah_pop3_Ssl_Receiver_SSL_RECEIVER_H
#define _SYRAH__Syrah_pop3_Ssl_Receiver_SSL_RECEIVER_H

#include <QObject>
#include <QString>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qsslsocket.h>
#include <QTimer>
#include <QFile>
#include "_syrah_mail_received.h"
#include "_syrah_mime_decoder.h"
#include "_syrah_dbset/_syrah_kernel.h"
#include "_syrah_dbset/_syrah_user.h"

extern vector <_syrah_user*> _syrah_userTable;
extern _syrah_kernel _syrah_sys;

class _Syrah_pop3_Ssl_Receiver : public QObject
{
    Q_OBJECT
public:
    explicit _Syrah_pop3_Ssl_Receiver(QObject *parent = 0, int MailBoxIDD = -1);
    virtual ~_Syrah_pop3_Ssl_Receiver();

    _syrah_mail_received getMails();
    int getMailsNum();

signals:
    void NewMailArrived();

public slots:
    void encryptedEvent();          //when connected, set state to Connected
    void stateChanged();
    void readMessege();
    void timerOut();

private:
    QSslSocket *socket;
    QTimer *timer;
    _syrah_mail_received mail;
    _syrah_Mime_Decoder decoder;

    QString server;
    QString username;
    QString passwd;
    QStringList UIDLlist;
    QStringList mailContent;
    int mailNumtobeReceived;
    int mailIndex;
    int state;
    int BytestoRead;

    static const int port=995;

    int MailBoxID;


    enum progressingState{
        NotConnected,
        Connected,
        Encrypted,
        WaitforResponseforUserName,
        WaitforResponseforPassWord,
        WaitforStat,
        WaitforUIDL,
        WaitforBytestoRead,
        ReadContent,
        WaitforDele,
        ReceiveOver
    };

};

#endif // _SARAH__Syrah_pop3_Ssl_Receiver_SSL_RECEIVER_H
