#include "_syrah_mail.h"

_syrah_mail::_syrah_mail(unsigned long int iD,
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
                                     int statusx,
                                     QString inStackDateTimex, QString mailTimex) {
    Id = iD;    uidl = uidlx;
    subject = subjectx;
    from = fromx;   to = tox;
    cc = ccx;   bcc = bccx;
    messageId = messageIdx;
    inReplyTo = inReplyTox;
    contentType = contentTypex;
    precedence = precedencex;
    references = referencesx;
    replyTo = replyTox;
    sender = senderx;
    archivedAt = archivedAtx;
    content = contentx;
    attatchment = attatchmentx;
    status = statusx;
    inStackDateTime = inStackDateTimex;
    mailTime = mailTimex;

}

_syrah_mail::_syrah_mail()
{

}

void _syrah_mail::_set_subject(const QString& var_subject)
{
    subject = var_subject;
}

void _syrah_mail::_set_sender(const QString& var_sender)
{
    sender = var_sender;
}

void _syrah_mail::_set_content(const QByteArray& var_content)
{
    content = var_content;
}

void _syrah_mail::_set_to(const QString& var_to)
{
    to << var_to;
}

QString _syrah_mail::SqlWriteMail(unsigned long folid) {
  QString sql("");
  QVariant v;
  qlonglong q;

  sql.append("(");
  sql.append("'"); q = Id; v = q; sql.append(v.toString()); sql.append("', ");
  sql.append("'"); q = folid; v = q; sql.append(v.toString()); sql.append("', ");
  sql.append("'"); sql.append(uidl); sql.append("', ");
  sql.append("'"); sql.append(subject); sql.append("', ");
  sql.append("'"); sql.append(from); sql.append("', ");
  sql.append("'"); sql.append(to.join(';')); sql.append("', ");
  sql.append("'"); sql.append(cc); sql.append("', ");
  sql.append("'"); sql.append(bcc); sql.append("', ");
  sql.append("'"); sql.append(messageId); sql.append("', ");
  sql.append("'"); sql.append(inReplyTo); sql.append("', ");
  sql.append("'"); sql.append(contentType); sql.append("', ");
  sql.append("'"); sql.append(precedence); sql.append("', ");
  sql.append("'"); sql.append(references); sql.append("', ");
  sql.append("'"); sql.append(replyTo); sql.append("', ");
  sql.append("'"); sql.append(sender); sql.append("', ");
  sql.append("'"); sql.append(archivedAt); sql.append("', ");
  sql.append("'"); v = content; sql.append(v.toString()); sql.append("', ");
  sql.append("'"); sql.append(attatchment); sql.append("', ");
  sql.append("'"); v = status; sql.append(v.toString()); sql.append("', ");
  sql.append("'"); sql.append(inStackDateTime); sql.append("', ");
  sql.append("'"); sql.append(mailTime); sql.append("' ");
  sql.append(")");

  return sql;
}
