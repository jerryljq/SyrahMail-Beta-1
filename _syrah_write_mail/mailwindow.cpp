#include "mailwindow.h"
//#include "_syrah_write_mail/store.h"
#include "ui_mailwindow.h"
#include <iostream>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "_syrah_network/smtpclient.h"
#include "_syrah_network/mimepart.h"
#include "_syrah_network/mimemessage.h"
#include "_syrah_network/mimetext.h"
#include "_syrah_network/mimehtml.h"
#include "mimeattachment.h"
#include "mimefile.h"
#include "_syrah_dbset/_syrah_kernel.h"
#include "_syrah_dbset/_syrah_user.h"

extern _syrah_kernel _syrah_sys;
extern vector <_syrah_user*> _syrah_userTable;


MailWindow::MailWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MailWindow)
{
    ui->setupUi(this);
    var_Sender =new QLabel(tr("Send to: "));
    var_SendLine =new QLineEdit;
    var_Title = new QLabel(tr("The Title: "));
    var_TitleLine = new QLineEdit;
    var_Content = new QTextEdit;
    var_file = new QLabel(tr("Attachment: "));
    var_FileLine = new QLineEdit;
    var_attachment = new QPushButton(tr("Browse..."));
//    var_Content->setText(("<html><body><b>aaa</b></body></html>"));
    var_Send = new QPushButton(tr("Send"));
    QObject::connect(var_Send,SIGNAL(clicked()),this,SLOT(readySend()));
    QObject::connect(var_attachment,SIGNAL(clicked()),this,SLOT(select_file()));
    HtmlToolBar = new QToolBar;

    Size_large = new QAction(tr("&Size:Large"),this);
    Size_large -> setIcon(QIcon(":/Enlarge.png"));

    Size_normal = new QAction(tr("Size:normal"),this);
    Size_normal -> setIcon(QIcon(":/Normal.png"));

    Size_small = new QAction(tr("Size:small"),this);
    Size_small -> setIcon(QIcon(":/Small.png"));

    bold = new QAction(tr("Bold/Unbold"),this);
    bold -> setIcon(QIcon(":/Bold.png"));
    bold -> setCheckable(true);
    bold -> setChecked(false);

    HtmlToolBar -> addAction(Size_large);
    HtmlToolBar -> addAction(Size_normal);
    HtmlToolBar -> addAction(Size_small);
    HtmlToolBar -> addAction(bold);

    QObject::connect(Size_large,SIGNAL(triggered()),this,SLOT(enlarge_size()));
    QObject::connect(Size_normal,SIGNAL(triggered()),this,SLOT(norm_size()));
    QObject::connect(Size_small,SIGNAL(triggered()),this,SLOT(small_size()));
    QObject::connect(bold,SIGNAL(toggled(bool)),this,SLOT(set_bold(bool)));
    QObject::connect(bold,SIGNAL(toggled(bool)),this,SLOT(set_bold(bool)));

    Mail = new _syrah_mail;

    var_uppart=new QGridLayout;
    var_downpart=new QVBoxLayout;
    var_mainlayout=new QVBoxLayout;
    var_uppart->addWidget(var_Sender,0,0);
    var_uppart->addWidget(var_SendLine,0,1);
    var_uppart->addWidget(var_Title,1,0);
    var_uppart->addWidget(var_TitleLine,1,1);
    var_uppart->addWidget(var_Send,0,2);
    var_uppart ->addWidget(var_file,2,0);
    var_uppart ->addWidget(var_FileLine,2,1);
    var_uppart ->addWidget(var_attachment,2,2);

    var_downpart->addWidget(HtmlToolBar);
    var_downpart->addWidget(var_Content);

    var_mainlayout->addLayout(var_uppart);
    var_mainlayout->addLayout(var_downpart);
    setLayout(var_mainlayout);


}

MailWindow::~MailWindow()
{
    delete ui;
}

