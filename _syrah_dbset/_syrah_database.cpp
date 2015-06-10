#include "_syrah_database.h"

_syrah_database::_syrah_database() {
    status_naDb = true;
    status_loadDrive = false;
    status_openDb = false;
}

_syrah_database::_syrah_database(const QString& username, const QString& password) {
    _db_user = username;
    inspector.resetLen(SYRAH_MAXLENGTH_USERNAME);
    _md5_user = coder.md5_encoder(username);
    _syrah_sys._debug("IO-database","constructor/md5_user",coder.toQStr(_md5_user));

    inspector.resetLen(SYRAH_MAXLENGTH_PASSWORD);
    _md5_pass = coder.md5_encoder(password);
    _syrah_sys._debug("IO-database","constructor/md5_pass",coder.toQStr(_md5_pass));

    resetEncyptKey();

    status_naDb = false;
    status_loadDrive = false;
    status_openDb = false;
}

_syrah_database::~_syrah_database() {
    encodeDb();
    _syrah_db.close();
    status_naDb = true;
    status_loadDrive = false;
    status_openDb = false;
}

bool _syrah_database::setNewDb() {
    if(_db_user.isEmpty() || _db_user.isNull() || _encrypt_key.empty() || status_naDb) {
        _syrah_sys._debug("database","setNewDb","naUser / naKey / naDb");
        return false;
    }
    _syrah_sys._debug("database","setNewDb","invoke setting new databse");
    loadDriver();
    setDbPath();
    if(!QFile::exists("mail/sysDb")) {
        _syrah_sys._debug("database","setNewDb","Template not exist");
        return false;
    }
    QFile::copy("mail/sysDb", _db_name);
    if(!checkDbExist()) {
        _syrah_sys._debug("database","setNewDb","Copy failed");
        return false;
    }
    openDb();
    _syrah_encode inc;
    _syrah_security ins;
    _syrah_db.exec(ins.SQLQueryDefence(inc.PtoQStr(1)));
    _syrah_db.exec(ins.SQLQueryDefence(inc.PtoQStr(2)));
    _syrah_db.exec(ins.SQLQueryDefence(inc.PtoQStr(3)));
    _syrah_db.exec(ins.SQLQueryDefence(inc.PtoQStr(4)));
    _syrah_db.exec(ins.SQLQueryDefence(inc.PtoQStr(5)));
    _syrah_db.exec(ins.SQLQueryDefence(inc.PtoQStr(6)));
    _syrah_db.exec(ins.SQLQueryDefence(inc.PtoQStr(7)));
    //_syrah_db.exec(ins.SQLQueryDefence("insert into setting values(1, \"abc\", \"abc\")"));  //NEED COMMIT
    _syrah_db.close();
    status_openDb = false;
    //QFile::copy(_db_name, "mail/debugCheck.db");
    encodeDb();
    return true;
}

void _syrah_database::resetUserName(const QString &username) {
    if(username.isEmpty() || username.isNull()) return;
    _db_user = username;
    inspector.resetLen(SYRAH_MAXLENGTH_USERNAME);
    _md5_user = coder.md5_encoder(username);
    _syrah_sys._debug("IO-database", "resetUser/md5_user", coder.toQStr(_md5_user));
    if(!_md5_pass.empty()) {
        status_naDb = false;
        resetEncyptKey();
    }
}

void _syrah_database::resetPassword(const QString &password) {
    if(password.isEmpty() || password.isNull()) return ;
    inspector.resetLen(SYRAH_MAXLENGTH_PASSWORD);
    _md5_pass = coder.md5_encoder(password);
    _syrah_sys._debug("IO-database", "resetPass/md5_pass", coder.toQStr(_md5_pass));
    if(!_md5_user.empty()) {
        status_naDb = false;
        resetEncyptKey();
    }
}

void _syrah_database::resetEncyptKey() {
    if(status_naDb) {
        _syrah_sys._debug("database","resetEncyptKey","N/A Database");
        return ;
    }
    QString key = coder.toQStr(_md5_user);
    key.append(coder.toQStr(_md5_pass));
    _encrypt_key = coder.md5_encoder(key);
    _syrah_sys._debug("IO-database","resetKey/key",coder.toQStr(_encrypt_key));
}

void _syrah_database::loadDriver() {
    if(status_naDb) {
        _syrah_sys._debug("database","loadDriver","N/A Database");
        return ;
    }
    if(status_loadDrive) return ;
    _syrah_db = QSqlDatabase::addDatabase("QSQLITE", _db_user);
    _syrah_sys._debug("database", "QSQLITE driver valid", _syrah_db.isValid()?"Yes":"No");
    status_loadDrive = true;
}

void _syrah_database::setDbPath() {
    if(status_naDb) {
        _syrah_sys._debug("database","setDbPath","N/A Database");
        return ;
    }
    _db_path = "mail/";
    _db_path.append(_db_user);
    _db_name = _db_path;
    _db_path.append(".box");
    _syrah_sys._debug("database", "setDbPath", _db_path);
    _syrah_db.setDatabaseName(_db_name);
    _syrah_sys._debug("database", "setDbPath(dbName)", _db_name);
}

