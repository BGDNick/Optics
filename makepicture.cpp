#include "makepicture.hpp"
#include "ui_makepicture.h"

MakePicture::MakePicture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MakePicture)
{
    ui->setupUi(this);
    this->setFixedSize(640,440);
    ui->graphicsViewColor->setFixedSize(30,30);
    ui->graphicsViewPicture->setFixedSize(440,440);

    ui->graphicsViewColor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsViewColor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff );

    ui->graphicsViewPicture->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsViewPicture->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff );

    colorScene = std::make_unique<QGraphicsScene>(this);
    drawingScene = std::make_unique<QGraphicsScene>(this);
    ui->graphicsViewColor->setScene(colorScene.get());
    ui->graphicsViewPicture->setScene(drawingScene.get());

    drawingScene.get()->addRect(0, 0, 440, 440);
}

MakePicture::~MakePicture()
{
    delete ui;
}

void MakePicture::on_buttonBox_accepted()
{
    this->setVisible(false);
}


void MakePicture::on_spinBoxR_valueChanged(int arg1)
{
    size_t red = ui->spinBoxR->text().toInt();
    size_t blue = ui->spinBoxB->text().toInt();
    size_t green = ui->spinBoxG->text().toInt();
    colorScene.get()->clear();
    colorScene.get()->addRect(0, 0, 30, 30,QPen(QColor(red, green, blue)), QBrush(QColor(red, green, blue)));
}

void MakePicture::on_spinBoxG_valueChanged(int arg1)
{
    size_t red = ui->spinBoxR->text().toInt();
    size_t blue = ui->spinBoxB->text().toInt();
    size_t green = ui->spinBoxG->text().toInt();
    colorScene.get()->clear();
    colorScene.get()->addRect(0, 0, 30, 30,QPen(QColor(red, green, blue)), QBrush(QColor(red, green, blue)));
}

void MakePicture::on_spinBoxB_valueChanged(int arg1)
{
    size_t red = ui->spinBoxR->text().toInt();
    size_t blue = ui->spinBoxB->text().toInt();
    size_t green = ui->spinBoxG->text().toInt();
    colorScene.get()->clear();
    colorScene.get()->addRect(0, 0, 30, 30,QPen(QColor(red, green, blue)), QBrush(QColor(red, green, blue)));
}

void MakePicture::mouseMoveEvent(QMouseEvent *event)
{
    //MakePicture::QWidget::mouseMoveEvent(event);
    std::cout << event->x() << " " << event->y() << std::endl;
}

void MakePicture::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    //QWidget::mousePressEvent(event);
    if(((x<0)||(x>440)||(y<0)||(y>440)))
    {
        std::cout << false << std::endl;
        std::cout << event->x() << " " << event->y() << std::endl;
        return;
    }
    drawingScene.get()->clear();
    int arg1 = ui->spinBox->text().toInt();

    for(int i = 0; i < arg1; i++)
    {
        for(int j = 0; j < arg1; j ++)
        {
            if(QRect(0 + i * 440 / arg1, 0 + j * 440 / arg1, 440 / arg1, 440 / arg1).contains(event->x(), event->y()))
            {
                size_t red = ui->spinBoxR->text().toInt();
                size_t blue = ui->spinBoxB->text().toInt();
                size_t green = ui->spinBoxG->text().toInt();

                drawingScene.get()->addRect(0 + i * 440 / arg1, 0 + j * 440 / arg1, 440 / arg1, 440 / arg1,
                                            QPen(QColor(red, green, blue)), QBrush(QColor(red, green, blue)));
            }
            else
            {
                drawingScene.get()->addRect(0 + i * 440 / arg1, 0 + j * 440 / arg1, 440 / arg1, 440 / arg1);
            }

        }

    }

}

void MakePicture::mouseReleaseEvent(QMouseEvent *event)
{
    //QWidget::mouseReleaseEvent(event);
}

void MakePicture::on_spinBox_valueChanged(int arg1)
{
    drawingScene.get()->clear();
    for(int i = 0; i < arg1; i++)
    {
        for(int j = 0; j < arg1; j ++)
        {
            drawingScene.get()->addRect(0 + i * 440 / arg1, 0 + j * 440 / arg1, 440 / arg1, 440 / arg1);
        }

    }
}
