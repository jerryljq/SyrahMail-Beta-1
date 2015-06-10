#ifndef _SYRAH_USER_H
#define _SYRAH_USER_H

//Qt Library
#include <QString>
#include <QVariant>
#include <QSqlQuery>
//C++ Standard Library
#include <vector>
#include <map>
//Syrah Library
#include "_syrah_kernel.h"
#include "_syrah_security.h"
#include "_syrah_database.h"
#include "mailbox/_syrah_mailbox.h"
#include "mailbox/_syrah_rules.h"
#include "mailbox/_syrah_contacts.h"

using namespace std;

class _syrah_user
{
private:

    map<unsigned long int, _syrah_mailbox*> userMailbox;
    map<unsigned long int, _syrah_rules*> userRule;
    map<unsigned long int, _syrah_contacts*> userContacts;
    map<QString, QVariant> userSetting;

    //unsigned long int defaultSend;

public:
    _syrah_database userDb;

    _syrah_user(const QString& username, const QString& password);

    map<unsigned long int, _syrah_mailbox*>& uMailbox();
    map<unsigned long int, _syrah_rules*>& uRule();
    map<unsigned long int, _syrah_contacts*>& uContacts();
    map<QString, QVariant>& uSetting();
  //  unsigned long int getId(const QString table);

    //void Query();
};

#endif // _SYRAH_USER_H
