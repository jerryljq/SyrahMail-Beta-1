#ifndef _SYRAH_SELDEF_H
#define _SYRAH_SELDEF_H

#include <QWidget>

namespace Ui {
class _syrah_seldef;
}

class _syrah_seldef : public QWidget
{
    Q_OBJECT

public:
    explicit _syrah_seldef(QWidget *parent = 0);
    ~_syrah_seldef();
public slots:
    void changefinish(QString boxName);

private slots:
    void on_pushButton_2_clicked();

signals:

    void passtomain(QString);

private:
    Ui::_syrah_seldef *ui;
};

#endif // _SYRAH_SELDEF_H
