#include "_syrah_folder.h"



map<unsigned long int, _syrah_mail*>& _syrah_folder::uMail() { return userMail; }
QString _syrah_folder::SqlWriteFolder(unsigned long boxid) {
  QString sql("");
  QVariant v;
  qlonglong q;

  sql.append("(");
  sql.append("'"); q = Id; v = q; sql.append(v.toString()); sql.append("', ");
  sql.append("'"); q = boxid; v = q; sql.append(v.toString()); sql.append("', ");
  sql.append("'"); sql.append(folderName); sql.append("', ");
  sql.append("'"); v = FolderSecurity; sql.append(v.toString()); sql.append("', ");
  sql.append("'"); sql.append(AccessCode); sql.append("' ");
  sql.append(")");

  return sql;
}
