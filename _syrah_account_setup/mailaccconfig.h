#ifndef MAILACCCONFIG_H
#define MAILACCCONFIG_H

#include <QWizard>
#include "smtplogin.h"

namespace Ui {
class MailAccConfig;
}

class MailAccConfig : public QWizard
{
    Q_OBJECT

public:
    explicit MailAccConfig(QWidget *parent = 0);
    ~MailAccConfig();
    bool validateCurrentPage();


private slots:
    void on_Btn163_pressed();

    void on_Btn126_pressed();

    void on_BtnQQ_pressed();

    void on_BtnGoogle_pressed();

    void on_BtnYahoo_pressed();

    void on_BtnElse_pressed();

    void on_MailAcc_editingFinished();

    void on_MailPassWd_editingFinished();

    bool checkis();

signals:
    void regfinished();

private:
    Ui::MailAccConfig *ui;
    int AccNum;
    QString MailRecvServer;
    QString MailSendServer;
    QString MailAccount;
    QString MailAccPassWd;
    int Recvport;
    int Sendport;
    int SSLRecvPort;
    int SSLSendPort;
    bool IsRecvAlwSSL;

    QString MailBoxName;

    smtplogin *smtp;

};

#endif // MAILACCCONFIG_H
