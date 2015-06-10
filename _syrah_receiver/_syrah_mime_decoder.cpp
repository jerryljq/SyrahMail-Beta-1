#include "_syrah_mime_decoder.h"

_syrah_Mime_Decoder::_syrah_Mime_Decoder(QObject *parent) : QObject(parent)
{

}

_syrah_Mime_Decoder::~_syrah_Mime_Decoder()
{

}

void _syrah_Mime_Decoder::Decord(const QString &content)
{
    DecordedContent="";
    CurrentLine=0;

    Content.clear();
    ContentTransferEncoding.clear();
    mail.clear();

    bool multiPart=false;
    QString boundary="";

    QString content_type;
    QString charset;
    QString transferEncoding;
    MailContent=content.split("\r\n");

    for(int i=0;i<MailContent.size();i++)
        qDebug()<<i<<": "<<MailContent.at(i);
    QString temp;


    //int CurrentLine=0;
    while(MailContent.at(CurrentLine)!=""&&CurrentLine<=MailContent.size())             //deal with the head of the mail
    {
        temp=MailContent.at(CurrentLine);
        qDebug()<<"Processing CurrentLine"<<CurrentLine<<":"<<temp;
        if(temp.split(":").at(0).trimmed().compare("From",Qt::CaseInsensitive)==0)
        {
            int front=temp.indexOf("<");
            int rear=temp.indexOf(">");
            mail.from=temp.mid(front+1,rear-front-1).trimmed();
            qDebug()<<mail.from;

            front=temp.indexOf(":");
            rear=temp.indexOf("<");
            mail.fromName=temp.mid(front+1,rear-front-1).trimmed();
            mail.fromName.remove("\"");
            qDebug()<<mail.fromName;

            CurrentLine++;
            continue;
        }

        if(temp.split(":").at(0).trimmed().compare("To",Qt::CaseInsensitive)==0)
        {
            int front=temp.indexOf("<");
            int rear=temp.indexOf(">");
            mail.to<<temp.mid(front+1,rear-front-1).trimmed();
            qDebug()<<mail.to.at(0);

            front=temp.indexOf(":");
            rear=temp.indexOf("<");
            mail.toName<<temp.mid(front+1,rear-front-1).trimmed();
            qDebug()<<mail.toName.at(0);

            CurrentLine++;
            continue;
        }

        if(temp.split(":").at(0).trimmed().compare("Subject",Qt::CaseInsensitive)==0)
        {
            mail.subject=temp.mid(temp.indexOf(":")+1).trimmed();
            qDebug()<<mail.subject;

            CurrentLine++;
            continue;
        }

        if(temp.split(":").at(0).trimmed().compare("Content-Type",Qt::CaseInsensitive)==0)
        {
            content_type=temp.split(":").at(1).trimmed();
            qDebug()<<content_type;

            if(content_type.contains("charset",Qt::CaseInsensitive))
            {
                charset=content_type.split(";").at(1).split("=").at(1).trimmed();
                charset.remove("\"");
                content_type=content_type.split(";").at(0);
                qDebug()<<charset;
            }
            else if(content_type.contains("boundary",Qt::CaseInsensitive))
            {
                qDebug()<<"8888888888888888888888888888888888888888888";
                boundary=content_type.split(";").at(1).split("=").at(1).trimmed();
                boundary.remove("\"");
                boundary.remove(";");
                content_type=content_type.split(";").at(0);
            }
            else
                content_type.remove(";");

            if(content_type.contains("multipart",Qt::CaseInsensitive))
                multiPart=true;

            CurrentLine++;
            continue;
        }

        if(multiPart)
        {
            qDebug()<<"shitshitshtishitshitshitshitshit";
            if(temp.split("=").at(0).trimmed().compare("boundary",Qt::CaseInsensitive)==0)
            {
                qDebug()<<"aaaaaaaaaaaaaaaaaaaaaaaaaaa";
                boundary=temp.mid(temp.indexOf("=")+1).trimmed();
                boundary.remove("\"");
                boundary.remove(";");
                multiPart=true;
                qDebug()<<boundary;
                qDebug()<<"above is the fucking boundary, man! shit !man!";

                CurrentLine++;
                continue;
            }
        }
        else if(temp.split("=").at(0).trimmed().compare("charset",Qt::CaseInsensitive)==0)
        {
            qDebug()<<"bbbbbbbbbbbbbbbbbbbbb";
            charset=temp.split("=").at(1).trimmed();
            charset.remove("\"");
            content_type=content_type.split(";").at(0);
            qDebug()<<charset;
            CurrentLine++;
            continue;
        }

        /*
        if(temp.contains("charset"))
        {
            QString charset=temp.split("=").at(1).trimmed();
            qDebug()<<charset;
        }
        */

        if(temp.split(":").at(0).trimmed().compare("Content-Transfer-Encoding",Qt::CaseInsensitive)==0)
        {
            transferEncoding=temp.split(":").at(1).trimmed();
            qDebug()<<transferEncoding;

            CurrentLine++;
            continue;
        }

        if(temp.split(":").at(0).trimmed().compare("Date",Qt::CaseInsensitive)==0)
        {
            QString date=temp.mid(temp.indexOf(":")+1).trimmed();
            qDebug()<<date;
        }



        CurrentLine++;
    }

    //deal with the mailContent
    if(multiPart)
        DoWithMultiPart(boundary);
    else
    {
        mail.contentType<<content_type;
        ContentTransferEncoding<<transferEncoding;
        mail.content<<readContent(".");
    }

    for(int i=0;i<Content.size();i++){
        qDebug()<<i<<ContentTransferEncoding.at(i);
        qDebug()<<i<<mail.contentType.at(i);
        qDebug()<<i<<"wocalei :"<<Content.at(i);
    }

    for(int i=0;i<Content.size();i++)
    {
        if(ContentTransferEncoding.at(i).compare("base64",Qt::CaseInsensitive)==0)
        {
            mail.content<<QByteArray().fromBase64(QVariant(Content.at(i)).toByteArray());
        }
        else
            mail.content<<Content.at(i);
    }

    for(int i=0;i<attachContent.size();i++)
    {
        if(attachContentTransferEncoding.at(i).compare("base64",Qt::CaseInsensitive)==0)
            mail.AttachMentContent<<QByteArray().fromBase64(QVariant(attachContent.at(i)).toByteArray());
        else
            mail.AttachMentContent<<QVariant(attachContent.at(i)).toByteArray();
    }

    qDebug()<<"2222222222222222222222222222222222222222";


    if(mail.content.size()>=2)
    {
        if(mail.contentType.at(1).contains("text/html"))
        {
            QTextDocumentFragment check1,check2;
            //QString check2=mail.content.at(i-1);
            qDebug()<<2333;
            check1=QTextDocumentFragment::fromHtml(mail.content.at(1));
            qDebug()<<3334;
            check2=QTextDocumentFragment::fromPlainText(mail.content.at(0));
            qDebug()<<4445;
            qDebug()<<"plain text"<<check1.toPlainText(); //.remove("\n");
            qDebug()<<"compare to:"<<check2.toPlainText();//.remove("\n");
            qDebug()<<check1.toPlainText().compare(check2.toPlainText());
            if(check1.toPlainText().compare(check2.toPlainText())==0)
            {
                qDebug()<<"it is right!!!!!!!!!!!!!!!!!!!!!!!!!!!";
                mail.content[0].clear();
            }
        }
    }
    qDebug()<<"33333333333333333333333333333333333333333333";
}

