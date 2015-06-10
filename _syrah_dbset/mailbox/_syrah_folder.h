#ifndef _SYRAH_FOLDER_H
#define _SYRAH_FOLDER_H

//Qt Library
#include <QString>
//C++ Standard Library
#include <map>
//Syrah Library
#include "_syrah_dbset/mailbox/_syrah_mail.h"

using namespace std;

class _syrah_folder
{
private:
    QString folderName;

    bool FolderSecurity;    // may support in the future
    QString AccessCode;     // using AccessCode encrypts each mail first


public:
    unsigned long int Id;
    map<unsigned long int, _syrah_mail*> userMail;
    _syrah_folder(unsigned long int iD, const QString& fName,bool Security,
                  QString AccessC,map<unsigned long int, _syrah_mail*> user) {
        Id = iD;
        folderName = fName;
        FolderSecurity = Security;
        AccessCode = AccessC;
        userMail = user;
    }

    _syrah_folder(unsigned long int iD, const QString& fName) {
        Id = iD;
        folderName = fName;
        //FolderSecurity = Security;
        //AccessCode = AccessC;
        userMail.clear();
    }

    const unsigned long int& getId();
    QString getName() { return folderName; }

    //const bool& isSecure();    // may support in the future
    //const QString& getCode();     // using AccessCode encrypts each mail first

    map<unsigned long int, _syrah_mail*>& uMail();
    QString SqlWriteFolder(unsigned long boxid);
};

#endif // _SYRAH_FOLDER_H
