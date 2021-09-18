#pragma once

#include <QString>

class Field {
public:
    virtual QString toString() const = 0;
    virtual std::string toStdString() const = 0;
};
