#ifndef _SYRAH_MAILBOX_H
#define _SYRAH_MAILBOX_H

//Qt Library
#include <QString>
//C++ Standard Library
#include <vector>
#include <map>
//Syrah Library
#include "_syrah_dbset/_syrah_kernel.h"
#include "_syrah_dbset/mailbox/_syrah_folder.h"
#include "_syrah_dbset/mailbox/_syrah_contacts.h"

using namespace std;

class _syrah_mailbox
{
private:
    QString mailboxName;
    QString pop3Server; unsigned int pop3Port;
    QString imapServer; unsigned int imapPort;
    QString smtpServer; unsigned int smtpPort;
    QString boxUser;
    QString boxAccess;
    bool ssl;

    int sendDefault;
    int recieveDefault;
    QString userSign;

public:
    unsigned long int Id;
    map<unsigned long int, _syrah_folder*> userFolder;
    map<unsigned long int, _syrah_contacts*> userContacts;
    vector<QString> mailStack;

    _syrah_mailbox(unsigned long int iD, const QString& boxName, const QString pop3s, unsigned int pop3p, QString smtps, unsigned int smtpp,
                   QString boxU, QString boxA, bool ss, int sent, int recieve, map<unsigned long int, _syrah_folder*> uF,
                   map<unsigned long int, _syrah_contacts*> uC, vector<QString> mailS, QString uSign);

    _syrah_mailbox(unsigned long int iD, const QString& boxName, const QString pop3s, unsigned int pop3p, QString smtps, unsigned int smtpp,
                                   QString boxU, QString boxA, bool ss);

    const unsigned long int& getId();
    QString getName();

    const QString& getUser() { return boxUser; }
    const QString& getPass() { return boxAccess; }
    const QString& getSmtpServer() { return smtpServer; }
    const int& getSendPort() { return smtpPort; }
    const QString& getRecvServer() { return pop3Server; }
    const int& getRecvPort() { return pop3Port; }

    map<unsigned long int, _syrah_folder*>& uFolder();
    map<unsigned long int, _syrah_contacts*>& uContacts();

    const QString& uSign();
    QString SqlWriteMailbox();
};

#endif // _SYRAH_MAILBOX_H
