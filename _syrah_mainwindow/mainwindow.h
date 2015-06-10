#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "_syrah_write_mail/mailwindow.h"
#include "_syrah_user_setup/usersetup.h"
#include "_syrah_logon/userlogon.h"
#include "QTreeWidget"
#include <QTimer>
#include "_syrah_receiver/_Syrah_pop3_Ssl_Receiver.h"
#include "_syrah_receiver/_syrah_mail_received.h"
#include "_syrah_receiver/_syrah_mime_decoder.h"
#include "_syrah_receiver/_Syarh_pop3_Receiver.h"
#include "_syrah_account_setup/mailaccconfig.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    void RecvMau();
    void ReadDB();
    void Receiver();
    void setSender(QString Sender);

public slots:
    void on_WriteMail_clicked();

    void on_actionSetup_triggered();

    void ShowWindow();
    
    void Draw();

    void ShowEmptyBackGround();

    void ShowFolder(QTreeWidgetItem *item, int column);

    void Timer_out();

    void ready_receive();

    void ShowMail(QString);

    void receiveMail();

    void DefaultSenderChanged(QString sender);

    void NewAccountAddedAutoReadMail();

private slots:
    void on_actionAdd_Mail_account_triggered();

    void on_actionAdd_mail_account_triggered();

  //  void ShowMail(QString Headline);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_actionChange_default_account_triggered();

public slots:
    void regfinish();



private:
    Ui::MainWindow *ui;
    QTimer *RecvTimer;
    _Syrah_pop3_Receiver *pop;
    MailAccConfig *NewMailAcc;
    QString DefaultSender;
    //_Syrah_pop3_Receiver *Receiver;


    
};

#endif // MAINWINDOW_H