void MailWindow::send(QString Account, QString PassWd, QString SendServer, int port)
{

        //实例化发送邮件对象
        SmtpClient smtp(SendServer,
                        port,
                        SmtpClient::TcpConnection);//ui->ckSSL->isChecked()?SmtpClient::SslConnection:SmtpClient::TcpConnection
        smtp.setUser(Account);
        smtp.setPassword(PassWd);

        //构建邮件主题,包含发件人收件人附件等.
        MimeMessage message;
        message.setSender(new EmailAddress(Account));
        QObject::connect(&message,SIGNAL(SendSuccess()),this, SLOT(StoreReady()));

        //逐个添加收件人
        QStringList receiver =var_SendLine->text().split(';');
        for (int i = 0; i < receiver.size(); i++)
        {
             message.addRecipient(new EmailAddress(var_SendLine->text()));
        }
        //构建邮件标题
        message.setSubject(var_TitleLine->text());

        //构建邮件正文
        //MimeText text;
        //text.setText(var_Content->toPlainText());
        //message.addPart(&text);
        MimeHtml texth;
        texth.setHtml(var_Content->toHtml());

        message.addPart(&texth);

        //构建附件
        QString atta=var_FileLine->text();
        if (atta!=""){
            QStringList attas=atta.split(";");
            foreach (QString tempAtta, attas) {
                QFile *file=new QFile(tempAtta);
                if (file->exists()){
                    message.addPart(new MimeAttachment(file));
                }
            }
        }

        if (!smtp.connectToHost()){
            QMessageBox::critical(this,"Error","Connecting server failed.");
            return;
        }
        if (!smtp.login()){
            QMessageBox::critical(this,"Error","The Login failed.");
            return;
        }
        if (!smtp.sendMail(message,Mail)){
            QMessageBox::critical(this,"Error","The sending process failed.");
            return;
        }else{
            QMessageBox::information(this,tr("Sucess"),tr("The email has been sucessfully sent."));
        }
        smtp.quit();
        this->close();
}

QString MailWindow::GetContent()const
{
    return var_Content->toPlainText();
}
QString MailWindow::GetTitleLine()const
{
    return var_TitleLine->text();
}
QString MailWindow::GetSendLine()const
{
    return var_SendLine->text();
}

void MailWindow::enlarge_size()
{
    Cursor = var_Content->textCursor();
    if(Cursor.hasSelection())
    {
        QString a(Cursor.selectedText());
        Cursor.removeSelectedText();
        Cursor.insertHtml("<div><font size=\"5\">"+a+"</font></div>");
    }
    else
        var_Content->setFontPointSize(14);
}

void MailWindow::select_file()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    if (dialog.exec()){
        var_FileLine->clear();
        QStringList files=dialog.selectedFiles();
        foreach (QString file, files) {
            var_FileLine->setText(var_FileLine->text()+file+";");
        }
    }
}

void MailWindow::norm_size()
{
    Cursor = var_Content->textCursor();
    if(Cursor.hasSelection())
    {
        QString a(Cursor.selectedText());
        Cursor.removeSelectedText();
        Cursor.insertHtml("<div><font size=\"3\">"+a+"</font></div>");
    }
    else
        var_Content->setFontPointSize(10);
}

void MailWindow::small_size()
{
    Cursor = var_Content->textCursor();
    if(Cursor.hasSelection())
    {
        QString a(Cursor.selectedText());
        Cursor.removeSelectedText();
        Cursor.insertHtml("<div><font size=\"1\">"+a+"</font></div>");
    }
    else
        var_Content->setFontPointSize(7);
}

void MailWindow::set_bold(bool flag)
{
    if (flag==false)
    {
        if(Cursor.hasSelection())
        {
            QString a(Cursor.selectedText());
            Cursor.removeSelectedText();
            Cursor.insertHtml("<div><span style=\"font_weight:normal\">"+a+"</span></div>");
        }
        else
            var_Content->setFontWeight(50);
    }
    else
    {
        if(Cursor.hasSelection())
        {
            QString a(Cursor.selectedText());
            Cursor.removeSelectedText();
            Cursor.insertHtml("<div><span style=\"font_weight:normal\">"+a+"</span></div>");
        }
        else
            var_Content->setFontWeight(75);
    }
}

void MailWindow::readySend()
{
    qDebug() << "********************";
    vector<_syrah_user*>& uTable = _syrah_userTable;
    QString CurrentAccount = uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->getUser();
    qDebug() << CurrentAccount;
    QString PassWord = uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->getPass();
    qDebug() << PassWord;
    QString SendServer = uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->getSmtpServer();
    qDebug() << SendServer;
    int SendPort = uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->getSendPort();
    qDebug() << SendPort;
    qDebug() << "********************";
    send(CurrentAccount, PassWord, SendServer, SendPort);
}

void MailWindow::StoreReady()
{
    emit achievedstore();
}
