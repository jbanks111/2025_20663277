#include "optiondialog.h"
#include "ui_optiondialog.h"
#include <QString>
#include <QList>
#include <QVariant>

optionDialog::optionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::optionDialog)
{
    ui->setupUi(this);
}

optionDialog::~optionDialog()
{
    delete ui;
}


void optionDialog::setColourB(int B) {
    ui->spinBoxB->setValue(B);
}
int optionDialog::getColourB() {
    return ui->spinBoxB->value();
}
void optionDialog::setColourR(int R) {
    ui->spinBoxR->setValue(R);
}
int optionDialog::getColourR() {
    return ui->spinBoxR->value();
}
void optionDialog::setColourG(int G) {
    ui->spinBoxG->setValue(G);
}
int optionDialog::getColourG() {
    return ui->spinBoxG->value();
}
void optionDialog::setPartVisible(bool I) {
    ui->checkBoxVisible->setChecked(I);
}
bool optionDialog::getPartVisible() {
    return ui->checkBoxVisible->isChecked();
}
void optionDialog::setName(QString N) {
    ui->LineEditName->setText(N);
}
QString optionDialog::getName() {
    return ui->LineEditName->text();
}