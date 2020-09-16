#include "makepicture.hpp"
#include "ui_makepicture.h"

MakePicture::MakePicture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MakePicture)
{
    ui->setupUi(this);
}

MakePicture::~MakePicture()
{
    delete ui;
}
