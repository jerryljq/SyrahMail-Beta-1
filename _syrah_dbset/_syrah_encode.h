#ifndef _SYRAH_ENCODE_H
#define _SYRAH_ENCODE_H

//Qt Library
#include <QString>
#include <QByteArray>
//C++ Standard Libaray
#include <string>
//Syrah Library
#include "_syrah_kernel.h"
#include "_syrah_security.h"
#include "encode/_syrah_encode_md5.h"

extern _syrah_kernel _syrah_sys;

using namespace std;



class _syrah_encode {
private:
    QByteArray qba;
    _syrah_encode_md5 MD5;
public:
    QString toQStr(string& cxstr);
    //QString toQStr(char* str);           //Use it carefully, may overflow!
    QString PtoQStr(int i);
    const string toCxStr(const QString& qstr);


    const string md5_encoder(const string& msg);
    const string md5_encoder(const QString& msg);

};

#endif // _SYRAH_ENCODE_H