bool _syrah_database::expandDb() {
    if(status_naDb) {
        _syrah_sys._debug("database","expandDb","N/A Database");
        return false;
    }
    if(_db_path.isEmpty()||_db_path.isNull()) {
        _syrah_sys._debug("database","expandDb","Invalid path");
        return false;
    }
    if(_db_name.isEmpty()||_db_name.isNull()) {
        _syrah_sys._debug("database","expandDb","Invalid new path");
        return false;
    }
    QFile opF1(_db_path), opF2(_db_name);
    QFileInfo opF(_db_path);
    if(opF1.open(QIODevice::ReadOnly)) {
        _syrah_sys._debug("database","expandDb","Open EncodedDb");
    }
    else {
        _syrah_sys._debug("database","expandDb","Cannot Open EncodeDb");
        return false;
    }
    char* EDB = new char[opF.size()];
    if(EDB == NULL) {
        _syrah_sys._debug("database","expandDb","Db too large");
        return false;
    }
    QDataStream In(&opF1);
    In.readRawData(EDB, opF.size());
    _syrah_sys._debug("database","expandDb/Filesize", opF.size());
    _syrah_sys._debug("database","expandDb/original",EDB);
    int ring=0;
    for(int i=0; i<opF.size(); i++) {
        EDB[i] ^= _encrypt_key[ring++];
        if(ring == 32) ring = 0;
    }
    _syrah_sys._debug("database","expandDb/decode",EDB);
    _syrah_security ins(16);
    if(!inspector.verifyDb(ins.vcxstr_LEN(EDB))) {
        _syrah_sys._debug("database","expandDb","Not correct Db format");
        opF2.remove();
        status_naDb = true;
        return false;
    }
    else _syrah_sys._debug("database","expandDb","Format verified");

    opF2.open(QIODevice::WriteOnly);
    QDataStream Out(&opF2);
    Out.writeRawData(EDB, opF.size());
    _syrah_sys._debug("database","expandDb","New file copied");
    opF1.remove();
    return true;
}

bool _syrah_database::encodeDb() {
    if(status_naDb) {
        _syrah_sys._debug("database","encodeDb","N/A Database");
        return false;
    }
    if(_db_path.isEmpty()||_db_path.isNull()) {
        _syrah_sys._debug("database","encodeDb","Invalid path");
        return false;
    }
    if(_db_name.isEmpty()||_db_name.isNull()) {
        _syrah_sys._debug("database","encodeDb","Invalid new path");
        return false;
    }
    QFile opF1(_db_name), opF2(_db_path);
    QFileInfo opF(_db_name);
    if(opF1.open(QIODevice::ReadOnly)) {
        _syrah_sys._debug("database","encodeDb","Open Db");
    }
    else {
        _syrah_sys._debug("database","encodeDb","Cannot Open Db");
        return false;
    }
    char* EDB = new char[opF.size()];
    if(EDB == NULL) {
        _syrah_sys._debug("database","encodeDb","Db too large");
        return false;
    }
    QDataStream In(&opF1);
    In.readRawData(EDB, opF.size());
    _syrah_sys._debug("database","encodeDb/Filesize", opF.size());
    _syrah_sys._debug("database","encodeDb/original",EDB);
    int ring=0;
    for(int i=0; i<opF.size(); i++) {
        EDB[i] ^= _encrypt_key[ring++];
        if(ring == 32) ring = 0;
    }
    _syrah_sys._debug("database","encodeDb/encode",EDB);

    opF2.open(QIODevice::WriteOnly);
    QDataStream Out(&opF2);
    Out.writeRawData(EDB, opF.size());
    _syrah_sys._debug("database","encodeDb","New file copied");
    opF1.remove();
    return true;
}

bool _syrah_database::checkDbExist() {
    if(status_naDb) {
        _syrah_sys._debug("database","checkdbExist","N/A Database");
        return false;
    }
    bool exist = QFile::exists(_db_name);
    if(!exist) _syrah_sys._debug("database","checkDbExist","Not exist");
    return exist;
}

bool _syrah_database::openDb() {
    if(status_naDb) {
        _syrah_sys._debug("database","openDb","N/A Database");
        return false;
    }
    if(!_syrah_db.open()) {
        _syrah_sys._debug("database","openDb","Cannot open");
        return false;
    }
    else {
        status_openDb = true;
        return true;
    }
}

bool _syrah_database::autoDatabase() {
  if(status_openDb) return true;
  if(status_naDb) return false;
  loadDriver();
  setDbPath();
  if(expandDb()) {
    if(checkDbExist()) {
      if(openDb()) {

        return true; }
        else {
          _syrah_sys._debug("database","autoload","Cannot open");
          status_naDb = false;
          status_openDb = false;
          return false;
        }
      }
      else {
        _syrah_sys._debug("database","autoload","Expanded not exist");
        status_naDb = true;
        status_openDb = false;
        return false;
      }
    }
    else {
      _syrah_sys._debug("database","autoload","Failed expand");
      status_naDb = true;
      status_openDb = false;
      return false;
  }
}

unsigned long int _syrah_database::primaryId(const QString table/*, vector<_syrah_user*>& uTable*/) {
    unsigned long int pkid;
    if(status_naDb) {
      _syrah_sys._debug("database","primaryId","N/A Database");
      return SYRAH_ID_ERROR;
    }
    if(status_openDb) {
      _syrah_sys._debug("database","primaryId","Database is not open");
      return SYRAH_ID_ERROR;
    }
    QSqlQuery query(_syrah_db);
    _syrah_security ins;
    QString SQL = "SELECT TOP 1 * FROM ";
    SQL.append(table.trimmed());
    SQL.append(" ORDER BY id DESC");
    query.exec(ins.SQLQueryDefence(SQL));
    pkid = query.value(0).toUInt();
    qDebug() << "pkid - " << pkid + 1;
    return (pkid+1);
}

bool _syrah_database::getNADB() { return status_naDb; }
bool _syrah_database::getOPENDB() { return status_openDb; }
bool _syrah_database::getkeyEmpty() {
  _syrah_encode enc;
  QString key(enc.toQStr(_encrypt_key));
  return key.isEmpty();
}
QSqlDatabase& _syrah_database::getDb() { return _syrah_db; }
