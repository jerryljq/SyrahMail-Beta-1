#include "_syrah_mainwindow/mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include "_syrah_dbset/_syrah_user.h"
#include "_syrah_account_setup/mailaccconfig.h"
#include "QtWebkitWidgets/QWebView"
#include "QMessageBox"
#include "_syrah_seldef.h"

extern vector <_syrah_user*> _syrah_userTable;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();

    UserLogon *logon;
    logon = new UserLogon;


    connect(logon, SIGNAL(UserLogonStateChanged()), this, SLOT(ShowWindow()));
    connect(logon, SIGNAL(UserLogonStateChanged()),this,SLOT(Draw()));

    logon->show();
    setWindowTitle(tr("SyrahMail"));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_WriteMail_clicked()
{
    MailWindow *WriteWindow=new MailWindow;//(}QWidge}t *parent = 0);
    WriteWindow->setAttribute(Qt::WA_DeleteOnClose);
    WriteWindow->show();
    connect(WriteWindow, SIGNAL(achievedstore()), this, SLOT(Draw()));

}

void MainWindow::on_actionSetup_triggered()
{
    UserSetup *setup=new UserSetup;
    setup->setAttribute(Qt::WA_DeleteOnClose);
    setup->show();
}

void MainWindow::ShowWindow()
{
    vector<_syrah_user*>& uTable = _syrah_userTable;
    _syrah_sys.ReadDBS();
    this->show();
    for(unsigned long int i = 0; i < uTable[_syrah_sys.getCurrentUserID()]->uMailbox().size(); i++) {

        _Syrah_pop3_Ssl_Receiver *popssl;
        popssl = new _Syrah_pop3_Ssl_Receiver(this, i);
        connect(popssl,SIGNAL(NewMailArrived()),this,SLOT(receiveMail()));
    }
}

void MainWindow::on_actionAdd_Mail_account_triggered()
{
    MailAccConfig *NewMailAcc = new MailAccConfig;
    NewMailAcc->setAttribute(Qt::WA_DeleteOnClose);
    NewMailAcc->show();
}

void MainWindow::on_actionAdd_mail_account_triggered()
{
    NewMailAcc = new MailAccConfig;
    connect(NewMailAcc, SIGNAL(regfinished()), this, SLOT(regfinish()));
    connect(NewMailAcc, SIGNAL(regfinished()), this, SLOT(NewAccountAddedAutoReadMail()));
    NewMailAcc->setAttribute(Qt::WA_DeleteOnClose);
    NewMailAcc->show();
}

/*void MainWindow::ShowMail(QString Headline)
{
    if(Headline == "1")
        ui->webView->setUrl(QUrl(QStringLiteral("http://www.baidu.com")));
    else
        ui->webView->setUrl(QUrl(QStringLiteral("http://news.qq.com")));
}*/

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("Quit"),
            QString(tr("Are you sure to quit?")),
            QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::No) {
            event->ignore();
        }
        else if (button == QMessageBox::Yes) {
            qDebug() << "App going to die";
            _syrah_sys.writeData();
            delete _syrah_userTable[0];
            event->accept();
        }
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::Draw()
{
    QString DfSdr;
    DfSdr.append("Default Account: ").append(DefaultSender);
    ui->label->setText(DfSdr);

    vector<_syrah_user*>& uTable = _syrah_userTable;
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setHeaderLabel("Inbox");

    //static unsigned long folder_index = 0;
    //static unsigned long mail_index = 0;

    unsigned long uid = _syrah_sys.getCurrentUserID();

    QFont ft;
    ft.setBold(1);

    unsigned long mailboxCount, folderCount, mailCount;
    //unsigned long int uid = _syrah_sys.getCurrentUserID();
    qDebug() << uid << endl;

    mailboxCount = uTable[uid]->uMailbox().size();

    QTreeWidgetItem *mailbox[mailboxCount];

    for(unsigned int i=0; i<mailboxCount; i++) {
        mailbox[i] = new QTreeWidgetItem(ui->treeWidget, QStringList(uTable[uid]->uMailbox()[i]->getName()));
        //connect(mailbox[i], SIGNAL(itemPressed(QTreeWidgetItem *item, int column)), this, SLOT(ShowEmptyBackGround()));
        mailbox[i]->setFont(0, ft);
        QVariant k = i;
        mailbox[i]->setText(1,k.toString());
        folderCount = uTable[uid]->uMailbox()[i]->uFolder().size();
        QTreeWidgetItem *folder[folderCount];
        for(unsigned int j=0; j<folderCount; j++) {
            qDebug() << uTable[uid]->uMailbox()[i]->uFolder()[j]->getName() << endl;
            folder[j] = new QTreeWidgetItem(mailbox[i], QStringList(uTable[uid]->uMailbox()[i]->uFolder()[j]->getName()));
            k = j;
            folder[j]->setText(1,k.toString());
          //  connect(folder[j], SIGNAL(itemPressed(QTreeWidgetItem *item, int column)), this, SLOT(ShowFolder(QTreeWidgetItem *Item)));
            qDebug() << "bbbbbbbbbbbbbbbbbbb";
        }
    }

    connect(ui->treeWidget, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(ShowFolder(QTreeWidgetItem *, int)));
    ui->treeWidget->expandAll();
}

