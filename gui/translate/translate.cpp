#include "translate.h"
#include "ui_translate.h"

translate::translate(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::translate)
{
    ui->setupUi(this);
}

translate::~translate()
{
    delete ui;
}
