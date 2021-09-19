#include "gmtfield.hpp"
#include "unsupportedformat.hpp"
#include <QDebug>

GmtField::GmtField(qint64 value, int year): _value(value), _year(year) {
}

QDateTime GmtField::toDateTime(const QTimeZone &tz) const {
    return {getDate(), getTime(), tz};
}

GmtField GmtField::fromString(QString &gmt, int year) {
    // check that string format is compliant with one of the supported formats
    QString gmtFormat = findGmtFormat(gmt);
    if (gmtFormat.isEmpty())
        throw UnsupportedFormat();

    // Apply regex and init nbMicroSeconds to 0
    QRegularExpression regex = gmtFormats.value(gmtFormat);
    QRegularExpressionMatch matchResult = regex.match(gmt);
    qint64 nbMicroSeconds = 0;

    // check if format contains a nb of year's day.
    if (gmtFormat.contains("%j")) {
        int nbDays = matchResult.captured("j").toInt();
        if (nbDays < 1 || nbDays > 365)
            throw std::range_error("Out of range value for year's day, should be included in 1-365 range.");
        nbMicroSeconds += (nbDays - 1) * DAY_IN_MICRO;
    }

    // check if format contains a nb of hours.
    if (gmtFormat.contains("%H") || gmtFormat.contains("hh"))
        nbMicroSeconds += matchResult.captured("H").toInt() * HOUR_IN_MICRO;

    // check if format contains a nb of minutes.
    if (gmtFormat.contains("%M") || gmtFormat.contains("mm"))
        nbMicroSeconds += matchResult.captured("M").toInt() * MINUTE_IN_MICRO;

    // check if format contains a nb of seconds.
    if (gmtFormat.contains("%S") || gmtFormat.contains("ss"))
        nbMicroSeconds += matchResult.captured("S").toInt() * SEC_IN_MICRO;

    if (gmtFormat.contains("%f") || gmtFormat.contains("zzz")) {
        // specific case for microseconds separated from milliseconds separated by a `.`
        QString milliSecondsString = matchResult.captured("f");
        if (milliSecondsString.contains(".")) {
            // case f is a number of microseconds
            milliSecondsString = milliSecondsString.replace(QLatin1String("."), QLatin1String(""));
            nbMicroSeconds += milliSecondsString.toInt();
        } else {
            // case f is a number of milliseconds
            nbMicroSeconds += milliSecondsString.toInt() * MILLI_IN_MICRO;
        }
    }

    return GmtField(nbMicroSeconds, year);
}

QString GmtField::toString() const {
    int yd = int(_value / DAY_IN_MICRO + 1);
    qint64 rest = _value % DAY_IN_MICRO;

    int hh = int(rest / HOUR_IN_MICRO);
    rest %= HOUR_IN_MICRO;

    int mm = int(rest / MINUTE_IN_MICRO);
    rest %= MINUTE_IN_MICRO;

    int ss = int(rest / SEC_IN_MICRO);
    rest %= SEC_IN_MICRO;

    int ms = int(rest / MILLI_IN_MICRO);

    int mms = int(rest % MILLI_IN_MICRO);

    if (mms == 0) {
        if (ms == 0) {
            return QString("%1-%2:%3:%4").arg(yd, 3, 10, QChar('0')).arg(hh, 2, 10, QChar('0')).arg(mm, 2, 10, QChar('0')).arg(ss, 2, 10, QChar('0'));
        } else {
            return QString("%1-%2:%3:%4-%5").arg(yd, 3, 10, QChar('0')).arg(hh, 2, 10, QChar('0')).arg(mm, 2, 10, QChar('0')).arg(ss, 2, 10, QChar('0')).arg(ms, 3, 10, QChar('0'));
        }
    } else {
        return QString("%1-%2:%3:%4-%5.%6").arg(yd, 3, 10, QChar('0')).arg(hh, 2, 10, QChar('0')).arg(mm, 2, 10, QChar('0')).arg(ss, 2, 10, QChar('0')).arg(ms, 3, 10, QChar('0')).arg(mms, 3, 10, QChar('0'));
    }
}

std::string GmtField::toStdString() const {
    return toString().toStdString();
}

GmtField GmtField::fromDateTime(const QDateTime &dt) {
    if (!dt.isValid())
        return {};
    int year = dt.date().year();
    qint64 value = dt.toMSecsSinceEpoch() - QDateTime(QDate(year, 1, 1), QTime(0,0,0), dt.timeZone()).toMSecsSinceEpoch();
    // as QDateTime has a precision of 1 millisecond we need to multiply the value by 1000 to convert into microsecond
    value *= 1000;
    return GmtField(value, year);
}

qint64 GmtField::toTimeStamp() const {
    // TODO
    return _value;
}

int GmtField::getYear() const {
    return _year;
}

void GmtField::setYear(int year) {
    _year = year;
}

QDate GmtField::getDate() const {
    int yd = int(_value / DAY_IN_MICRO);
    return QDate(getYear(), 1, 1).addDays(yd);
}

QTime GmtField::getTime() const {
    qint64 rest = _value % DAY_IN_MICRO;

    int hh = int(rest / HOUR_IN_MICRO);
    rest %= HOUR_IN_MICRO;

    int mm = int(rest / MINUTE_IN_MICRO);
    rest %= MINUTE_IN_MICRO;

    int ss = int(rest / SEC_IN_MICRO);
    rest %= SEC_IN_MICRO;

    int ms = int(rest / MILLI_IN_MICRO);

    return {hh, mm, ss, ms};
}

qint64 GmtField::getValue() const {
    return _value;
}

QString findGmtFormat(const QString &gmtFormat) {
    QMapIterator<QString, QRegularExpression> itGmtFormats(gmtFormats);
    while (itGmtFormats.hasNext()) {
        itGmtFormats.next();
        if (itGmtFormats.value().match(gmtFormat, QRegularExpression::NormalMatch).hasMatch())
            return itGmtFormats.key();
    }
    return {};
}

bool GmtField::operator==(const GmtField &rhs) const {
    return _value == rhs._value && _year == rhs._year;
}

bool GmtField::operator!=(const GmtField &rhs) const {
    return !(rhs == *this);
}