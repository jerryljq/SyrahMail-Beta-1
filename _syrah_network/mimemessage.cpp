#include "_syrah_network/mimemessage.h"
#include "_syrah_dbset/_syrah_user.h"
#include "_syrah_dbset/_syrah_kernel.h"
#include <QDateTime>
#include "_syrah_write_mail/quotedprintable.h"
#include <typeinfo>


extern _syrah_kernel _syrah_sys;
extern vector <_syrah_user*> _syrah_userTable;

MimeMessage::MimeMessage(bool createAutoMimeContent) :
    hEncoding(MimePart::_8Bit)
{
    if (createAutoMimeContent)
        this->content = new MimeMultiPart();
}

MimeMessage::~MimeMessage()
{
}

MimePart& MimeMessage::getContent() {
    return *content;
}

void MimeMessage::setContent(MimePart *content) {
    this->content = content;
}

void MimeMessage::setSender(EmailAddress* e)
{
    this->sender = e;
//    Mail._set_sender(e->getAddress());
}

void MimeMessage::addRecipient(EmailAddress* rcpt, RecipientType type)
{
    switch (type)
    {
    case To:
        recipientsTo << rcpt;
        break;
    case Cc:
        recipientsCc << rcpt;
        break;
    case Bcc:
        recipientsBcc << rcpt;
        break;
    }
 //   Mail._set_to(rcpt->getAddress());
}

void MimeMessage::addTo(EmailAddress* rcpt) {
    this->recipientsTo << rcpt;
}

void MimeMessage::addCc(EmailAddress* rcpt) {
    this->recipientsCc << rcpt;
}

void MimeMessage::addBcc(EmailAddress* rcpt) {
    this->recipientsBcc << rcpt;
}

void MimeMessage::setSubject(const QString & subject)
{
    this->subject = subject;
 //   Mail._set_subject(subject);
}

void MimeMessage::addPart(MimePart *part)
{
    if (typeid(*content) == typeid(MimeMultiPart)) {
        ((MimeMultiPart*) content)->addPart(part);
       // Mail._set_content(content->getContent());
    };

}

void MimeMessage::setHeaderEncoding(MimePart::Encoding hEnc)
{
    this->hEncoding = hEnc;
}

const EmailAddress & MimeMessage::getSender() const
{
    return *sender;
}

const QList<EmailAddress*> & MimeMessage::getRecipients(RecipientType type) const
{
    switch (type)
    {
    default:
    case To:
        return recipientsTo;
    case Cc:
        return recipientsCc;
    case Bcc:
        return recipientsBcc;
    }
}

const QString & MimeMessage::getSubject() const
{
    return subject;
}

const QList<MimePart*> & MimeMessage::getParts() const
{
    if (typeid(*content) == typeid(MimeMultiPart)) {
        return ((MimeMultiPart*) content)->getParts();
    }
    else {
        QList<MimePart*> *res = new QList<MimePart*>();
        res->append(content);
        return *res;
    }
}

QString MimeMessage::toString()
{
    QString mime;

    mime = "From:";
    if (sender->getName() != "")
    {
        switch (hEncoding)
        {
        case MimePart::Base64:
            mime += " =?utf-8?B?" + QByteArray().append(sender->getName()).toBase64() + "?=";
            break;
        case MimePart::QuotedPrintable:
            mime += " =?utf-8?Q?" + QuotedPrintable::encode(QByteArray().append(sender->getName())).replace(' ', "_").replace(':',"=3A") + "?=";
            break;
        default:
            mime += " " + sender->getName();
        }
    }
    mime += " <" + sender->getAddress() + ">\r\n";

    mime += "To:";
    QList<EmailAddress*>::iterator it;  int i;
    for (i = 0, it = recipientsTo.begin(); it != recipientsTo.end(); ++it, ++i)
    {
        if (i != 0) { mime += ","; }

        if ((*it)->getName() != "")
        {
            switch (hEncoding)
            {
            case MimePart::Base64:
                mime += " =?utf-8?B?" + QByteArray().append((*it)->getName()).toBase64() + "?=";
                break;
            case MimePart::QuotedPrintable:
                mime += " =?utf-8?Q?" + QuotedPrintable::encode(QByteArray().append((*it)->getName())).replace(' ', "_").replace(':',"=3A") + "?=";
                break;
            default:
                mime += " " + (*it)->getName();
            }
        }
        mime += " <" + (*it)->getAddress() + ">";
    }
    mime += "\r\n";

    if (recipientsCc.size() != 0) {
        mime += "Cc:";
    }
    for (i = 0, it = recipientsCc.begin(); it != recipientsCc.end(); ++it, ++i)
    {
        if (i != 0) { mime += ","; }

        if ((*it)->getName() != "")
        {
            switch (hEncoding)
            {
            case MimePart::Base64:
                mime += " =?utf-8?B?" + QByteArray().append((*it)->getName()).toBase64() + "?=";
                break;
            case MimePart::QuotedPrintable:
                mime += " =?utf-8?Q?" + QuotedPrintable::encode(QByteArray().append((*it)->getName())).replace(' ', "_").replace(':',"=3A") + "?=";
                break;
            default:
                mime += " " + (*it)->getName();
            }
        }
        mime += " <" + (*it)->getAddress() + ">";
    }
    if (recipientsCc.size() != 0) {
        mime += "\r\n";
    }

    mime += "Subject: ";

    switch (hEncoding)
    {
    case MimePart::Base64:
        mime += "=?utf-8?B?" + QByteArray().append(subject).toBase64() + "?=";
        break;
    case MimePart::QuotedPrintable:
        mime += "=?utf-8?Q?" + QuotedPrintable::encode(QByteArray().append(subject)).replace(' ', "_").replace(':',"=3A") + "?=";
        break;
    default:
        mime += subject;
    }

    mime += "\r\n";
    mime += "MIME-Version: 1.0\r\n";

    mime += content->toString();
    return mime;
}

void MimeMessage::store(_syrah_mail *Mail)
{   
    //Init
    vector<_syrah_user*>& uTable = _syrah_userTable;
    map<unsigned long, _syrah_mail*>& UM = uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->uFolder()[1]->userMail;

    //Set Info
    Mail->_set_id(_syrah_sys.getUniId(2));
    Mail->_set_subject(subject);
    Mail->_set_content(content->getContent().toBase64());
    for (int i = 0; i < recipientsTo.size(); i++)
        Mail->_set_to(recipientsTo.at(i)->getAddress());
    Mail->_set_sender(sender->getAddress());

    //Insert
    UM.insert(pair<unsigned long int, _syrah_mail*>(uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->uFolder()[1]->userMail.size(), Mail));

    //Siginal
    emit SendSuccess();
    qDebug() << "Success!!!";

}

//void MimeMessage::sendSuccessfully(){}

