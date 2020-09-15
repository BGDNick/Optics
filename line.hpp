#ifndef LINE_HPP
#define LINE_HPP
#include <IncludeAll.hpp>
#include <objects.hpp>


class Line: public Object
{
public:
    Line();

    friend bool operator==(Line l1, Line l2);

    Line(std::pair<double, double> p1, std::pair<double, double> p2, QMap<QString, int> i_rgb);

    QJsonObject toJsonObject() const;
    Line fromJsonObject(QJsonObject jObj);

public:
    std::pair<double, double> coord2;
    QMap<QString, int> rgb;
};

#endif // LINE_HPP
