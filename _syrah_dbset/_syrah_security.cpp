#include "_syrah_security.h"

_syrah_security::_syrah_security() {
    resetLen(0);
    status_SQLDefenceInit = false;
}
_syrah_security::_syrah_security(unsigned long l) {
    resetLen(l);
    status_SQLDefenceInit = false;
}

void _syrah_security::Init_SQLDefenceTable() {
    SQLDefenceTable[0] = "AUX";
    SQLDefenceTable[1] = "UX";
    SQLDefenceTable[2] = "CLOCK$";
    SQLDefenceTable[3] = "COM1";
    SQLDefenceTable[4] = "COM2";
    SQLDefenceTable[5] = "COM3";
    SQLDefenceTable[6] = "COM4";
    SQLDefenceTable[7] = "COM5";
    SQLDefenceTable[8] = "COM6";
    SQLDefenceTable[9] = "COM7";
    SQLDefenceTable[10] = "COM8";
    SQLDefenceTable[11] = "CON";
    SQLDefenceTable[12] = "CONFIG$";
    SQLDefenceTable[13] = "LPT1";
    SQLDefenceTable[14] = "LPT2";
    SQLDefenceTable[15] = "LPT3";
    SQLDefenceTable[16] = "LPT4";
    SQLDefenceTable[17] = "LPT5";
    SQLDefenceTable[18] = "LPT6";
    SQLDefenceTable[19] = "LPT7";
    SQLDefenceTable[20] = "LPT8";
    SQLDefenceTable[21] = "NUL";
    SQLDefenceTable[22] = "PRN";

    SQLDefenceTable[23] = ";";
    SQLDefenceTable[24] = "'";
    SQLDefenceTable[25] = "--";
    SQLDefenceTable[26] = "/*";
    SQLDefenceTable[27] = "*/";
    SQLDefenceTable[28] = "xp_";
    status_SQLDefenceInit = true;
}

void _syrah_security::resetLen(unsigned long l) { len = l; }

const string& _syrah_security::cxstr_LEN(string &str) {
    if(str.length() > len) {
        char* res_str = (char*)malloc(sizeof(char)*len);
        size_t length = str.copy(res_str,len);
        res_str[length] = '\0';
        _syrah_sys._debug("security","cxstr_LEN","&cxstr overflow");
        str = res_str;
    }
    return str;
}

const string _syrah_security::vcxstr_LEN(char* str) {
    string k = str;
    if(k.length() > len) {
        char* res_str = (char*)malloc(sizeof(char)*len);
        size_t length = k.copy(res_str,len);
        res_str[length] = '\0';
        _syrah_sys._debug("security","cxstr_LEN","&cxstr overflow");
        k = res_str;
    }
    return k;
}

/*const QString& _syrah_security::qstr_LEN(const QString& str) {
    if(str.length() > len) {
        str.resize(len);
        _debug("security","qstr_LEN","&qstr overflow");
    }
    return str;
}
*/

bool _syrah_security::verifyDb(string magic) {
    _syrah_security ins(16);
    string tmp_magic = ins.cxstr_LEN(magic);
    if(tmp_magic == SYRAH_VERIFY_DATABASE) return true;
    else return false;
}

const QString _syrah_security::SQLQueryDefence(QString query) {
    if(!status_SQLDefenceInit) Init_SQLDefenceTable();

    for(int i=23; i<=28; i++) {
        query.replace(SQLDefenceTable[i], "");
    }
    _syrah_sys._debug("security", "SQLQueryDefence", query);
    return query;
}
