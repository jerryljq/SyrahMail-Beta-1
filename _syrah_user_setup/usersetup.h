#ifndef USERSETUP_H
#define USERSETUP_H

#include <QWizard>

namespace Ui {
class UserSetup;
}

class UserSetup : public QWizard
{
    Q_OBJECT

public:
    explicit UserSetup(QWidget *parent = 0);
    ~UserSetup();
    bool CheckUsrName();
    bool CheckPassWd();
    void SaveUsrInfo();
    bool validateCurrentPage();
    bool CheckUser();

private slots:
    void on_lineEdit_textEdited(const QString &arg1);

    void on_PassEdit_textEdited(const QString &arg1);

private:
    Ui::UserSetup *ui2;
signals:
    void regfinished();
    void regbegin();
};

#endif // USERSETUP_H
