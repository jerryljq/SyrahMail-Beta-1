/*mainmial.h
 *This .h file is to extern all the variables that will be used in the Mail part.
 *Alan Ruijia & Dec. 9th
 */

#ifndef MAILWINDOW_H
#define MAILWINDOW_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QToolBar>
#include <QAction>
#include <QTextCursor>
#include <QTextDocumentFragment>
#include <QFile>
#include <QDialog>
#include <QIcon>
#include "_syrah_dbset/mailbox/_syrah_mail.h"

namespace Ui {
class MailWindow;
}

class MailWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MailWindow(QWidget* parent = 0);
    ~MailWindow();
    QString GetContent()const;
    QString GetTitleLine()const;
    QString GetSendLine()const;
    _syrah_mail *Mail;
signals:
    void achievedstore();

public slots:
    void readySend();
    void StoreReady();

private slots:
    void send(QString Account, QString PassWd, QString SendServer, int port);
    void enlarge_size();
    void norm_size();
    void small_size();
    void select_file();
    void set_bold(bool flag);

private:
    Ui::MailWindow *ui;
    QLabel *var_Sender,                    //var_Sender: The Label of Sender name;
           *var_Title,                     //var_Title: The Label of Title;
           *var_file;                      //var_file: If the mail can send files, it will be the Label of the file.
    QPushButton *var_Send,                 // var_Send: Sending mail button;
                *var_Save,                 // var_Save: The button to save the mail as a draft
                *var_attachment;
    QLineEdit *var_SendLine,               //var_SendLine: This variable is to store the sending address.
              *var_TitleLine,              //var_TitleLine: This variable is to store the title;
              *var_FileLine;
    QTextEdit *var_Content;                 //The variable is to store the content  of the mail.

    QToolBar *HtmlToolBar;

    QAction *Size_large,
            *Size_normal,
            *Size_small,
            *bold;

    QGridLayout *var_uppart;
    QVBoxLayout *var_downpart,
                *var_mainlayout;
    QString var_SSend,
            var_STitle;
    QTextCursor Cursor;

    bool Check();

//    QString modify(QString text);
};

#endif // MAILWINDOW_H
