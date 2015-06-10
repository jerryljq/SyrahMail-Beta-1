#include "_Syrah_pop3_Ssl_Receiver.h"

_Syrah_pop3_Ssl_Receiver::_Syrah_pop3_Ssl_Receiver(QObject *parent, int MailBoxIDD) :
    QObject(parent)
{
    socket=new QSslSocket;
    timer=new QTimer;

    /*server=Mail_Server;
    username=MailBoxID;
    passwd=Password;*/
    state=NotConnected;
    //port=Server_Port;

    MailBoxID = MailBoxIDD;

    QList<QSslError> expectedSslErrors;
    expectedSslErrors.append(QSslError::SelfSignedCertificate);
    expectedSslErrors.append(QSslError::CertificateUntrusted);

    connect(socket,SIGNAL(encrypted()),this,SLOT(encryptedEvent()));
    connect(socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readMessege()));
    connect(timer,SIGNAL(timeout()),this,SLOT(timerOut()));

    timer->start();
    socket->blockSignals(false);
    socket->ignoreSslErrors(expectedSslErrors);
}

void _Syrah_pop3_Ssl_Receiver::timerOut()
{
    timer->stop();

    vector<_syrah_user*>& uTable = _syrah_userTable;

    socket->connectToHostEncrypted(uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[MailBoxID]->getRecvServer(), port);

    mailIndex=0;
    mailNumtobeReceived=0;
    BytestoRead=0;
    UIDLlist.clear();
    mailContent.clear();
}

void _Syrah_pop3_Ssl_Receiver::encryptedEvent()
{
    state=Encrypted;
}


