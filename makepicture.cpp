#include "makepicture.hpp"
#include "ui_makepicture.h"

MakePicture::MakePicture(QWidget *parent):
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
    std::vector<Pixel> first_pixel;
    first_pixel.push_back(Pixel(0, 0, 0, 1, 255, 255, 255, 440, 440));
    pixels.push_back(first_pixel);
    pixels.back().back().drawPixel(drawingScene.get());
    std::cout << "MakePicture" << std::endl;
}

MakePicture::~MakePicture()
{
    std::cout << "~MakePicture" << std::endl;
    delete ui;
}

void MakePicture::on_buttonBox_accepted()
{}


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
/*
void MakePicture::mouseMoveEvent(QMouseEvent *event)
{
    //MakePicture::QWidget::mouseMoveEvent(event);
    std::cout << event->x() << " " << event->y() << std::endl;
}
*/

void MakePicture::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();

    //проверка на то находится ли нажатие в нужной области
    if(((x<0)||(x>440)||(y<0)||(y>440)))
    {
        return;
    }

    //закрашивание нужного пикселя
    int quantity = pixels.size();
    double freq = 440 / quantity;
    int definition = ui->spinBoxDefinition->text().toInt();
    int number_x = int( x / freq);
    int number_y = int( y / freq);
    int r = ui->spinBoxR->text().toInt();
    int g = ui->spinBoxG->text().toInt();
    int b = ui->spinBoxB->text().toInt();
    Pixel new_pixel(freq * number_x, freq * number_y, 0.0, definition, r, g, b, freq, freq);
    new_pixel.drawPixel(drawingScene.get());
    pixels.at(number_x).at(number_y) = new_pixel;

}

void MakePicture::mouseReleaseEvent(QMouseEvent *event)
{
    //QWidget::mouseReleaseEvent(event);
}

void MakePicture::on_spinBox_valueChanged(int arg1)
{
    int first = pixels.size();
    int second = arg1;
    int delta = abs(second - first);
    int definition = ui->spinBoxDefinition->text().toInt();
    int quantity = arg1;
    double freq = 440 / quantity;
    drawingScene.get()->clear();
    // если разница равна нулю просто прорисовка

    if((second - first) >= 0)
    {
        // заполнение дополнительными пикселями
        for(int i = 0; i < pixels.size(); i++)
        {
            for(int j = 0; j < delta; j++)
            {
                pixels.at(i).push_back(Pixel());
            }
        }
        for(int i = 0; i < delta; i++)
        {
            pixels.push_back(std::vector<Pixel>(quantity, Pixel()));
        }

        //настройка всех пикселей и их прорисовка
        for(int i = 0; i < pixels.size(); i++)
        {
            for(int j = 0; j < pixels.size(); j++)
            {
                pixels.at(i).at(j).setPixel1(freq, freq, freq * i, freq * j, definition);
                pixels.at(i).at(j).drawPixel(drawingScene.get());
            }
        }

    }
    else
    {
        //удаление ненцужных пикселей
        for(int i = 0; i < pixels.size(); i++)
        {
            for(int j = 0; j < delta; j++)
            {
                pixels.at(i).pop_back();
            }
        }
        for(int i = 0; i < delta; i++)
        {
            pixels.pop_back();
        }

        //настройка всех пикселей и их прорисовка
        for(int i = 0; i < pixels.size(); i++)
        {
            for(int j = 0; j < pixels.size(); j++)
            {
                pixels.at(i).at(j).setPixel1(freq, freq, freq * i, freq * j, definition);
                pixels.at(i).at(j).drawPixel(drawingScene.get());
                pixels.at(i).at(j).print();
            }
        }
    }
}

void MakePicture::on_spinBoxDefinition_valueChanged(int arg1)
{

}
