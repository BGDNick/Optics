#ifndef LENS_HPP
#define LENS_HPP

#include "IncludeAll.hpp"

class Lens
{
public:
    Lens();

    explicit Lens(std::pair<double, double> i_coord, std::pair<double,double> i_radius,
         double i_lenght, double i_r_index, QString i_type, double angle, double i_width);

    QJsonObject toJsonObject() const;

    Lens fromJsonObject(QJsonObject jObj);

    void draw(QGraphicsScene* scene) const;

public:
    std::pair<double, double> coord;
    std::pair<double, double> radius;
    double length;
    double width;
    double r_index;
    double angle;
    double height = 0;
    QString name = "Lens";
    //QString type = "Flat";

    //QString type_flat = "Flat";
    //QString type_convex = "Convex";

};
#endif // LENS_HPP

