#ifndef _SYRAH_MAIL_RECEIVED_H
#define _SYRAH_MAIL_RECEIVED_H

#include "_syrah_dbset/mailbox/_syrah_mail.h"
#include <QByteArray>

class _syrah_mail_received : public _syrah_mail
{

public:
    QStringList contentType;
    QStringList charset;
    QStringList content;

    QString fromName;
    QStringList toName;

    QStringList AttachmentContentType;
    QStringList AttachMentName;
    QList<QByteArray> AttachMentContent;

public:
    _syrah_mail_received();
    ~_syrah_mail_received();
    void clear();
    const _syrah_mail toMail();
};

#endif // _SYRAH_MAIL_RECEIVED_H
