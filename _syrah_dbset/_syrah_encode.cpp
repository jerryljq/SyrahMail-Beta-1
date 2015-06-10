#include "_syrah_encode.h"

QString _syrah_encode::toQStr(string& cxstr) {
    qba.clear();
    _syrah_security inspector(SYRAH_MAXLENGTH_CXSTRING);
    char *cx = (char *)malloc(sizeof(char)*SYRAH_MAXLENGTH_CXSTRING);   //-> conver to C++ syntax
    strcpy(cx, (inspector.cxstr_LEN(cxstr)).c_str());
    qba.append(cx);
    delete cx;
    return qba;
}

QString _syrah_encode::PtoQStr(int i) {
    qba.clear();
    switch (i) {
    case 1:
        qba.append(SYRAH_NEW_1);
        break;
    case 2:
        qba.append(SYRAH_NEW_2);
        break;
    case 3:
        qba.append(SYRAH_NEW_3);
        break;
    case 4:
        qba.append(SYRAH_NEW_4);
        break;
    case 5:
        qba.append(SYRAH_NEW_5);
        break;
    case 6:
        qba.append(SYRAH_NEW_6);
        break;
    case 7:
        qba.append(SYRAH_NEW_7);
        break;
    }
    return qba;
}

const string _syrah_encode::toCxStr(const QString& qstr) {
    qba.clear();
    qba.append(qstr);
    //return qba.toStdString();   Qt 5.4 provided
    return qba.data();
}



const string _syrah_encode::md5_encoder(const string& msg) {
    MD5.update(msg);
    return MD5.toString();
}

const string _syrah_encode::md5_encoder(const QString& msg) {
    MD5.update(toCxStr(msg));
    return MD5.toString();
}
