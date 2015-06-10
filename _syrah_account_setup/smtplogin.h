#ifndef SMTPLOGIN_H
#define SMTPLOGIN_H

#include <QObject>
#include <QString>
#include <QtNetwork/QTcpSocket>

class smtplogin : public QObject
{
    Q_OBJECT
public:
   // explicit smtplogin(QObject *parent = 0);
    smtplogin();
    bool AccLoginState();
    void setArgv(QString MailAccount, QString MailAccPassWord, QString serverhost);
    bool isFinish();
    bool loginevent(QString MailAccount, QString MailAccPassWord, QString serverhost, int serverport);

public slots:
    void reconnect();
    void readMessage();
    void StateChanged();
    void connectEvent();

private:

    QTcpSocket *Socket;
    QString MailServer;
    QString MailAcc;
    QString MailAccPassWd;
    int port;
    int state;
    int codeRecv;
    bool IsFinished;
    bool LoginState;

    enum ConnectionState
    {
        NotConnected,
        Connected,
        UserNameRequired,
        PassRequired,
        Hello,
        LoginRequired,
        WaitForResponse,
        DisConnected
    };
signals:
    void readMessagesig(bool);
};

#endif // SMTPLOGIN_H
