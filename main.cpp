#include "_syrah_mainwindow/mainwindow.h"
#include "_syrah_user_setup/usersetup.h"
#include "_syrah_logon/userlogon.h"
#include <QApplication>

#include <vector>
#include <_syrah_dbset/_syrah_kernel.h>
#include <_syrah_dbset/_syrah_user.h>

_syrah_kernel _syrah_sys;
vector <_syrah_user*> _syrah_userTable;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   /* UserLogon *logon=new UserLogon;
    logon->setAttribute(Qt::WA_DeleteOnClose);
    logon->show();*/
    MainWindow w;
    //w.show();
   // for(unsigned long i = 0; i<_syrah_userTable.size(); i++) { delete _syrah_userTable[i];}
    qDebug () << "I will die son.";
    return a.exec();
}
