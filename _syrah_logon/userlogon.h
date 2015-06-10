#ifndef USERLOGON_H
#define USERLOGON_H

#include <QMainWindow>
#include "_syrah_user_setup/usersetup.h"

namespace Ui {
class UserLogon;
}

class UserLogon : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserLogon(QWidget *parent = 0);
    ~UserLogon();
    bool CheckUser();

private slots:

    void on_UserNameEdit_textEdited(const QString &arg1);

    void on_PassWdEdit_textEdited(const QString &arg1);

    void on_BtnCancel_clicked();

    void on_BtnLogon_clicked();

    void on_BtnNewUsr_clicked();

    void ShowLogonWindow();

    void HideLogonWindow();

signals:
    void UserLogonStateChanged();

private:
    Ui::UserLogon *ui;
    UserSetup *ui2;
    bool LoginState;
};

#endif // USERLOGON_H
