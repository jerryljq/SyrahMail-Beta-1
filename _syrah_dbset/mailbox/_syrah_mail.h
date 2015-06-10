#ifndef _SYRAH_MAIL_H
#define _SYRAH_MAIL_H

//Qt Library
#include <QString>
#include <QTextBlock>
#include <QTextDocument>
#include <QDateTime>
//C++ Standard Library
#include <map>
//Syrah Library
#include "_syrah_dbset/_syrah_kernel.h"

using namespace std;



class _syrah_mail
{
public:
//private:
    unsigned long int Id;
    QString uidl;
    QString subject;
    QString from;
    QStringList to;
    QString cc, bcc;
    QString messageId;
    QString inReplyTo;
    QString contentType;
    QString precedence;
    QString references;
    QString replyTo;
    QString sender;
    QString archivedAt;         //More details on WikiPedia["Email"]

    QByteArray content;
    QString attatchment;

    int status;
    QString inStackDateTime;
    QString mailTime;         //Send Time or Receive Time

    //_syrah_mail(unsigned long int iD, const QString& msubject);
    _syrah_mail();
    _syrah_mail(unsigned long int iD,
    QString uidlx,
    QString subjectx,
    QString fromx,
    QStringList tox,
    QString ccx, QString bccx,
    QString messageIdx,
    QString inReplyTox,
    QString contentTypex,
    QString precedencex,
    QString referencesx,
    QString replyTox,
    QString senderx,
    QString archivedAtx,
    QByteArray contentx,
    QString attatchmentx,
    int status,
    QString inStackDateTimex, QString mailTimex);

    void _set_subject(const QString& var_subject);
    void _set_sender(const QString& var_sender);
    void _set_content(const QByteArray& var_content);
    void _set_to(const QString& var_to);
    void _set_id(unsigned long int v) { Id = v; }

    QString getSubject() { return subject; }

    QByteArray getContent() {return content;}
    QString SqlWriteMail(unsigned long folid);
};

#endif // _SYRAH_MAIL_H
