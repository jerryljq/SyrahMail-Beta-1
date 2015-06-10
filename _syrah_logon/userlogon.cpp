#include "userlogon.h"
#include "ui_userlogon.h"
#include <QMessageBox>
#include <vector>
#include "_syrah_dbset/_syrah_user.h"
#include "_syrah_welcome/Welcome.h"
#include "_syrah_mainwindow/mainwindow.h"

#include <stdio.h>

extern vector <_syrah_user*> _syrah_userTable;
extern _syrah_kernel _syrah_sys;

UserLogon::UserLogon(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserLogon)
{
    ui->setupUi(this);
    ui->PassWdEdit->setEnabled(false);
    ui->PassWdEdit->setEchoMode(QLineEdit::Password);
    ui->BtnLogon->setEnabled(ui->PassWdEdit->hasAcceptableInput());
}

UserLogon::~UserLogon()
{
    delete ui;
}

void UserLogon::on_UserNameEdit_textEdited(const QString &arg1)
{
    ui->PassWdEdit->setEnabled(ui->UserNameEdit->hasAcceptableInput());
}

void UserLogon::on_PassWdEdit_textEdited(const QString &arg1)
{
    ui->BtnLogon->setEnabled(ui->PassWdEdit->hasAcceptableInput());
}

void UserLogon::on_BtnCancel_clicked()
{

    this->close();
}

void UserLogon::on_BtnLogon_clicked()
{
    if(CheckUser())
    {
        _syrah_userTable.push_back(new _syrah_user(ui->UserNameEdit->text().trimmed(),ui->PassWdEdit->text().trimmed()));

        if(!_syrah_userTable[_syrah_sys.getCurrentUserID()]->userDb.autoDatabase())
        {
            _syrah_userTable.pop_back();
            QMessageBox::warning(this, "Invalid account","Please check your user name and password", QMessageBox::Yes);
            LoginState = false;
        } else
        {
         //_syrah_userTable.push_back(new _syrah_user(ui->UserNameEdit->text().trimmed(),ui->PassWdEdit->text().trimmed()));
         //_syrah_userTable[0]->preview();
         //_syrah_userTable[0]->Query();

         QMessageBox::information(this, "Logon Succeed","Logon Succeed", QMessageBox::Yes);
         _syrah_sys.setCurrentUserName(ui->UserNameEdit->text().trimmed());

         emit UserLogonStateChanged();
         //delete _syrah_userTable[0];

         this->close();
        // Welcome *w = new Welcome;
        // w->show();
        }
    } else if (ui->UserNameEdit->text().trimmed() == "")
    {
        QMessageBox::warning(this, "No User Name","User Name cannot be empty, please input username.", QMessageBox::Yes);
        LoginState = false;
    }
    else
    {
        QMessageBox::warning(this, "No account available","Please register new account first", QMessageBox::Yes);
        LoginState = false;
        ui->UserNameEdit->clear();
        ui->UserNameEdit->setFocus();
    }
}

void UserLogon::on_BtnNewUsr_clicked()
{
    ui2 = new UserSetup;
    this->hide();
    connect(ui2, SIGNAL(regfinished()), this, SLOT(ShowLogonWindow()));
    ui2->show();
}

bool UserLogon::CheckUser()
{
    QString FileName;
    FileName.append("mail/").append(ui->UserNameEdit->text().trimmed()).append(".box");
    QFile UsrDBName;
    UsrDBName.setFileName(FileName);
    qDebug() << "is Template Exist?" << QFile::exists("mail/sysDb");
    qDebug() << QDir::current();
    if(!UsrDBName.exists())
        return false;
    else
    {
        return true;
    }
}

void UserLogon::ShowLogonWindow()
{
    this->show();
}

void UserLogon::HideLogonWindow()
{
    this->hide();
}

