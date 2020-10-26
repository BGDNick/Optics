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

void Pixel::setPixel1(double _width, double _height, double _x, double _y, size_t _definition)
{
    this->width = _width;
    this->height = _height;
    this->x = _x;
    this->y = _y;
    this->definition = _definition;
}

void Pixel::print()
{
    std::cout << "(" << x << " " << y <<") " << width    << " " << height <<std::endl;
}

Pixel Pixel::fromJsonObject(QJsonObject jObj)
{

    this->r = jObj.value("r").toInt();
    this->g = jObj.value("g").toInt();
    this->b = jObj.value("b").toInt();
    this->definition = jObj.value("definition").toInt();
    this->x = jObj.value("z").toDouble();
    this->y = jObj.value("y").toDouble();
    this->z = jObj.value("x").toDouble();
    this->width = jObj.value("width").toDouble();
    this->height = jObj.value("height").toDouble();

    return *this;
}

QJsonObject Pixel::toJsonObject() const
{
    QJsonObject jObj;

    QJsonArray RGB;
    QJsonArray pos;

    RGB.append(QJsonValue::fromVariant(r));
    RGB.append(QJsonValue::fromVariant(g));
    RGB.append(QJsonValue::fromVariant(b));

    pos.append(QJsonValue::fromVariant(x));
    pos.append(QJsonValue::fromVariant(y));
    pos.append(QJsonValue::fromVariant(z));

    jObj.insert("definition", QJsonValue::fromVariant(definition));
    jObj.insert("width", QJsonValue::fromVariant(width));
    jObj.insert("height", QJsonValue::fromVariant(height));
    jObj.insert("centre", pos);
    jObj.insert("rgb", RGB);

    return jObj;
}

void Pixel::sumColor(int _r, int _g, int _b)
{
    if((r + _r) >=255)
    {
        this->r = 255;
    }
    else
    {
        this->r = r + _r;
    }

    if((g + _g) >=255)
    {
        this->g = 255;
    }
    else
    {
        this->g = g + _g;
    }

    if((b + _b) >=255)
    {
        this->b = 255;
    }
    else
    {
        this->b = b + _b;
    }
}

void Pixel::setColor(int _r, int _g, int _b)
{
    this->r = _r;
    this->g = _g;
    this->b = _b;
}