void _Syrah_pop3_Ssl_Receiver::readMessege()
{
    qDebug()<<state;
    if(state==Encrypted)
    {

        QString response=socket->readAll();
        qDebug()<<response.trimmed();
        if(response.contains("+OK"))
        {
            qDebug()<<"connect successfully";
            state=WaitforResponseforUserName;
            vector<_syrah_user*>& uTable = _syrah_userTable;

            QByteArray messegetoServer;
            messegetoServer.append("user ").append(uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[MailBoxID]->getUser()).append("\r\n");
            //qDebug()<<messegetoServer;
            socket->write(messegetoServer);
            return;
        }

    }

    if(state==WaitforResponseforUserName)   //2
    {
        QString response=socket->readAll();
        qDebug()<<response.trimmed();
        if(response.contains("+OK"))
        {
            qDebug()<<"username input successfully";
            QByteArray messegetoServer;
            vector<_syrah_user*>& uTable = _syrah_userTable;

            messegetoServer.append("pass ").append(uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[MailBoxID]->getPass()).append("\r\n");
            socket->write(messegetoServer);
            //qDebug()<<messegetoServer;
            state=WaitforResponseforPassWord;
            return;
        }
    }

    if(state==WaitforResponseforPassWord)   //3
    {
        QString response=socket->readAll();
        qDebug()<<response.trimmed();
        if(response.contains("+OK"))
        {
            state=WaitforStat;
            QByteArray messegetoServer;
            messegetoServer.append("stat ").append("\r\n");
            socket->write(messegetoServer);
            return;
        }
        else
            qDebug()<<"passwd failed";
    }


    if(state==WaitforStat)  //4
    {
        QString response=socket->readAll();
        qDebug()<<response.trimmed();
        if(response.contains("+OK"))
        {
            state=WaitforUIDL;              //update state

            QString num=response.split(" ").at(1);
            mailNumtobeReceived=num.toInt();
            if(mailNumtobeReceived==0)
            {
                state=ReceiveOver;
                socket->write(QByteArray().append("quit ").append("\r\n"));
                return;
            }
            qDebug()<<"the number of mails = "<<mailNumtobeReceived;
            QByteArray messegetoServer;
            messegetoServer.append("UIDL").append("\r\n");
            socket->write(messegetoServer);
            return;
        }
    }


    if(state==WaitforUIDL)  //5
    {
        QString response=socket->readLine();
        if(response.contains("+OK"))
        {
            while(socket->bytesAvailable()>0)
            {
                response=socket->readLine();
                UIDLlist<<response.trimmed();
            }
            if(UIDLlist.size()>0)
                UIDLlist.removeLast();              //remove the last line(. )

            for(int i=0;i<mailNumtobeReceived;i++)
            {
                qDebug()<<UIDLlist.at(i);
            }

            QByteArray messegetoServer;
            messegetoServer.append("list ").append(QVariant(++mailIndex).toByteArray()).append("\r\n");
            qDebug()<<messegetoServer;
            socket->write(messegetoServer);
            state=WaitforBytestoRead;
            return;
        }
    }

    if(state==WaitforBytestoRead)   //6
    {
        QString response=socket->readAll();
        qDebug()<<response;
        if(response.contains("+OK"))
        {
            BytestoRead=QVariant(response.split(" ").at(2)).toInt();
            qDebug()<<"Bytes to read: "<<BytestoRead;

            state=ReadContent;
            QByteArray messegetoServer;
            messegetoServer.append("retr ").append(QVariant(mailIndex).toByteArray()).append("\r\n");
            qDebug()<<messegetoServer;
            socket->write(messegetoServer);
            return;
        }
    }


    if(state==ReadContent)               //7
    {
        if(socket->bytesAvailable()<BytestoRead)
        {
            qDebug()<<socket->bytesAvailable();
            qDebug()<<11111111111111;
            return;
        };

        qDebug()<<socket->bytesAvailable();
        qDebug()<<2222222222222222222;
        QString response=socket->readAll();
        qDebug()<<"shit"<<response;

        mailContent<<response;
        if(response.contains("+OK"))
        {
            state=WaitforDele;

            QByteArray messegetoServer;
            messegetoServer.append("dele ").append(QVariant(mailIndex).toByteArray()).append("\r\n");
            socket->write(messegetoServer);

            return;
        }
    }




    if(state==WaitforDele)              //8
    {
        QString response=socket->readAll();
        qDebug()<<"response to dele:  "<<response;
        qDebug()<<"del succefully";
        if(response.contains("+OK"))
        {
            UIDLlist.removeFirst();
            if(mailIndex==mailNumtobeReceived)
            {
                state=ReceiveOver;
                socket->write(QByteArray().append("quit ").append("\r\n"));
                return;
            }
            else
            {
                state=WaitforBytestoRead;
                QByteArray messegetoServer;
                messegetoServer.append("list ").append(QVariant(++mailIndex).toByteArray()).append("\r\n");
                socket->write(messegetoServer);
                return;
            }
        }

    }

    if(state==ReceiveOver)
    {
        QString response=socket->readAll();
        qDebug()<<response;
        if(response.contains("+OK"))
        {
            state=NotConnected;
            qDebug()<<"quit succefully";
            if(mailNumtobeReceived>0)
            {

                for(int i=0;i<mailContent.size();i++)
                {
                    decoder.Decord(mailContent.at(i));
                    mail=decoder.getMailDecoded();
                    //mail.uidl=UIDLlist.at(i);


                    qDebug()<<mail.AttachMentContent.size();
                    for(int i=0;i<mail.AttachMentContent.size();i++)
                    {
                        qDebug()<<"zhe beng le";
                        QFile attachment(mail.AttachMentName.at(i));
                        if(attachment.open(QIODevice::WriteOnly))
                        {
                            QDataStream out(&attachment);
                            qDebug()<<"attachmentcontent size"<<mail.AttachMentContent.at(i).size();
                            out<<mail.AttachMentContent.at(i);
                        }
                    }

                    //dealWith(mail);
                    vector<_syrah_user*>& uTable = _syrah_userTable;
                    extern _syrah_kernel _syrah_sys;
                    map<unsigned long, _syrah_mail*>& UM = uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->uFolder()[0]->userMail;
                    _syrah_mail* uM;


                    //uM = mail.toMail();
                    uM = new _syrah_mail;
                    uM->Id = _syrah_sys.getUniId(2);
                    uM->subject = mail.subject;
                    uM->from = mail.from;
                    uM->to = mail.to;
                    uM->mailTime = mail.mailTime;
                    uM->contentType = "text/html";
                    uM->content = QVariant(mail.content.join("")).toByteArray().toBase64();
                    uM->uidl = mail.uidl;

                    uM->attatchment = mail.AttachMentName.join("_this_is_the_fucking_boundary__");

                    //Insert
                    UM.insert(pair<unsigned long int, _syrah_mail*>(uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->uFolder()[0]->userMail.size(), uM));
                }
                emit NewMailArrived();
            }
            timer->start(5000);
        }
    }

}

void _Syrah_pop3_Ssl_Receiver::stateChanged()
{
    qDebug()<<"state changed"<<socket->state();
}

_syrah_mail_received _Syrah_pop3_Ssl_Receiver::getMails()
{
    return mail;
}

int _Syrah_pop3_Ssl_Receiver::getMailsNum()
{
    return mailNumtobeReceived;
}

_Syrah_pop3_Ssl_Receiver::~_Syrah_pop3_Ssl_Receiver()
{
    socket->close();
}
