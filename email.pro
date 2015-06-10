#-------------------------------------------------
#
# Project created by QtCreator 2014-12-07T10:51:10
#
#-------------------------------------------------

QT       += core gui sql network webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = email
TEMPLATE = app


SOURCES += main.cpp \
    _syrah_account_setup/mailaccconfig.cpp \
    _syrah_account_setup/smtplogin.cpp \
    _syrah_dbset/encode/_syrah_encode_md5.cpp \
    _syrah_dbset/encode/_syrah_encode_xor.cpp \
    _syrah_dbset/mailbox/_syrah_contacts.cpp \
    _syrah_dbset/mailbox/_syrah_folder.cpp \
    _syrah_dbset/mailbox/_syrah_mail.cpp \
    _syrah_dbset/mailbox/_syrah_mailbox.cpp \
    _syrah_dbset/mailbox/_syrah_rules.cpp \
    _syrah_dbset/_syrah_database.cpp \
    _syrah_dbset/_syrah_encode.cpp \
    _syrah_dbset/_syrah_kernel.cpp \
    _syrah_dbset/_syrah_security.cpp \
    _syrah_dbset/_syrah_user.cpp \
    _syrah_logon/userlogon.cpp \
    _syrah_mainwindow/mainwindow.cpp \
    _syrah_network/smtpclient.cpp \
    _syrah_user_setup/usersetup.cpp \
    _syrah_welcome/Welcome.cpp \
    _syrah_write_mail/emailaddress.cpp \
    _syrah_write_mail/mailwindow.cpp \
    _syrah_write_mail/quotedptrintable.cpp \
    _syrah_network/mimecontentformatter.cpp \
    _syrah_network/mimehtml.cpp \
    _syrah_network/mimemessage.cpp \
    _syrah_network/mimepart.cpp \
    _syrah_network/mimetext.cpp \
    _syrah_receiver/_Syarh_pop3_Receiver.cpp \
    _syrah_receiver/_syrah_mail_received.cpp \
    _syrah_receiver/_syrah_mime_decoder.cpp \
    _syrah_receiver/_Syrah_pop3_Ssl_Receiver.cpp \
    _syrah_write_mail/mimeattachment.cpp \
    _syrah_write_mail/mimefile.cpp \
    _syrah_write_mail/mimemultipart.cpp \
    _syrah_mainwindow/_syrah_seldef.cpp

HEADERS  += _syrah_account_setup/mailaccconfig.h \
    _syrah_account_setup/smtplogin.h \
    _syrah_dbset/encode/_syrah_encode_md5.h \
    _syrah_dbset/encode/_syrah_encode_xor.h \
    _syrah_dbset/mailbox/_syrah_contacts.h \
    _syrah_dbset/mailbox/_syrah_folder.h \
    _syrah_dbset/mailbox/_syrah_mail.h \
    _syrah_dbset/mailbox/_syrah_mailbox.h \
    _syrah_dbset/mailbox/_syrah_rules.h \
    _syrah_dbset/_syrah_database.h \
    _syrah_dbset/_syrah_encode.h \
    _syrah_dbset/_syrah_kernel.h \
    _syrah_dbset/_syrah_security.h \
    _syrah_dbset/_syrah_user.h \
    _syrah_logon/userlogon.h \
    _syrah_mainwindow/mainwindow.h \
    _syrah_network/smtpclient.h \
    _syrah_user_setup/usersetup.h \
    _syrah_welcome/Welcome.h \
    _syrah_write_mail/emailaddress.h \
    _syrah_write_mail/mailwindow.h \
    _syrah_write_mail/quotedprintable.h \
    _syrah_network/mimecontentformatter.h \
    _syrah_network/mimehtml.h \
    _syrah_network/mimemessage.h \
    _syrah_network/mimemultipart.h \
    _syrah_network/mimepart.h \
    _syrah_network/mimetext.h \
    _syrah_receiver/_Syarh_pop3_Receiver.h \
    _syrah_receiver/_syrah_mail_received.h \
    _syrah_receiver/_syrah_mime_decoder.h \
    _syrah_receiver/_Syrah_pop3_Ssl_Receiver.h \
    _syrah_write_mail/mimemultipart.h \
    _syrah_write_mail/mimeattachment.h \
    _syrah_write_mail/mimefile.h \
    _syrah_mainwindow/_syrah_seldef.h

FORMS    += \
    _syrah_account_setup/mailaccconfig.ui \
    _syrah_logon/userlogon.ui \
    _syrah_mainwindow/mainwindow.ui \
    _syrah_user_setup/usersetup.ui \
    _syrah_welcome/Welcome.ui \
    _syrah_write_mail/mailwindow.ui \
    _syrah_mainwindow/_syrah_seldef.ui

RESOURCES += \
    resource.qrc
