#ifndef _SYRAH_SECURITY_H
#define _SYRAH_SECURITY_H

//Qt Library
#include <QString>
//C++ Standard Library
#include <string>
#include <map>
//Syrah Library
#include "_syrah_kernel.h"

//Security Defence
#define SYRAH_MAXLENGTH_USERNAME 50
#define SYRAH_MAXLENGTH_PASSWORD 50
#define SYRAH_MAXLENGTH_CXSTRING 250

#define SYRAH_MAXLENGTH_SQLQUERY 75

#define SYRAH_VERIFY_DATABASE "SQLite format 3"
#define SYRAH_NEW_1 "DROP TABLE E;"
#define SYRAH_NEW_2 "CREATE TABLE mailbox(id int, mailbox varchar(250), boxUser varchar(250), boxAccess varchar(250), pop3server varchar(250), pop3port int, imapserver varchar(250), imapport int, smtpserver varchar(250), smtpport int, ssl int, defsend int, defrecieve int, sign varchar(250), primary key(id));"
#define SYRAH_NEW_3 "CREATE TABLE folder(id int, boxid int, folder varchar(250), security int, code varchar(250), primary key(id));"
#define SYRAH_NEW_4 "CREATE TABLE mail(id int, folderid int, uidl varchar(250), subject varchar(250), mailFrom varchar(250), mailTo varchar(250), cc varchar(250), bcc varchar(250), messageId varchar(500), inReplyTo varchar(500), contentType varchar(250), precedence varchar(250), mailReferences varchar(500), replyTo varchar(250), sender varchar(250), archivedAt varchar(250), content varchar(25000), attachment varchar(250), status int, inStackDt varchar(250), mailDt varchar(250), primary key(id));"
#define SYRAH_NEW_5 "CREATE TABLE contacts(id int, boxid int, name varchar(250), email varchar(250), primary key(id));"
#define SYRAH_NEW_6 "CREATE TABLE rules(id int, contains varchar(250), field int, toBoxid int, toFolderid int, primary key(id));"
#define SYRAH_NEW_7 "CREATE TABLE setting(id int, key varchar(250), value varchar(250), primary key(id));"
#define SYRAH_ID_ERROR 0xFFFFFFFF

extern _syrah_kernel _syrah_sys;

using namespace std;

class _syrah_security {
private:
    unsigned long len;
    bool status_SQLDefenceInit;
    QString SQLDefenceTable[29];

    void Init_SQLDefenceTable();
public:
    _syrah_security();
    _syrah_security(unsigned long l);

    void resetLen(unsigned long l);
    const string& cxstr_LEN(string& str);
    const string vcxstr_LEN(char* str);
    //const QString& qstr_LEN(const QString& str);

    bool verifyDb(string magic);
    const QString SQLQueryDefence(QString query);
};

#endif // _SYRAH_SECURITY_H
