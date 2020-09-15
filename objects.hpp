#ifndef OBJECTS_HPP
#define OBJECTS_HPP
#include "IncludeAll.hpp"

class Object
{

public:
    Object();

    std::pair<double, double> coord1;

    virtual QJsonObject toJsonObject() const;
};

#endif // OBJECTS_HPP
