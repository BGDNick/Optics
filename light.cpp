#include "light.hpp"

Light::Light()
{
    x1 = 0.0;
    y1 = 0.0;

    power = 100.0;
    name = "Light";
}

Light::Light(QString name_i, double power_i, double x1_i, double y1_i, double x2_i, double y2_i):
    name(name_i), power(power_i), x1(x1_i), y1(y1_i), x2(x2_i), y2(y2_i)
{

}

QBrush Light::defineQBrush() const
{
    if(power < 30)
    {
        return QBrush(QColorConstants::Svg::goldenrod);
    }
    else if (power > 70)
    {
        return QBrush(QColorConstants::Svg::lightyellow);
    }
    else
    return QBrush(QColorConstants::Svg::gold);
}

QPen Light::defineQPen() const
{
    if(power < 30)
    {
        return QPen(QColorConstants::Svg::goldenrod);
    }
    else if (power > 70)
    {
        return QPen(QColorConstants::Svg::lightyellow);
    }
    else
    return QPen(QColorConstants::Svg::gold);
}

Light Light::fromJsonObject(QJsonObject jObj)
{
    QJsonArray centr;
    centr = { x1, y1, 0};
    this->name = jObj.value("name").toString();
    this->power = jObj.value("power").toDouble();
    this->x1 = jObj.value("x1").toDouble();
    this->y1 = jObj.value("y1").toDouble();

    this->x2 = jObj.value("x2").toDouble();
    this->y2 = jObj.value("y2").toDouble();
    this->type = jObj.value("type").toBool();

    return *this;
}

QJsonObject Light::toJsonObject() const// поправить для протяженного источника свеа
{
    QJsonObject jObj;

    QJsonArray centr;
    centr =  {x1, y1, 0};
    jObj.insert("centre", centr);

    jObj.insert("name", QJsonValue::fromVariant(name));
    jObj.insert("power", QJsonValue::fromVariant(power));
    jObj.insert("x1", QJsonValue::fromVariant(x1));
    jObj.insert("y1", QJsonValue::fromVariant(y1));

    jObj.insert("x2", QJsonValue::fromVariant(x2));
    jObj.insert("y2", QJsonValue::fromVariant(y2));
    jObj.insert("type", QJsonValue::fromVariant(type));


    return jObj;
}

void Light::draw(QGraphicsScene* scene) const
{
    QPen pen = this->defineQPen();
    QBrush brush = this->defineQBrush();
    if(!type)
    {
        scene->addEllipse(x1, y1, 1, 1, pen, brush);
    }
    else
    {
        for(int i = 0; i <= amount; ++i)
        {
            double x_new = x1 + i * (x2 - x1) / amount;
            double y_new = y1 + i * (y2 - y1) / amount;

            Light temp = *this;
            temp.type = false;
            temp.x1 = x_new;
            temp.y1 = y_new;
            temp.draw(scene);
        }
    }
}