void _syrah_Mime_Decoder::DoWithMultiPart(QString boundary)
{
    qDebug()<<"now doing multipart, line :"<<CurrentLine;
    qDebug()<<boundary;
    bool multiPart;
    QString content_type;
    QString charset;
    QString transferEncoding;
    QString AttachmentName;
    QString subBoundary;

    QString binBoundary=boundary;
    binBoundary.prepend("--");
    QString endBoundary=binBoundary;
    endBoundary.append("--");
    QString temp;

    while(MailContent.at(++CurrentLine)!=binBoundary){
        qDebug()<<CurrentLine;
        qDebug()<<"current line"<<MailContent.at(CurrentLine);
        qDebug()<<"binBoundary"<<binBoundary;
    }                                   //go to the first

    qDebug()<<"3333333333333333333333333333333333333333333333333333333";

    while(CurrentLine<MailContent.size()&&MailContent.at(CurrentLine)!=endBoundary)
    {
        temp=MailContent.at(CurrentLine);
        if(temp==binBoundary)
        {
            //while(MailContent.at(++CurrentLine).trimmed()!="")
                //qDebug()<<MailContent.at(CurrentLine);
            content_type.clear();
            charset.clear();
            transferEncoding.clear();
            subBoundary.clear();
            AttachmentName.clear();

            while(MailContent.at(++CurrentLine).trimmed()!="")
            {
                temp=MailContent.at(CurrentLine);
                if(temp.split(":").at(0).trimmed().compare("Content-Type",Qt::CaseInsensitive)==0)
                {
                    content_type=temp.split(":").at(1).trimmed();
                    qDebug()<<content_type;

                    if(content_type.contains("charset",Qt::CaseInsensitive))
                    {
                        charset=content_type.split(";").at(1).split("=").at(1).trimmed();
                        charset.remove("\"");
                        content_type=content_type.split(";").at(0);
                        qDebug()<<charset;
                    }
                    else if(content_type.contains("boundary",Qt::CaseInsensitive))
                    {
                        subBoundary=content_type.split(";").at(1).split("=").at(1).trimmed();
                        subBoundary.remove("\"");
                        subBoundary.remove(";");
                        content_type=content_type.split(";").at(0);
                    }
                    else if(content_type.contains("name",Qt::CaseInsensitive))
                    {
                        AttachmentName=content_type.split(";").at(1).split("=").at(1).trimmed();
                        AttachmentName.remove("\"");
                        AttachmentName.remove(";");
                        QByteArray temp=QByteArray().fromBase64(QVariant(AttachmentName.mid(AttachmentName.indexOf("B?")+2)).toByteArray());
                        AttachmentName=QVariant(temp).toString();
                        content_type=content_type.split(";").at(0);
                    }
                    else
                        content_type.remove(";");

                    if(content_type.contains("multipart",Qt::CaseInsensitive))
                        multiPart=true;
                    else
                        multiPart=false;

                    continue;
                }

                if(multiPart)
                {
                    if(temp.split("=").at(0).trimmed().compare("boundary",Qt::CaseInsensitive)==0)
                    {
                        qDebug()<<"aaaaaaaaaaaaaaaaaaaaaaaaaaa";
                        subBoundary=temp.mid(temp.indexOf("=")+1).trimmed();
                        subBoundary.remove("\"");
                        subBoundary.remove(";");
                        multiPart=true;
                        qDebug()<<subBoundary;
                        qDebug()<<"above is the fucking boundary, man! shit !man!";

                        continue;
                    }
                }
                else if(temp.split("=").at(0).trimmed().compare("charset",Qt::CaseInsensitive)==0)
                {
                    qDebug()<<"bbbbbbbbbbbbbbbbbbbbb";
                    charset=temp.split("=").at(1).trimmed();
                    charset.remove("\"");
                    content_type=content_type.split(";").at(0);
                    qDebug()<<charset;
                    continue;
                }
                else if(temp.split("=").at(0).trimmed().compare("name",Qt::CaseInsensitive)==0)
                {
                    AttachmentName=temp.mid(temp.indexOf("=")+1).trimmed();
                    AttachmentName.remove("\"").remove(";");
                    QByteArray temp=QByteArray().fromBase64(QVariant(AttachmentName.mid(AttachmentName.indexOf("B?")+2)).toByteArray());
                    AttachmentName=QVariant(temp).toString();
                }


                if(temp.split(":").at(0).trimmed().compare("Content-Transfer-Encoding",Qt::CaseInsensitive)==0)
                {
                    transferEncoding=temp.split(":").at(1).trimmed();
                    qDebug()<<transferEncoding;

                    continue;
                }
            }
            qDebug()<<"choose between recursion and read";
            if(multiPart)
                DoWithMultiPart(subBoundary);
            else
            {
                if(content_type.contains("text",Qt::CaseInsensitive))
                {
                    mail.contentType<<content_type;
                    mail.charset<<charset;
                    ContentTransferEncoding<<transferEncoding;
                    Content<<readContent(boundary);
                }
                else
                {
                    mail.AttachmentContentType<<content_type;
                    mail.AttachMentName<<AttachmentName;
                    attachContentTransferEncoding<<transferEncoding;
                    attachContent<<readContent(boundary);

                }
            }

        }

        CurrentLine++;
    }

}

QString _syrah_Mime_Decoder::readContent(QString boundary)
{
    qDebug()<<"Now reading content,current line: "<<CurrentLine;
    QString tempContent;
    QString temp;
    if(boundary=="."){
        qDebug() << "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq";
        while(MailContent.at(CurrentLine).trimmed()!=boundary){
            temp=MailContent.at(CurrentLine).trimmed();
            if(temp!="")
                tempContent.append(temp);

            CurrentLine++;
        }
    }
    else{
        while(!MailContent.at(CurrentLine).contains(boundary)){
            temp=MailContent.at(CurrentLine).trimmed();
            if(temp!="")
                tempContent.append(temp);

            qDebug()<<11111111<<temp;
            CurrentLine++;
        }
        qDebug()<<"while loop jumped out at line "<<CurrentLine;
    }

    //Content<<tempContent;
    qDebug()<<"this is the fucking content!!!!!!!!!!!!!!!!!!!!!!: "<<tempContent;

    CurrentLine--;
    return tempContent;
}

_syrah_mail_received _syrah_Mime_Decoder::getMailDecoded()
{
    return mail;
}

