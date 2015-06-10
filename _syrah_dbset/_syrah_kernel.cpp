// Syrah Library
#include "_syrah_kernel.h"
#include "_syrah_user.h"
#include "mailbox/_syrah_folder.h"
#include "mailbox/_syrah_mail.h"

extern vector <_syrah_user*> _syrah_userTable;

_syrah_kernel::_syrah_kernel() {
    //Initialise Setting
    AppName = "Syrah Mail";
    AppGroup = "No Sleepless";
    AppVersion = "Alpha";
    //QCoreApplication::setOrganizationName("No Sleepless");
    //QCoreApplication::setOrganizationDomain("No_Sleepless.com");
    //QCoreApplication::setApplicationName("Syrah Mail");

    //Read Setting
    //debug = _syrah_setting.value("system/debug").toBool();
    debug = true;  //Forced open Debug
}

_syrah_kernel::~_syrah_kernel() {
    //_syrah_setting.setValue("system/debug", debug);
    //_syrah_setting.sync();
}


void _syrah_kernel::getVersion() {
    qDebug() << AppName << " ver. " << AppVersion << " (" << AppGroup << ")";
}

bool _syrah_kernel::getDebugStatus() {
    return debug;
}

void _syrah_kernel::_debug(const QString& infoClass, const QString& infoFunction, const QVariant& infoDetails) {
    if(debug) {
        QDateTime rec;
        qDebug() << rec.currentDateTime().toString() << "[" << rec.currentDateTimeUtc().toString() << "]";
        qDebug() << infoClass << " :: " << infoFunction << " with:";
        qDebug() << "    - " << infoDetails << "\n";
    }
}

void _syrah_kernel::ReadDBS()
{
    vector<_syrah_user*>& uTable = _syrah_userTable;
    unsigned long uid = currentUserId;
    static unsigned long folder_index = 0;
    static unsigned long mail_index = 0;

    QSqlQuery q(uTable[uid]->userDb.getDb());
    _syrah_security ins;
    QString SQL = "select * from mailbox";
    q.exec(ins.SQLQueryDefence(SQL));
    while(q.next()) {
        _syrah_mailbox* nBox;
        map<unsigned long int, _syrah_folder*> uF;
        map<unsigned long int, _syrah_contacts*> uC;        //>This is empty but completes constructor of mailbox
        vector<QString> mS;
        nBox = new _syrah_mailbox(q.value(0).toUInt(), q.value(1).toString(), q.value(4).toString(), q.value(5).toUInt(),
                                  q.value(8).toString(), q.value(9).toUInt(), q.value(2).toString(),
                                  q.value(3).toString(), q.value(10).toBool(), q.value(11).toInt(), q.value(12).toInt(),
                                  uF, uC, mS, q.value(13).toString());
        uTable[uid]->uMailbox().insert(pair<unsigned long, _syrah_mailbox*>(q.value(0).toUInt(), nBox));
    }

    unsigned long dbmboxC, dbfdC;

    dbmboxC = uTable[uid]->uMailbox().size();
    for(unsigned int i=0; i<dbmboxC; i++) {
        QString s1 = "select * from folder where boxid=";
        qlonglong ks = uTable[uid]->uMailbox()[i]->Id;
        QVariant k = ks;
        s1.append(k.toString());
        s1.append(" order by id");
        q.exec(ins.SQLQueryDefence(s1));
        map<unsigned long int, _syrah_folder*> UF;
        UF.clear();
        folder_index=0;
        while(q.next()) {
            //qDebug() << q.value(0);
            _syrah_folder* uFolder;
            map<unsigned long int, _syrah_mail*> uM;
            uFolder = new _syrah_folder(q.value(0).toUInt(), q.value(2).toString(), q.value(3).toBool(), q.value(4).toString(), uM);
            UF.insert(pair<unsigned long int, _syrah_folder*>(folder_index++, uFolder));
        }
        uTable[uid]->uMailbox()[i]->userFolder = UF;
    }

    for(unsigned int long ib=0; ib<uTable[uid]->uMailbox().size(); ib++) {
        dbfdC = uTable[uid]->uMailbox()[ib]->uFolder().size();
        for(unsigned int i=0; i<dbfdC; i++) {
            QString s1 = "select * from mail where folderid=";
            qlonglong ks = uTable[uid]->uMailbox()[ib]->userFolder[i]->Id;
            QVariant k = ks;
            s1.append(k.toString());
            s1.append(" order by id");
            q.exec(ins.SQLQueryDefence(s1));
            map<unsigned long int, _syrah_mail*> UM;
            UM.clear();
            mail_index=0;
            while(q.next()) {
                _syrah_mail* uM;
                uM = new _syrah_mail(
                            q.value(0).toUInt(),        //id
                            q.value(2).toString(),      //uidl
                            q.value(3).toString(),      //subject
                            q.value(4).toString(),      //from
                            q.value(5).toStringList(),  //to
                            q.value(6).toString(),      //cc
                            q.value(7).toString(),      //bcc
                            q.value(8).toString(),      //messageId
                            q.value(9).toString(),      //inReplyTo
                            q.value(10).toString(),     //contentType
                            q.value(11).toString(),     //precedence
                            q.value(12).toString(),     //references
                            q.value(13).toString(),     //replyTo
                            q.value(14).toString(),     //sender
                            q.value(15).toString(),     //archivedAt
                            q.value(16).toByteArray(),  //content       //!!!!!!!!!!
                            q.value(17).toString(),     //attachment
                            q.value(18).toInt(),        //status
                            q.value(19).toString(),     //inStackDateTime
                            q.value(20).toString());    //mailTime
                UM.insert(pair<unsigned long int, _syrah_mail*>(mail_index++, uM));
            }
            uTable[uid]->uMailbox()[ib]->uFolder()[i]->userMail = UM;
        }
    }
}

