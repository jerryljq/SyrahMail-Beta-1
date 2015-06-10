#include "mailaccconfig.h"
#include "ui_mailaccconfig.h"
#include <QMessageBox>
#include <vector>
#include "_syrah_dbset/_syrah_user.h"
#include "_syrah_dbset/_syrah_kernel.h"

extern vector <_syrah_user*> _syrah_userTable;
extern _syrah_kernel _syrah_sys;

MailAccConfig::MailAccConfig(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::MailAccConfig)
{
    ui->setupUi(this);
    AccNum = 0;
    //port = 25;
    ui->verifyacc->hide();
    ui->Check->hide();
}


MailAccConfig::~MailAccConfig()
{
    delete ui;
}

bool MailAccConfig::validateCurrentPage()
{

    if(3 == this->currentPage()->nextId())
    {
        if(ui->MailAcc->text().trimmed() == "")
        {
            MailAccount = ui->MailAcc->text().trimmed();
            QMessageBox::warning(this, "No email address","Please input a valid account", QMessageBox::Yes);
            return false;
        } else
        if(ui->MailPassWd->text().trimmed() == "")
        {
            MailAccPassWd = ui->MailPassWd->text().trimmed();
            QMessageBox::warning(this, "No password","Please input your password", QMessageBox::Yes);
            return false;
        } else
        {
            bool a;
            ui->verifyacc->setEnabled(true);
            smtp = new smtplogin;

            if(!smtp->loginevent(ui->MailAcc->text(), ui->MailPassWd->text(), MailSendServer, Sendport))
            {
              qDebug() << "a is false";
              QMessageBox::warning(this, "Validation failed","Your account or password is not available", QMessageBox::Yes);
              return false;
            }

           /* ui->Check->setMinimum(0);
            ui->Check->setMaximum(0);
            ui->verifyacc->show();*/
        }
    }
    if(2 == this->currentPage()->nextId())
    {
        if(AccNum == 0)
        {
            QMessageBox::warning(this, " ","Choose your service provider first", QMessageBox::Yes);
            return false;
        }
    }

    if(4 == this->currentPage()->nextId())
    {
        if(ui->MailBoxName->text().trimmed() != "")
        {
            IsRecvAlwSSL = ui->RecvAlwSSL->isChecked();
            MailBoxName = ui->MailBoxName->text().trimmed();
            unsigned long int BoxSize = _syrah_userTable[_syrah_sys.getCurrentUserID()]->uMailbox().size();
            _syrah_userTable[_syrah_sys.getCurrentUserID()]->uMailbox().insert(pair<unsigned long int, _syrah_mailbox*>(BoxSize ,new _syrah_mailbox(BoxSize, MailBoxName, MailRecvServer, Recvport, MailSendServer, Sendport,
                                                                                                                                              MailAccount,MailAccPassWd, true)));
            BoxSize++;

            _syrah_userTable[_syrah_sys.getCurrentUserID()]->uMailbox()[BoxSize-1]->userFolder.insert(pair<unsigned long int, _syrah_folder*>(0 ,new _syrah_folder(_syrah_sys.getUniId(1), "Inbox")));

            //userFolder.insert(pair<unsigned long int, _syrah_folder*>(0 ,new _syrah_folder(_syrah_sys.getUniId(1), "Inbox")));
            //qDebug() << _syrah_sys.getUniId(1);
            _syrah_userTable[_syrah_sys.getCurrentUserID()]->uMailbox()[BoxSize-1]->userFolder.insert(pair<unsigned long int, _syrah_folder*>(1 ,new _syrah_folder(_syrah_sys.getUniId(1), "Sent")));


            qDebug() << MailBoxName;

            emit regfinished();

            return true;
        }
        else
        {
           // MailBoxName = ui->MailBoxName->text().trimmed();
            qDebug() << MailBoxName;
            return true;
        }
    }
    return true;
}

void MailAccConfig::on_Btn163_pressed()
{
    AccNum = 1;
    MailSendServer = "smtp.163.com";
    MailRecvServer = "pop.163.com";
    //QMessageBox::warning(this, " ","This service provider is not available at the moment", QMessageBox::Yes);
    MailBoxName = "163";
    Recvport = 110;
    SSLRecvPort = 995;
    Sendport = 25;
}

void MailAccConfig::on_Btn126_pressed()
{
    AccNum = 2;
    MailSendServer = "smtp.126.com";
    MailRecvServer = "pop.126.com";
    //QMessageBox::warning(this, " ","This service provider is not available at the moment", QMessageBox::Yes);
    MailBoxName = "126";
}

void MailAccConfig::on_BtnQQ_pressed()
{
    MailSendServer = "smtp.qq.com";
    MailRecvServer = "pop.qq.com";
    AccNum = 3;
    MailBoxName = "QQ";
    Recvport = 110;
    SSLRecvPort = 995;
    Sendport = 25;
}

void MailAccConfig::on_BtnGoogle_pressed()
{
    AccNum = 4;
    MailSendServer = "smtp.gmail.com";
    MailRecvServer = "pop.gmail.com";
    //QMessageBox::warning(this, " ","This service provider is not available at the moment", QMessageBox::Yes);
    MailBoxName = "Google";
}

void MailAccConfig::on_BtnYahoo_pressed()
{
    AccNum = 5;
    MailSendServer = "smtp.mail.yahoo.com";
    MailRecvServer = "pop.mail.yahoo.com";
    //QMessageBox::warning(this, " ","This service provider is not available at the moment", QMessageBox::Yes);
    MailBoxName = "Yahoo";
}

void MailAccConfig::on_BtnElse_pressed()
{
    //AccNum = 0;
    QMessageBox::warning(this, " ","This service provider is not available at the moment", QMessageBox::Yes);
    MailBoxName = "My Mailbox";
}

void MailAccConfig::on_MailAcc_editingFinished()
{
    MailAccount = ui->MailAcc->text().trimmed();
}

void MailAccConfig::on_MailPassWd_editingFinished()
{
    MailAccPassWd = ui->MailPassWd->text().trimmed();
}

bool MailAccConfig::checkis()
{
    return smtp->AccLoginState();
}

