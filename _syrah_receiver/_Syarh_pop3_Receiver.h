#ifndef _Syrah_pop3_Receiver_H

#define _Syrah_pop3_Receiver_H

#include <QObject>
#include <QString>
#include <QtNetwork/qtcpsocket.h>
#include <QTimer>
#include "_syrah_mail_received.h"
#include "_syrah_mime_decoder.h"
#include "_syrah_dbset/mailbox/_syrah_mailbox.h"
#include "_syrah_dbset/_syrah_kernel.h"
#include "_syrah_dbset/_syrah_user.h"
#include <map>
#include <vector>
#include <QFile>

extern _syrah_kernel _syrah_sys;
extern vector <_syrah_user*> _syrah_userTable;


class _Syrah_pop3_Receiver : public QObject
{
    Q_OBJECT
public:
    explicit _Syrah_pop3_Receiver(QObject *parent = 0,QString userName="",QString Password="",QString Mail_Server="");

   // _Syrah_pop3_Receiver(QObject *parent  ,QString userName,QString Password,QString Mail_Server, int Server_Port);

    ~_Syrah_pop3_Receiver();

    _syrah_mail_received getMails();
    int getMailsNum();

signals:
    void NewMailArrived();

private slots:
    void connectedEvent();          //when connected, set state to Connected
    void stateChanged();
    void readMessege();
    void timerOut();

private:
    QTcpSocket *socket;
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

    static const int port=110;

    enum Pop3ConnectionState
    {
        NotConnected,
        Connected,
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

#endif // _Syrah_pop3_Receiver_H
