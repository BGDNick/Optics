#include "line.hpp"

// описание Line
Line::Line()
{
    x1 = 0;
    x2 = 0;
    y1 = 0;
    y2 = 0;
    z1 = 0;
    z2 = 0;

    rgb["r"] = 0;
    rgb["g"] = 0;
    rgb["b"] = 0;
}

Line::Line(double _x1, double _y1, double _z1, double _x2, double _y2, double _z2, QMap<QString, int> i_rgb):
    x1(_x1), x2(_x2),
    y1(_y1), y2(_y2),
    z1(_z1), z2(_z2)
{
    rgb = i_rgb;
}


QJsonObject Line::toJsonObject() const
{

    QJsonObject jObj;
    QJsonArray begin;
    QJsonArray end;
    QJsonArray RGB;

    //создание конченого и начального значения
    begin.append(QJsonValue::fromVariant(x1));
    begin.append(QJsonValue::fromVariant(y1));
    begin.append(QJsonValue::fromVariant(z1));

    end.append(QJsonValue::fromVariant(x2));
    end.append(QJsonValue::fromVariant(y2));
    end.append(QJsonValue::fromVariant(z2));

    RGB.append(QJsonValue::fromVariant(rgb["r"]));
    RGB.append(QJsonValue::fromVariant(rgb["g"]));
    RGB.append(QJsonValue::fromVariant(rgb["b"]));

    jObj.insert("begin", begin);
    jObj.insert("end", end);
    jObj.insert("rgb", RGB);
    return jObj;

}

Line Line::fromJsonObject(QJsonObject jObj)
{
    QJsonArray begin;
    QJsonArray end;
    QJsonArray rgb;

    begin = jObj.value("begin").toArray();
    end = jObj.value("end").toArray();
    rgb = jObj.value("rgb").toArray();

    this->x1 = begin[0].toDouble();
    this->y1 = begin[1].toDouble();
    this->z1 = begin[2].toDouble();

    this->x2 = end[0].toDouble();
    this->y2 = end[1].toDouble();
    this->z2 = end[2].toDouble();

    this->rgb["r"] = rgb[0].toInt();
    this->rgb["g"] = rgb[1].toInt();
    this->rgb["b"] = rgb[2].toInt();

    return *this;
}

bool operator==(Line l1, Line l2)
{
    if(     (l1.x1 == l2.x1)&&
            (l1.x2 == l2.x2)&&
            (l1.y1 == l2.y1)&&
            (l1.y2 == l2.y2)&&
            (l1.z1 == l2.z1)&&
            (l1.z2 == l2.z2))
    {
        return true;
    }
    return false;
}

void Line::print()
{
    std::cout << x1 << "  " << y1 << "  " << z1 << std::endl;
    std::cout << x2 << "  " << y2 << "  " << z2 << std::endl;
    std::cout << rgb["r"] << "  " << rgb["g"] << "  " << rgb["b"] << std::endl;
}