bool _syrah_kernel::writeData() {
  unsigned long uid = _syrah_sys.getCurrentUserID();
  vector<_syrah_user*>& uTable = _syrah_userTable;
  _syrah_database& uDb = uTable[uid]->userDb;

  if(uDb._db_user.isEmpty() || uDb._db_user.isNull() || uDb.getkeyEmpty() || uDb.getNADB()) {
    _debug("kernel","writeData","naUser / naKey / naDb");
    return false;
  }
  _debug("kernel","writeData","invoke writing mem to databse");
  uDb.loadDriver();
  uDb.setDbPath();
  if(!QFile::exists(uDb._db_name)) {      //check expanded database whether exist
    _debug("kernel","writeData-flow","no expanded database, IGNORE");
  } else {
    _debug("kernel","wirteData-flow","find expanded database");
  }
  if(!QFile::remove(uDb._db_name)) {      //remove original(expanded) database
    _debug("kernel","writeData-flow","remove expanded database, FAILED");
    return false;
  } else {
    _debug("kernel","wirteData-flow","remove expanded database, SUCCESSED");
  }
  if(!QFile::exists("mail/sysDb")) {  //check databse template whether exist
    _debug("kernel","writeData","template not exist, FAILED");
    return false;
  }
  if(!QFile::copy("mail/sysDb", uDb._db_name)) {  //copy database template
    _debug("kernel","writeData","copy template, FAILED");
    return false;
  } else {
    _debug("kernel","writeData","copied template.");
  }
  if(!uDb.checkDbExist()) {
    _debug("kernel","writeData","Copy failed");
    return false;
  }
  uDb.openDb();
  _syrah_encode inc;
  _syrah_security ins;
  uDb.getDb().exec(ins.SQLQueryDefence(inc.PtoQStr(1)));            //write SQL template(create table)
  uDb.getDb().exec(ins.SQLQueryDefence(inc.PtoQStr(2)));
  uDb.getDb().exec(ins.SQLQueryDefence(inc.PtoQStr(3)));
  uDb.getDb().exec(ins.SQLQueryDefence(inc.PtoQStr(4)));
  uDb.getDb().exec(ins.SQLQueryDefence(inc.PtoQStr(5)));
  uDb.getDb().exec(ins.SQLQueryDefence(inc.PtoQStr(6)));
  uDb.getDb().exec(ins.SQLQueryDefence(inc.PtoQStr(7)));
  //_syrah_db.exec(ins.SQLQueryDefence("insert into setting values(1, \"abc\", \"abc\")"));  //NEED COMMIT

  //write data to database from mem
    unsigned long FX = 0;
    for(unsigned long i=0; i<uTable[uid]->uMailbox().size(); i++) {
        QString s("");
        s = "INSERT INTO mailbox VALUES ";
        s.append(uTable[uid]->uMailbox()[i]->SqlWriteMailbox());
        qDebug() << s ;
        uDb.getDb().exec(s);
        for(unsigned long j=0; j<uTable[uid]->uMailbox()[i]->uFolder().size(); j++) {
            s = "INSERT INTO folder VALUES ";
            s.append(uTable[uid]->uMailbox()[i]->uFolder()[j]->SqlWriteFolder(i));
            qDebug() << s;
            uDb.getDb().exec(s);
            qDebug() << "size" << uTable[uid]->uMailbox()[i]->uFolder()[j]->uMail().size();
            for(unsigned long k=0; k<uTable[uid]->uMailbox()[i]->uFolder()[j]->uMail().size(); k++) {
                s = "INSERT INTO mail VALUES ";
                s.append(uTable[uid]->uMailbox()[i]->uFolder()[j]->uMail()[k]->SqlWriteMail(FX));
                qDebug() << s;
                uDb.getDb().exec(s);
            }
            FX++;
        }
    }
    uDb.getDb().close();
    uDb.setOPENDB(false);
  //QFile::copy(_db_name, "mail/debugCheck.db");
  //uDb.encodeDb();       //encode is set in destructor
  return true;
}

unsigned long int _syrah_kernel::getUniId(int type) {
    //type = 1 - folder
    //type = 2 - mail
    unsigned long uid = _syrah_sys.getCurrentUserID();
    vector<_syrah_user*>& uTable = _syrah_userTable;
    //_syrah_database& uDb = uTable[uid]->userDb;

    unsigned long res = 0;
    switch (type) {
    case 1:     //folder
        res = 0;
        for(unsigned long int i=0; i<uTable[uid]->uMailbox().size(); i++) {
            res += uTable[uid]->uMailbox()[i]->uFolder().size();
        }
        break;
    case 2:     //mail
        res = 0;
        for(unsigned long int i=0; i<uTable[uid]->uMailbox().size(); i++) {
            for(unsigned long int j=0; j<uTable[uid]->uMailbox()[i]->uFolder().size(); j++) {
                res += uTable[uid]->uMailbox()[i]->uFolder()[j]->uMail().size();
            }
        }
        break;
    default:
        qDebug() << "kernel - getUniId - wrong type";
        return res;
    }
    qDebug() << "Generate UniId - for " << type << "  >> " << res;
    return res;
}