void MainWindow::ShowEmptyBackGround()
{
    ui->listWidget->clear();
}

void MainWindow::ShowFolder(QTreeWidgetItem *item, int column)
{
    if(item->parent() == NULL)
        ui->listWidget->clear();
    else {
        qDebug() << item->text(0);
        QString boxName = item->parent()->text(0).trimmed();
        unsigned long int boxId;
        //boxId = item->parent()->text(1).toULong();
        _syrah_sys.setCurrentMailBoxID(boxId);

        vector<_syrah_user*>& uTable = _syrah_userTable;
        unsigned long int uid = _syrah_sys.getCurrentUserID();


        for(unsigned long int i=0; i<uTable[uid]->uMailbox().size(); i++) {
            if(uTable[uid]->uMailbox()[i]->getName() == boxName) {
                _syrah_sys.setCurrentMailBoxID(i);
                boxId = i;
                break;
            }
        }

        QString folName = item->text(0).trimmed();
        unsigned long int folId;
        //folId = item->text(1).toULong();
        _syrah_sys.setCurrentFolderID(folId);
        for(unsigned long int i=0; i<uTable[uid]->uMailbox()[boxId]->uFolder().size(); i++) {
            if(uTable[uid]->uMailbox()[boxId]->uFolder()[i]->getName() == folName) {
                _syrah_sys.setCurrentFolderID(i);
                folId = i;
                break;
            }
        }
        ui->listWidget->clear();
        unsigned long mailCount = uTable[uid]->uMailbox()[boxId]->uFolder()[folId]->uMail().size();
        for(unsigned int k=0; k<mailCount; k++) {
           //QListWidgetItem *nMails = new QListWidgetItem(uTable[uid]->uMailbox()[boxId]->uFolder()[folId]->uMail()[k]->getSubject(), ui->listWidget);
           /*QString fuckMail = uTable[uid]->uMailbox()[boxId]->uFolder()[folId]->uMail()[k]->getSubject();
           QString fuckId = ("[");
           qlonglong wFuckId = uTable[uid]->uMailbox()[boxId]->uFolder()[folId]->uMail()[k]->Id;
           QVariant vFuckId = wFuckId;
           fuckId.append(vFuckId.toString());
           fuckId.append("],");
           fuckMail.prepend(fuckId);*/
           ui->listWidget->addItem(uTable[uid]->uMailbox()[boxId]->uFolder()[folId]->uMail()[k]->getSubject());
        }

    }
    connect(ui->listWidget,SIGNAL(currentTextChanged(QString)), this, SLOT(ShowMail(QString)));
}


void MainWindow::Timer_out()
{
    //Receiver = new _Syrah_pop3_Ssl_Receiver(this,"hejiaxunhe","Victor09","pop.163.com");
    //Receiver = new _Syrah_pop3_Receiver(this,"hejiaxunhe","Victor09","pop.163.com");
    //connect(Receiver, SIGNAL(MailsReceivedReady()),this, SLOT(ready_receive()));
}

void MainWindow::ready_receive()
{
}

void MainWindow::RecvMau()
{
   // pop = new _Syrah_pop3_Receiver(this,"2263909568","Victor09","pop.qq.com");
}

void MainWindow::on_pushButton_3_clicked()
{
    //RecvMau();
    Receiver();
}

void MainWindow::NewAccountAddedAutoReadMail() {
    unsigned long uid = _syrah_sys.getCurrentUserID();
    new _Syrah_pop3_Ssl_Receiver(this, _syrah_userTable[uid]->uMailbox().size()-1);
}

void MainWindow::ShowMail(QString subject)
{
    ui->textBrowser->clear();
    ui->MailInfo->clear();
    vector<_syrah_user*>& uTable = _syrah_userTable;
    unsigned long int uid = _syrah_sys.getCurrentUserID();


    for(unsigned long int i=0; i<uTable[uid]->uMailbox().size(); i++) {
        qDebug() << "111111111111111111***********";
        for(unsigned long int j=0; j<uTable[uid]->uMailbox()[i]->uFolder().size(); j++) {
            qDebug() << "22222222222222222222***********";
            //unsigned long i = _syrah_sys.getCurrentMailBoxID();
            //unsigned long j = _syrah_sys.getcurrentFolderID();
            unsigned long mailCount = uTable[uid]->uMailbox()[i]->uFolder()[j]->uMail().size();
            for(unsigned int k=0; k<mailCount; k++) {
                qDebug() << "33333333333333333333***********";
               if(uTable[uid]->uMailbox()[i]->uFolder()[j]->uMail()[k]->getSubject() == subject)
               {
                   qDebug() << "44444444444444444444***********";
                   ui->textBrowser->clear();


                   QByteArray TempContent = QVariant(uTable[uid]->uMailbox()[i]->uFolder()[j]->uMail()[k]->getContent()).toByteArray();
                   TempContent = TempContent.fromBase64(TempContent);
                   QByteArray OriTempCont(TempContent);
                   int front=TempContent.indexOf("<!");
                   int rear=TempContent.indexOf("</html>")+6;
                   TempContent = TempContent.mid(front,rear-front);
                   QString GeneralInfo = "Subject: ";
                   GeneralInfo.append(uTable[uid]->uMailbox()[i]->uFolder()[j]->uMail()[k]->subject).append("\n");
                   GeneralInfo.append("From: ").append(uTable[uid]->uMailbox()[i]->uFolder()[j]->uMail()[k]->from).append("\n");
                   ui->MailInfo->clear();
                   ui->MailInfo->setText(GeneralInfo);
                   if(front==-1 || rear == -1) {
                       ui->textBrowser->setText(OriTempCont);
                   }
                   else {
                       ui->textBrowser->setText(TempContent);
                   }
                   //ui->textBrowser->setText(OriTempCont);
                   qDebug() << TempContent;
               }
            }
        }
    }
}

