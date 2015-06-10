#ifndef _SYRAH_DATABASE_H
#define _SYRAH_DATABASE_H

// Qt Library
#include <QFile>
#include <QtSql>
// C++ Standard Library
#include <string>
// Syrah Library
#include "_syrah_kernel.h"
#include "_syrah_security.h"
#include "_syrah_encode.h"

extern _syrah_kernel _syrah_sys;
//extern vector <_syrah_user*> _syrah_userTable;

using namespace std;

class _syrah_database {
private:
  QSqlDatabase _syrah_db;

  string _md5_user;
  string _md5_pass;

  string _encrypt_key;
  QString _db_path;

  _syrah_security inspector;
  _syrah_encode coder;

  bool status_naDb;
  bool status_loadDrive;
  bool status_openDb;
public:
  QString _db_name;
  QString _db_user;
  _syrah_database();
  _syrah_database(const QString& username, const QString& password);
  ~_syrah_database();

  bool setNewDb();

  void resetUserName(const QString& username);
  void resetPassword(const QString& password);
  void resetEncyptKey();

  void loadDriver();
  void setDbPath();
  bool expandDb();
  bool encodeDb();
  bool checkDbExist();
  bool openDb();

  bool autoDatabase();
  //bool loadData(const unsigned long userid);
  bool writeData();
  unsigned long int primaryId(const QString table/*, vector<_syrah_user*>& uTable*/);

  bool getNADB();
  bool getOPENDB();
  void setOPENDB(bool v) {status_openDb = v;}
  bool getkeyEmpty();
  QSqlDatabase& getDb();

};

#endif // _SYRAH_DATABASE_H
