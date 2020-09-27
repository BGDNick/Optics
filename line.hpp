#ifndef LINE_HPP
#define LINE_HPP
#include <IncludeAll.hpp>
#include <objects.hpp>
#include <iostream>


class Line
{
public:
    Line();

    friend bool operator==(Line l1, Line l2);

    Line(double x1, double y1, double z1, double x2, double y2, double z2, QMap<QString, int> i_rgb);

    QJsonObject toJsonObject() const;

    Line fromJsonObject(QJsonObject jObj);

    void print();

public:
    double x1;
    double y1;
    double z1;

    double x2;
    double y2;
    double z2;
    QMap<QString, int> rgb;
};

#endif // LINE_HPP
