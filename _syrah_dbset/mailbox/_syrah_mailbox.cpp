#include "_syrah_mailbox.h"

extern _syrah_kernel _syrah_sys;

_syrah_mailbox::_syrah_mailbox(unsigned long int iD, const QString& boxName, const QString pop3s, unsigned int pop3p, QString smtps, unsigned int smtpp,
                               QString boxU, QString boxA, bool ss, int sent, int recieve, map<unsigned long int, _syrah_folder*> uF,
                               map<unsigned long int, _syrah_contacts*> uC, vector<QString> mailS, QString uSign) {
    Id = iD;
    mailboxName = boxName;
    pop3Server = pop3s;
    pop3Port = pop3p;
    smtpServer = smtps;
    smtpPort = smtpp;
    boxUser = boxU;
    boxAccess = boxA;
    ssl = ss;
    sendDefault = sent;
    recieveDefault = recieve;
    userFolder = uF;    userContacts = uC;
    mailStack = mailS;
    userSign = "Sent from SyrahMail";
}


_syrah_mailbox::_syrah_mailbox(unsigned long int iD, const QString& boxName, const QString pop3s, unsigned int pop3p, QString smtps, unsigned int smtpp,
                               QString boxU, QString boxA, bool ss) {
    Id = iD;
    mailboxName = boxName;
    pop3Server = pop3s;
    pop3Port = pop3p;
    smtpServer = smtps;
    smtpPort = smtpp;
    boxUser = boxU;
    boxAccess = boxA;
    ssl = ss;
    sendDefault = 0;  //0 stands for SMTP
    recieveDefault = 0; //0 stands for POP3
   // userFolder.insert(pair<unsigned long int, _syrah_folder*>(0 ,new _syrah_folder(_syrah_sys.getUniId(1), "Inbox")));
    //qDebug() << _syrah_sys.getUniId(1);
    //userFolder.insert(pair<unsigned long int, _syrah_folder*>(1 ,new _syrah_folder(_syrah_sys.getUniId(1), "Sent")));

    //userContacts = 0;
    //mailStack = 0;
    userSign = "Sent from SyrahMail";
}


QString _syrah_mailbox::getName() { return mailboxName; }

map<unsigned long int, _syrah_folder*>& _syrah_mailbox::uFolder() { return userFolder; }
map<unsigned long int, _syrah_contacts*>& _syrah_mailbox::uContacts() { return userContacts; }

QString _syrah_mailbox::SqlWriteMailbox() {
  QString sql("");
  QVariant v;
  qlonglong q;

    sql.append("(");
    //Id
    sql.append("'"); q = Id; v = q; sql.append(v.toString()); sql.append("', ");
    //mailbox
    sql.append("'"); sql.append(mailboxName); sql.append("', ");
    //boxUser
    sql.append("'"); sql.append(boxUser); sql.append("', ");
    sql.append("'"); sql.append(boxAccess); sql.append("', ");
    sql.append("'"); sql.append(pop3Server); sql.append("', ");
    sql.append("'"); v = pop3Port; sql.append(v.toString()); sql.append("', ");
    sql.append("'"); sql.append(imapServer); sql.append("', ");
    sql.append("'"); v = imapPort; sql.append(v.toString()); sql.append("', ");
    sql.append("'"); sql.append(smtpServer); sql.append("', ");
    sql.append("'"); v = smtpPort; sql.append(v.toString()); sql.append("', ");
    sql.append("'"); v = ssl; sql.append(v.toString()); sql.append("', ");
    sql.append("'"); v = sendDefault; sql.append(v.toString()); sql.append("', ");
    sql.append("'"); v = recieveDefault; sql.append(v.toString()); sql.append("', ");
    sql.append("'"); sql.append(userSign); sql.append("'");
    sql.append(")");

  return sql;
}
