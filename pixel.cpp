#include "pixel.hpp"

Pixel::Pixel():
    r(255), g(255), b(255),
    definition(1),
    x(0), y(0), z(0),
    width(0), height(0)
{

}
Pixel::Pixel(double _x, double _y, double _z, size_t _definition, int _r, int _g, int _b, double _width, double _height):
    r(_r), g(_g), b(_b),
    definition(_definition),
    x(_x), y(_y), z(_z),
    width(_width), height(_height)
{

}

void Pixel::drawPixel(QGraphicsScene *_scene)
{
    _scene->addRect(x, y, width, height, QPen(QColor(r, g, b)), QBrush(QColor(r, g, b)));
}

void Pixel::setSize(double _width, double _height)
{
    this->width = _width;
    this->height = _height;
}
