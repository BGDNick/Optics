#include "lens.hpp"

Lens::Lens()
{
    std::pair<double, double> coord = std::make_pair(0.0,0.0);
    std::pair<double, double> radius = std::make_pair(0.0, 0.0);
    double length = 0.0;
    double r_index = 0.0;
}

Lens::Lens(std::pair<double, double> i_coord, std::pair<double,double> i_radius, double i_length, double i_r_index,
           QString i_type, double angle_i):
    coord(i_coord), radius(i_radius), length(i_length),
    r_index(i_r_index), type(i_type), angle(angle_i)
{

}

QJsonObject Lens::toJsonObject() const
{

    QJsonObject jObj;

    QJsonArray centr = {coord.first, coord.second, 0};
    jObj.insert("centre", QJsonValue::fromVariant(centr));


    jObj.insert("x", QJsonValue::fromVariant(coord.first));
    jObj.insert("y", QJsonValue::fromVariant(coord.second));

    jObj.insert("R2", QJsonValue::fromVariant(radius.first));
    jObj.insert("R1", QJsonValue::fromVariant(-radius.second));
    jObj.insert("l", QJsonValue::fromVariant(length));
    jObj.insert("n", QJsonValue::fromVariant(r_index));
    jObj.insert("type", QJsonValue::fromVariant(type));
    jObj.insert("name", QJsonValue::fromVariant(name));
    jObj.insert("angle", QJsonValue::fromVariant(angle));
    jObj.insert("height", QJsonValue::fromVariant(height));

    return jObj;

}

Lens Lens::fromJsonObject(QJsonObject jObj)
{
    this->coord.first = jObj.value("x").toDouble();
    this->coord.second = jObj.value("y").toDouble();
    this->radius.first = jObj.value("R2").toDouble();
    this->radius.second = -jObj.value("R1").toDouble();
    this->length = jObj.value("l").toDouble();
    this->r_index = jObj.value("n").toDouble();
    this->type = jObj.value("type").toString();
    this->name = jObj.value("name").toString();
    this->angle = jObj.value("angle").toDouble();
    this->height = jObj.value("height").toDouble();

    return *this;
}

void Lens::draw(QGraphicsScene* scene) const
{
    double pi = 3.1415926535;

    if(type == type_flat)
    {
        //углы необходимые для посроения дуг
        double angle_left = asin((length / 2) / radius.first) ;
        double angle_right = asin((length / 2) / radius.second);


        //std::cout << "Drawing" << std::endl;
        //std::cout << coord.first << "  " << coord.second << "  " << radius.first << "  " << radius.second << "  " << length << "  " << r_index << std::endl;

        //расстояние от центра левой окружности до центра линзы
        //double a = (radius.first * radius.first + radius.first * radius.second - length * radius.first - length * radius.second + length * length)/
        //        (radius.first + radius.second - length);

        double a = radius.first * cos(angle_left);

        //расстояние от центра правой окружности до центра линзы
        //double b = (radius.second * radius.second + radius.first * radius.second - length * radius.first - length * radius.second) /
        //        (radius.first + radius.second - length);
        double b = radius.second * cos(angle_right);

        //координаты левого и правого центров окружностей
        std::pair<double, double> coord_left = std::pair <double, double> (coord.first - a * cos(pi * angle / 180), coord.second + a * sin(pi * angle / 180));

        std::pair<double, double> coord_right = std::pair<double, double> (coord.first + b * cos(pi * angle / 180), coord.second - b * sin(pi * angle /180));


        //начинаем создавать дугу

        //переводим перо в центр левой окружности
        //path.moveTo(coord_left.first, coord_left.second);
        //начинаем прорисовывать левую дугу
        //path.arcTo(coord_left.first - radius.first, coord_left.second + radius.first, radius.first, radius.first, angle_left, 180);
        //добавление дуги на экран

        // создаем пути т.к. нет элемента "дуга"
        QPainterPath path_left;
        QPainterPath path_right;

        // первая дуга

        path_left.moveTo(coord_left.first + radius.first * cos(angle_left + pi * angle / 180) , (coord_left.second - radius.first * sin(angle_left + pi * angle / 180)));

        path_left.arcTo(coord_left.first - radius.first, coord_left.second - radius.first, 2 * radius.first, 2 * radius.first, 180 * angle_left / pi + angle, -2 * 180 * angle_left / pi);
        scene->addPath(path_left);
        //вторая дуга

        path_right.moveTo(coord_right.first - radius.second * cos(angle_right - pi * angle / 180), (coord_right.second - radius.second * sin(angle_right - pi * angle / 180)));

        path_right.arcTo(coord_right.first - radius.second, coord_right.second - radius.second, 2 * radius.second, 2 * radius.second, 180 - 180 * angle_right / pi + angle, 2 * 180 * angle_right / pi);
        scene->addPath(path_right);



    }

    if(type == type_convex)
    {
        //расстояние от центра до левой окружности
        double a = length / 2 + radius.first;
        double b = length / 2 + radius.second;

        //углы необходимые для посроения дуг
        double angle_left = asin(height / radius.first) ;
        double angle_right = asin(height / radius.second);

        //координаты правого  лвеого центров окружностей

        std::pair<double, double> coord_left = std::pair <double, double> (coord.first - a * cos(pi * angle / 180), coord.second + a * sin(pi * angle / 180));

        std::pair<double, double> coord_right = std::pair<double, double> (coord.first + b * cos(pi * angle / 180), coord.second - b * sin(pi * angle /180));


        // создаем пути т.к. нет элемента "дуга"
        QPainterPath path_left;
        QPainterPath path_right;

        //первая дуга
        path_left.moveTo(coord_left.first + radius.first * cos(angle_left + pi * angle / 180) , (coord_left.second - radius.first * sin(angle_left + pi * angle / 180)));

        path_left.arcTo(coord_left.first - radius.first, coord_left.second - radius.first, 2 * radius.first, 2 * radius.first, 180 * angle_left / pi + angle, -2 * 180 * angle_left / pi);
        scene->addPath(path_left);
        //вторая дуга

        path_right.moveTo(coord_right.first - radius.second * cos(angle_right - pi * angle / 180), (coord_right.second - radius.second * sin(angle_right - pi * angle / 180)));

        path_right.arcTo(coord_right.first - radius.second, coord_right.second - radius.second, 2 * radius.second, 2 * radius.second, 180 - 180 * angle_right / pi + angle, 2 * 180 * angle_right / pi);
        scene->addPath(path_right);

        //линии оснований толстой линзы
        scene->addLine(coord_left.first + radius.first * cos(angle_left + pi * angle / 180) , (coord_left.second - radius.first * sin(angle_left + pi * angle / 180)),
                       coord_right.first - radius.second * cos(angle_right - pi * angle / 180), (coord_right.second - radius.second * sin(angle_right - pi * angle / 180)));

        scene->addLine(coord_left.first + radius.first * cos(angle_left - pi * angle / 180) , (coord_left.second + radius.first * sin(angle_left - pi * angle / 180)),
                       coord_right.first - radius.second * cos(angle_right + pi * angle / 180), (coord_right.second + radius.second * sin(angle_right + pi * angle / 180)));




    }

}