void MainWindow::regfinish()
{
    ui->treeWidget->clear();
    Draw();
    //delete NewMailAcc;
}

void MainWindow::ReadDB()
{

}

void MainWindow::on_pushButton_4_clicked()
{
       QStringList k;
       QString Sub("MEM_TEST");
       QByteArray s;
       static unsigned long int index = 1;

       qDebug() << "Manually insert to MEM";
       ++index;
       int st=index;
       //_syrah_mail* m1 = new _syrah_mail(index,"uidl1",Sub.append(st),"ab@g.c",k,"a","a","S","dfdf","dfd","low","dfdf","dfd","dfd","df",s,"atta",st,"14","14");
       //vector<_syrah_user*>& uTable = _syrah_userTable;
       //_syrah_userTable[0]->uMailbox()[0]->uFolder()[0]->uMail().insert(pair<unsigned long int, _syrah_mail*>(index, m1));
       _syrah_sys.writeData();
       qDebug() << "InsertNew";

}

void MainWindow::Receiver()
{
    qDebug() << "********************";
    vector<_syrah_user*>& uTable = _syrah_userTable;
    QString CurrentAccount = uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->getUser();
    qDebug() << CurrentAccount;
    QString PassWord = uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->getPass();
    qDebug() << PassWord;
    QString RecvServer = uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->getRecvServer();
    qDebug() << RecvServer;
    int RecvPort = uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[_syrah_sys.getCurrentMailBoxID()]->getRecvPort();
    qDebug() << RecvPort;
    qDebug() << "********************";
    pop=new _Syrah_pop3_Receiver(this,CurrentAccount,PassWord, RecvServer);
    connect(pop,SIGNAL(NewMailArrived()),this,SLOT(receiveMail()));
}

void MainWindow::receiveMail()
{
    qDebug() <<"mails passed to mainwindow";

        /*if(pop->getMailsNum()==0)
        {
            //delete pop;
            return;
        }

        _syrah_mail_received mail=pop->getMails();
        QString show="";

        qDebug()<<"748927759273597298759275927592759275927957297592";

        qDebug()<<mail.subject;
        qDebug()<<mail.fromName<<" :"<<mail.from;
        qDebug()<<mail.toName<<" :"<<mail.to;

        for(int i=0;i<mail.content.size();i++)
        {
            qDebug()<<mail.content.at(i);
            qDebug() << mail.contentType.at(i);
            if(mail.contentType.at(i).contains("text"))
                show.append(mail.content.at(i));
        }

        show.prepend("<html>\n<body>\n").append("</body>\n</html>");


        qDebug() << show;
        ui->textBrowser->clear();
        ui->MailInfo->clear();
        QString GeneralInfo = "Subject: ";
        GeneralInfo.append(mail.subject).append("\n");
        GeneralInfo.append("From: ").append(mail.from).append("\n");
        ui->MailInfo->setText(GeneralInfo);
        ui->textBrowser->setText(show);
       // ui->textBrowser->setText("123");
        //sleep(1000);

      /*  for(int i=0;i<mail.AttachMentContent.size();i++)
        {
            show.clear();
            show.append(mail.AttachMentContent.at(i));
        }

        ui->textBrowser->setText(show);*/
}

void MainWindow::on_actionChange_default_account_triggered()
{
    _syrah_seldef *setdef;
    setdef = new _syrah_seldef;
    setdef->show();
    connect(setdef, SIGNAL(passtomain(QString)), this, SLOT(DefaultSenderChanged(QString)));
}

void MainWindow::DefaultSenderChanged(QString sender)
{
    setSender(sender);
    qDebug() << sender << "is sender";
}

void MainWindow::setSender(QString Sender)
{
    DefaultSender = Sender;
    QString DfSdr;
    DfSdr.append("Default Account: ").append(DefaultSender);
    ui->label->setText(DfSdr);
    vector<_syrah_user*>& uTable = _syrah_userTable;
    for(unsigned long int i = 0; i < uTable[_syrah_sys.getCurrentUserID()]->uMailbox().size(); i++)
    {
        if(Sender == uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[i]->getUser())
        {
            _syrah_sys.setCurrentMailBoxID(i);
            qDebug() << uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[i]->getUser();
        }
    }
}
