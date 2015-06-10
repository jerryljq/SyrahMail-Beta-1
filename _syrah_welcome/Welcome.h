#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "_syrah_account_setup/mailaccconfig.h"

namespace Ui {
class Welcome;
}

class Welcome : public QMainWindow
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent);
    ~Welcome();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Welcome *ui;
    MailAccConfig ui2;
};

#endif // MAINWINDOW_H
