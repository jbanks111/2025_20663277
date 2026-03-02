#include "optiondialog.h"
#include "ui_optiondialog.h"

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
