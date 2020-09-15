#include "rectangle.hpp"
// описание Rectangle

Rect::Rect()
{
    coord1.first = 0;
    coord1.second = 0;
    coord2.first = 0;
    coord2.second = 0;
}

Rect::Rect(std::pair<double, double> p1, std::pair<double, double> p2)
{
    coord1.first = p1.first;
    coord1.second = p1.second;
    coord2.first = p2.first;
    coord2.second = p2.second;
}

QJsonObject Rect::toJsonObject() const
{
    QJsonObject jObj;

    QJsonArray obj;
    obj = {coord1.first, coord1.second /2 + coord2.second / 2, 0};
    jObj.insert("centre", QJsonValue::fromVariant(obj));
    jObj.insert("l", QJsonValue::fromVariant(coord1.second - coord2.second));
    jObj.insert("x1", QJsonValue::fromVariant(coord1.first));
    jObj.insert("y1", QJsonValue::fromVariant(coord1.second));
    jObj.insert("x2", QJsonValue::fromVariant(coord2.first));
    jObj.insert("y2", QJsonValue::fromVariant(coord2.second));
    jObj.insert("reflection", QJsonValue::fromVariant(reflection));
    jObj.insert("name", QJsonValue::fromVariant(name));

    return jObj;

}

Rect Rect::fromJsonObject(QJsonObject jObj)
{
    this->coord1.first = jObj.value("x1").toDouble();
    this->coord1.second = jObj.value("y1").toDouble();
    this->coord2.first = jObj.value("x2").toDouble();
    this->coord2.second = jObj.value("y2").toDouble();
    this->reflection = jObj.value("reflection").toDouble();
    this->name = jObj.value("name").toString();

    return *this;
}

QBrush Rect::defineQBrush() const
{
    if(reflection < 30)
    {
        return QBrush(QColorConstants::Black);
    }
    else if (reflection > 70)
    {
        return QBrush(QColorConstants::LightGray);
    }
    else
    return QBrush(QColorConstants::DarkGray);
}

QPen Rect::defineQPen() const
{
    if(reflection < 30)
    {
        return QPen(QColorConstants::Black);
    }
    else if (reflection > 70)
    {
        return QPen(QColorConstants::LightGray);
    }
    else
    return QPen(QColorConstants::DarkGray);
}
