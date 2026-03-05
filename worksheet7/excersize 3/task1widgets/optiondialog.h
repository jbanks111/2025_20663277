#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QVariant>

namespace Ui {
class optionDialog;
}

class optionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit optionDialog(QWidget *parent = nullptr);
    ~optionDialog();

    void setColourB(int B);
    int getColourB();
    void setColourR(int R);
    int getColourR();
    void setColourG(int G);
    int getColourG();
    void setPartVisible(bool I);
    bool getPartVisible();
    void setName(QString N);
    QString getName();
private:
    Ui::optionDialog *ui;
};

#endif // OPTIONDIALOG_H
