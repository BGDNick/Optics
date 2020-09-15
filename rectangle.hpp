#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "objects.hpp"

// прямоугольник
class Rect: public Object
{
public:
    Rect();
    Rect(std::pair<double, double> p1, std::pair<double, double> p2);

    QJsonObject toJsonObject() const;
    Rect fromJsonObject(QJsonObject jObj);
    QBrush defineQBrush() const;
    QPen defineQPen() const;

public:
    std::pair<double, double> coord2;
    double reflection = 0; // коэффициент отражения
    QString name = "Rectangle";
};

#endif // RECTANGLE_HPP
