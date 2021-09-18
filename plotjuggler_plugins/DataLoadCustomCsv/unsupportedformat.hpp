#pragma once
#include <QException>


class UnsupportedFormat : public QException {

public:
    void raise() const override { throw *this; }
    [[nodiscard]] UnsupportedFormat *clone() const override { return new UnsupportedFormat(*this); }
};
