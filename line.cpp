#include "line.hpp"

// описание Line
Line::Line()
{
    coord1.first = 0.0;
    coord2.first = 0.0;
    coord1.second = 0.0;
    coord2.second = 0.0;
    rgb["r"] = 0;
    rgb["g"] = 0;
    rgb["b"] = 0;
}

Line::Line(std::pair<double, double> p1, std::pair<double, double> p2, QMap<QString, int> i_rgb):
    coord2(p2)
{
    coord1 = p1;
    rgb = i_rgb;
}


QJsonObject Line::toJsonObject() const
{
    QJsonObject jObj;
    jObj.insert("x1", QJsonValue::fromVariant(coord1.first));
    jObj.insert("y1", QJsonValue::fromVariant(coord1.second));
    jObj.insert("x2", QJsonValue::fromVariant(coord2.first));
    jObj.insert("y2", QJsonValue::fromVariant(coord2.second));
    jObj.insert("r", QJsonValue::fromVariant(rgb["r"]));
    jObj.insert("g", QJsonValue::fromVariant(rgb["g"]));
    jObj.insert("b", QJsonValue::fromVariant(rgb["b"]));

    return jObj;

}

Line Line::fromJsonObject(QJsonObject jObj)
{
    this->coord1.first = jObj.value("x1").toDouble();
    this->coord1.second = jObj.value("y1").toDouble();
    this->coord2.first = jObj.value("x2").toDouble();
    this->coord2.second = jObj.value("y2").toDouble();
    this->rgb["r"] = jObj.value("r").toInt();
    this->rgb["g"] = jObj.value("g").toInt();
    this->rgb["b"] = jObj.value("b").toInt();

    return *this;
}

bool operator==(Line l1, Line l2)
{
    if(     (l1.coord1.first == l2.coord1.first)&&
            (l1.coord2.first == l2.coord2.first)&&
            (l1.coord1.second == l2.coord1.second)&&
            (l1.coord2.second == l2.coord2.second))
    {
        return true;
    }
    return false;
}
