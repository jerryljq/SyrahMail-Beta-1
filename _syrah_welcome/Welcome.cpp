#include "Welcome.h"
#include "ui_Welcome.h"

Welcome::Welcome(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);
}

Welcome::~Welcome()
{
    delete ui;
}

void Welcome::on_pushButton_2_clicked()
{
    this->close();
}

void Welcome::on_pushButton_clicked()
{
    this->close();
    ui2.show();
}

