#include "_syrah_user.h"

_syrah_user::_syrah_user(const QString& username, const QString& password) {
    userDb.resetUserName(username);
    userDb.resetPassword(password);
    QString DbCheck = "mail/";
    DbCheck.append(username);
    DbCheck.append(".box");
    _syrah_sys._debug("user","checkTemplate",QFile::exists("mail/sysDb"));
    if(!QFile::exists(DbCheck)) userDb.setNewDb();          //这里是对于新用户的处理
    userDb.autoDatabase();
}

map<unsigned long int, _syrah_mailbox*>& _syrah_user::uMailbox() { return userMailbox; }
map<unsigned long int, _syrah_rules*>& _syrah_user::uRule() { return userRule; }
map<unsigned long int, _syrah_contacts*>& _syrah_user::uContacts() { return userContacts; }
map<QString, QVariant>& _syrah_user::uSetting() { return userSetting; }
//unsigned long int _syrah_user::getId(const QString table) { return userDb.primaryId(table); }


/*void _syrah_user::Query() {     //DEMO for querying data in table setting

    QSqlQuery query(userDb.getDb());                                //Access Approche 1
    _syrah_security inspector;

    QString SQL = "select * from setting";         //SQL query
    query.exec(inspector.SQLQueryDefence(SQL));            //Access Approche 2 [Suggest]
    while(query.next()) {
        qDebug() << query.value(0).toInt() << "|" << query.value(1).toString() << query.value(2).toString();
    }
}
*/

//To get username or password of one mailbox, execute following statements
/*QSqlQuery query(uTable[0].userDb.getDb());
_syrah_security ins;
unsigned int mailboxId;                         //the Id should be transfered into this variable
QString S = "select * from mailbox where id=";  //keyword "where" specifies the data queried from database
S.append(mailboxId);
query.exec(ins.SQLQueryDefence(S));
QString username = query.value(2);              //the index was defined in _syrah_security.h
QString password = query.value(3);
*/
