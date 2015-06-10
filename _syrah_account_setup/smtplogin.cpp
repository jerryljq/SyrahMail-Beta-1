#include "smtplogin.h"
#include <QMessageBox>

smtplogin::smtplogin()
{
    Socket = new QTcpSocket;
    port = 25;
    LoginState = false;
    IsFinished = false;
}

void smtplogin::connectEvent()
{
    state = Connected;
}

void smtplogin::reconnect()
{
    Socket->connectToHost(MailServer, port);
}


void smtplogin::readMessage()
{
    if(state == Connected)
    {
        QString Response = Socket->readLine();
        codeRecv = Response.left(3).toInt();

        qDebug() << Response;
        qDebug() << "000"<<codeRecv;

        if(codeRecv == 220)
        {
            qDebug() << "Connected";
            state = Hello;

            QByteArray MessageToSend;
            MessageToSend.append("HELO ").append("SyrahMailApp").append("\r\n");
            Socket->write(MessageToSend);

            return;
        }
    }

    if(state == Hello)
    {
        QString Response = Socket->readLine();
        codeRecv = Response.left(3).toInt();
        qDebug() << "000"<<codeRecv;
        if(codeRecv == 250)
        {
            qDebug() << "HandShake Succeeded ";
            state = LoginRequired;

            QByteArray MessageToSend;
            MessageToSend.append("AUTH LOGIN").append("\r\n");
            Socket->write(MessageToSend);

            return;
        }
    }
    if(state == LoginRequired)
    {
        QString Response = Socket->readLine();
        codeRecv = Response.left(3).toInt();
        qDebug() << "000"<<codeRecv;
        if(codeRecv == 334)
        {
            qDebug() << "User Name Required ";
            state = UserNameRequired;

            QByteArray MessageToSend;
            MessageToSend.append(MailAcc.trimmed()).toBase64();
            qDebug() << MessageToSend;
           // Socket->write(QByteArray().append(MailAcc).toBase64().append("\r\n"));
            Socket->write(MessageToSend.toBase64().append("\r\n"));
            return;
        }
    }
    if(state == UserNameRequired)
    {
        QString Response = Socket->readLine();
        codeRecv = Response.left(3).toInt();
        qDebug() << codeRecv;
        qDebug() << Response;
        if(codeRecv == 334)
        {
            qDebug() << "UserName Accepted";
            state = PassRequired;

            QByteArray MessageToSend;
            MessageToSend.append(MailAccPassWd.trimmed()).toBase64();
            Socket->write(MessageToSend.toBase64().append("\r\n"));

            return;
        }
        else
        {
            qDebug() << Socket->readAll();
        }
    }
    if(state == PassRequired)
    {
        QString Response = Socket->readLine();
        codeRecv = Response.left(3).toInt();
        qDebug() << "000"<<codeRecv;
        if(codeRecv == 235)
        {
            qDebug() << "Logon Succeed";
            state = DisConnected;
            LoginState = true;
            //emit readMessagesig(LoginState);
            //return;
        }
        IsFinished = true;
    }
    if(state == WaitForResponse)
    {
        qDebug() << Socket->readAll();
    }

    if(state == DisConnected)
    {
        IsFinished = true;
        Socket->disconnect();
        qDebug() << "Disconnected";
    }
}
bool smtplogin::AccLoginState()
{
    return LoginState;
}

void smtplogin::StateChanged()
{
    qDebug() << "State changed" <<Socket->state();
    qDebug() << Socket->readAll();
}

void smtplogin::setArgv(QString MailAccount, QString MailAccPassWord, QString serverhost)
{
    MailAcc = MailAccount;
    MailAccPassWd = MailAccPassWord;
    MailServer = serverhost;
}

bool smtplogin::isFinish()
{
    return IsFinished;
}

bool smtplogin::loginevent(QString MailAccount, QString MailAccPassWord, QString serverhost, int serverport)
{
    Socket = new QTcpSocket;
    port = serverport;
    LoginState = false;
    IsFinished = false;
    MailAcc = MailAccount;
    MailAccPassWd = MailAccPassWord;
    MailServer = serverhost;
    connect(Socket, SIGNAL(connected()), this, SLOT(connectEvent()));
    connect(Socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(StateChanged()));
    connect(Socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    Socket->connectToHost(MailServer, port);
    Socket->blockSignals(false);
  //  QMessageBox::Information(this, "Validating","Verifying your account");
    Socket->waitForDisconnected(2000);
    qDebug () <<"Check here";
    if(LoginState)
    {
        return true;
    }
    else
        return false;
}

