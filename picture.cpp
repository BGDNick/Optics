#include "picture.hpp"
#include "ui_picture.h"

Picture::Picture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Picture)
{
    ui->setupUi(this);
    //настройка полей
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff );
    
    //настройка сцены
    scene = std::make_unique<QGraphicsScene>(this);
    ui->graphicsView->setScene(scene.get());
    bright_level = 1;
    quantity = 1;
    position = 500;
    
}

Picture::~Picture()
{
    delete ui;
}

void Picture::calculate(QList<Line> lines)
{
    std::cout << "===========================" <<std::endl;
    std::cout << "bright level " << bright_level << std::endl;
    std::cout << "position " << position << std::endl;
    foreach(Line l, lines)
    {
        l.print();
    }
    //очстка и заполнение новыми пикселями
    pixels.clear();
    for(int i = 0; i < quantity; i++)
    {
        pixels.push_back(std::vector<Pixel>());
        for(int j = 0; j < quantity; j++)
        {
            std::cout << "i " << i << "  j " << j <<std::endl;
            pixels.at(i).push_back(Pixel());
        }
    }

    //настройка всех пикселей 
    double freq = 440 / quantity;
    for(int i = 0; i < pixels.size(); i++)
    {
        for(int j = 0; j < pixels.size(); j++)
        {
            pixels.at(i).at(j).setPixel1(freq, freq, freq * i, freq * j, 1);
            pixels.at(i).at(j).setColor(0, 0, 0);
            pixels.at(i).at(j).drawPixel(scene.get());
        }
    }
    std::cout <<"vector size: " << pixels.size() <<"  " << pixels.at(0).size() << std::endl;
    
    //закрашивание пикселей
    foreach(Line l, lines)
    {
        if(l.x1 == position)
        {
            std::cout << "first condition" << std::endl;
             int x = l.z1;
             int y = l.y1;

             //проверка на то находится ли нажатие в нужной области
             if(((x<0)||(x>440)||(y<0)||(y>440)))
             {
                 std::cout << "not in square" << std::endl;
                 std::cout << "x:  " << x << " y:  " << y << std::endl;
                 return;
             }

             //закрашивание нужного пикселя
             double freq = 440 / quantity;
             int number_x = int( x / freq);
             int number_y = int( y / freq);
             int r = l.rgb["r"] / bright_level;
             int g = l.rgb["g"] / bright_level;
             int b = l.rgb["b"] / bright_level;
             std::cout << "numbers" << number_x << "  " << number_y << std::endl;
             pixels.at(number_x).at(number_y).sumColor(r, g, b);
             pixels.at(number_x).at(number_y).drawPixel(scene.get());
             return;
        }
        else if (l.x2 == position)
        {
            std::cout << "second condition" <<std::endl;
            int x = l.z2;
            int y = l.y2;

            //проверка на то находится ли нажатие в нужной области
            if(((x<0)||(x>440)||(y<0)||(y>440)))
            {
                std::cout << "not in square" << std::endl;
                std::cout << "x:  " << x << " y:  " << y << std::endl;
                return;
            }

            //закрашивание нужного пикселя
            double freq = 440 / quantity;
            int number_x = int( x / freq);
            int number_y = int( y / freq);
            int r = l.rgb["r"] / bright_level;
            int g = l.rgb["g"] / bright_level;
            int b = l.rgb["b"] / bright_level;
            std::cout << "rgb" << r << " " << g << " " << b << std::endl;
            std::cout << "numbers" << number_x << "  " << number_y << std::endl;
            pixels.at(number_x).at(number_y).sumColor(r, g, b);
            pixels.at(number_x).at(number_y).drawPixel(scene.get());
            return;
        }
    }
}
