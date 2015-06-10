#include "_syrah_mail_received.h"

_syrah_mail_received::_syrah_mail_received()
{

}

_syrah_mail_received::~_syrah_mail_received()
{

}

void _syrah_mail_received::clear()
{
    subject.clear();
    contentType.clear();
    charset.clear();
    content.clear();
    mailTime.clear();
    from.clear();
    fromName.clear();
    to.clear();
    toName.clear();
    uidl.clear();
}

const _syrah_mail _syrah_mail_received::toMail()
{
    _syrah_mail mail;
    mail.subject=subject;
    mail.from=from;
    mail.to=to;
    mail.mailTime=mailTime;
    mail.contentType="text/html";
    mail.content=QVariant(content.join("")).toByteArray();
    mail.uidl=uidl;

    mail.attatchment=AttachMentName.join("_this_is_the_fucking_boundary__");

    return mail;
}
