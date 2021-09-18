#pragma once

#include "field.hpp"
#include <QString>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QTimeZone>
#include <QRegularExpression>

static qint64 SEC_IN_MICRO = 1000000L;
static qint64 MILLI_IN_MICRO = 1000L;
static qint64 DAY_IN_MICRO = 86400000000L;
static qint64 HOUR_IN_MICRO = 3600000000L;
static qint64 MINUTE_IN_MICRO = 60000000L;

static QMap<QString, QRegularExpression> gmtFormats {{"%j-%H:%M:%S-%f.%f", QRegularExpression("^(?<j>[0-3][0-9][0-9])-(?<H>[0-2][0-9]):(?<M>[0-5][0-9]):(?<S>[0-5][0-9])-(?<f>[0-9]{3}.[0-9]{3})$", QRegularExpression::NoPatternOption)},
                                                     {"%j-%H:%M:%S-%f", QRegularExpression("^(?<j>[0-3][0-9]{2})-(?<H>[0-2][0-9]):(?<M>[0-5][0-9]):(?<S>[0-5][0-9])-(?<f>[0-9]{3})$", QRegularExpression::NoPatternOption)},
                                                     {"%j-%H:%M:%S:%f", QRegularExpression("^(?<j>[0-3][0-9]{2})-(?<H>[0-2][0-9]):(?<M>[0-5][0-9]):(?<S>[0-5][0-9]):(?<f>[0-9]{3})$", QRegularExpression::NoPatternOption)},
                                                     {"%j-%H:%M:%S", QRegularExpression("^(?<j>[0-3][0-9]{2})-(?<H>[0-2][0-9]):(?<M>[0-5][0-9]):(?<S>[0-5][0-9])$", QRegularExpression::NoPatternOption)},
                                                     {"hh:mm:ss:zzz", QRegularExpression("^(?<H>[0-2][0-9]):(?<M>[0-5][0-9]):(?<S>[0-5][0-9]):(?<f>[0-9]{3})$", QRegularExpression::NoPatternOption)}};
QString findGmtFormat(const QString &gmtFormat);

class GmtField : public Field {

public:
    // constructors
    GmtField() = default;
    explicit GmtField(qint64 value, int year=1970);

    // destructor
    ~GmtField() = default;

    // classmethods
    static GmtField fromDateTime(const QDateTime &dt);
    static GmtField fromString(QString &gmt, int year=1970);

    // Getter & Setter
    [[nodiscard]] int getYear() const;
    void setYear(int year);
    [[nodiscard]] qint64 getValue() const;

    // methods
    [[nodiscard]] QDateTime toDateTime(const QTimeZone &tz=QTimeZone::utc()) const;
    [[nodiscard]] qint64 toTimeStamp() const;
    [[nodiscard]] QString toString() const override;
    [[nodiscard]] std::string toStdString() const override;
    [[nodiscard]] QDate getDate() const;
    [[nodiscard]] QTime getTime() const;

    // operators
    bool operator==(const GmtField &rhs) const;
    bool operator!=(const GmtField &rhs) const;

private:
    long long int _value = 0;
    int _year = 1970;
};
