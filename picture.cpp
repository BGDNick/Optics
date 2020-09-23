#include "picture.hpp"
#include "ui_picture.h"

Picture::Picture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Picture)
{
    ui->setupUi(this);
}

Picture::~Picture()
{
    delete ui;
}
