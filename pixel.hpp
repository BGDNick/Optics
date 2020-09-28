#ifndef PIXEL_HPP
#define PIXEL_HPP
#include "IncludeAll.hpp"
#include <iostream>

class Pixel
{
public:
    Pixel();

    Pixel(double _x, double _y, double _z, size_t _definition, int _r, int _g, int _b, double _width, double _height);

    void drawPixel(QGraphicsScene* _scene);

    void setSize(double _width, double _height);

    void setPixel1(double _width, double _height, double _x, double _y, size_t _definition);

    void print();

    void sumColor(int r, int g, int b);

    void setColor(int r, int g, int b);

    QJsonObject toJsonObject() const;

    Pixel fromJsonObject(QJsonObject jObj);

    double x;
    double y;
    double z;
    double width;
    double height;
    size_t definition;
    int r;
    int g;
    int b;

};

#endif // PIXEL_HPP
