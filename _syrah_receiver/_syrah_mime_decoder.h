#ifndef _SYRAH_MIME_DECODER_H
#define _SYRAH_MIME_DECODER_H

#include <QObject>
#include <QStringList>
#include <QDebug>
#include <QTextDocumentFragment>
#include "_syrah_mail_received.h"


class _syrah_Mime_Decoder : public QObject
{
    Q_OBJECT
public:
    explicit _syrah_Mime_Decoder(QObject *parent = 0);
    ~_syrah_Mime_Decoder();
    void Decord(const QString &content);
    _syrah_mail_received getMailDecoded();

signals:

public slots:

private:
    QString DecordedContent;
    QStringList MailContent;

    //QStringList ContentType;
    QStringList Content;
    QStringList ContentTransferEncoding;

    QStringList attachContent;
    QStringList attachContentTransferEncoding;

    int CurrentLine;
    _syrah_mail_received mail;

    QString readContent(QString boundary);
    void DoWithMultiPart(QString boundary);
};

#endif // _SYRAH_MIME_DECODER_H
