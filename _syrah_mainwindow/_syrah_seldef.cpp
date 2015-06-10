#include "_syrah_seldef.h"
#include "ui__syrah_seldef.h"
#include "_syrah_dbset/_syrah_kernel.h"
#include "_syrah_dbset/_syrah_user.h"
#include "_syrah_dbset/mailbox/_syrah_mailbox.h"

extern _syrah_kernel _syrah_sys;
extern vector <_syrah_user*> _syrah_userTable;
vector<_syrah_user*>& uTable = _syrah_userTable;

_syrah_seldef::_syrah_seldef(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::_syrah_seldef)
{
    ui->setupUi(this);

    for(unsigned long int i = 0; i < uTable[_syrah_sys.getCurrentUserID()]->uMailbox().size(); i++) {
        ui->listWidget->addItem(uTable[_syrah_sys.getCurrentUserID()]->uMailbox()[i]->getUser());
    }

    connect(ui->listWidget, SIGNAL(currentTextChanged(QString)), this, SLOT(changefinish(QString)));
}

_syrah_seldef::~_syrah_seldef()
{
    delete ui;
}

void _syrah_seldef::on_pushButton_2_clicked()
{
    this->close();
}

void _syrah_seldef::changefinish(QString boxName)
{
    emit passtomain(boxName);
}
