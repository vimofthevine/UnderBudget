/*
 * Copyright 2017 Kyle Treubig
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Standard include(s)
#include <cstdint>

// Qt include(s)
#include <QDate>
#include <QStringBuilder>
#include <QtCore>

// UnderBudget include(s)
#include "Recurrence.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
Recurrence::Recurrence() : day_(0), month_(0), periodicity_(0), scope_(Yearly), week_(0) {}

//--------------------------------------------------------------------------------------------------
int32_t Recurrence::day() const {
    return day_;
}

//--------------------------------------------------------------------------------------------------
int32_t Recurrence::month() const {
    return month_;
}

//--------------------------------------------------------------------------------------------------
QDate Recurrence::nextOccurrence(const QDate & date) const {
    QDate next;
    if (periodicity_ >= 1) {
        switch (scope_) {
        case Yearly: {
            int year = date.year();
            while (true) {
                next = QDate(year, 1, 1);
                // Go to month of the year
                if ((month_ > 0) and (month_ < 13)) {
                    next = next.addMonths(month_ - 1);
                }
                next = goToDayOfMonth(next);

                if (next > date) {
                    return next;
                } else {
                    year += periodicity_;
                }
            }
            break;
        }
        case Monthly: {
            QDate iter(date.year(), date.month(), 1);
            while (true) {
                next = QDate(iter.year(), iter.month(), 1);
                next = goToDayOfMonth(next);

                if (next > date) {
                    return next;
                } else {
                    // Let Qt handle the year rollover for us
                    iter = iter.addMonths(periodicity_);
                }
            }
            break;
        }
        case Weekly: {
            next = goToDayOfWeek(date);
            if (next > date) {
                return next;
            } else {
                return next.addDays(7 * periodicity_);
            }
            break;
        }
        default:
            break;
        }
    }
    return next;
}

//--------------------------------------------------------------------------------------------------
QDate Recurrence::goToDayOfMonth(const QDate & date) const {
    if ((week_ > 0) and (week_ < 53)) {
        auto next = date.addDays((week_ - 1) * 7);
        return goToDayOfWeek(next);
    } else if ((day_ > 0) and (day_ < 32)) {
        return date.addDays(day_ - 1);
    } else { // No week or day defined, stay at first of the month
        return date;
    }
}

//--------------------------------------------------------------------------------------------------
QDate Recurrence::goToDayOfWeek(const QDate & date) const {
    // Go to beginning of week if no day was defined
    int day = ((day_ > 0) and (day_ < 8)) ? day_ : 1;
    int diff = day - date.dayOfWeek();
    if (diff >= 0) {
        return date.addDays(diff);
    } else {
        return date.addDays(7 + diff);
    }
}

//--------------------------------------------------------------------------------------------------
int32_t Recurrence::periodicity() const {
    return periodicity_;
}

//--------------------------------------------------------------------------------------------------
void Recurrence::setDay(int32_t day) {
    day_ = day;
}

//--------------------------------------------------------------------------------------------------
void Recurrence::setMonth(int32_t month) {
    month_ = month;
}

//--------------------------------------------------------------------------------------------------
void Recurrence::setPeriodicity(int32_t periodicity) {
    periodicity_ = periodicity;
}

//--------------------------------------------------------------------------------------------------
void Recurrence::setScope(ScopeType scope) {
    scope_ = scope;
}

//--------------------------------------------------------------------------------------------------
void Recurrence::setWeek(int32_t week) {
    week_ = week;
}

//--------------------------------------------------------------------------------------------------
Recurrence::ScopeType Recurrence::scope() const {
    return scope_;
}

//--------------------------------------------------------------------------------------------------
QString Recurrence::toString() const {
    if (periodicity_ < 1) {
        return QObject::tr("One-time");
    }

    QString str = QObject::tr("Every") % " ";

    if (periodicity_ > 1) {
        str = str % QString::number(periodicity_) % " ";
    }

    bool has_day = ((day_ > 0) and (day_ < 32));
    bool has_month = ((month_ > 0) and (month_ < 13));
    bool has_week = ((week_ > 0) and (week_ < 53));
    QLocale locale;

    switch (scope_) {
    case Yearly: {
        if (periodicity_ > 1) {
            str = str % QObject::tr("years");
        } else {
            str = str % QObject::tr("year");
        }

        if (has_day) {
            str = str % " " % QObject::tr("on day") % " " % QString::number(day_) % " ";
        }

        if (has_week) {
            str = str % " ";
            if (has_day) {
                str = str % QObject::tr("of week") % " " % QString::number(week_);
            } else {
                str = str % QObject::tr("in week") % " " % QString::number(week_);
            }
        }

        if (has_month) {
            str = str % " " % QObject::tr("in") % " " % locale.monthName(month_);
        }

        break;
    }
    case Monthly: {
        if (periodicity_ > 1) {
            str = str % QObject::tr("months");
        } else {
            str = str % QObject::tr("month");
        }

        if (has_day) {
            str = str % " " % QObject::tr("on day") % " " % QString::number(day_) % " ";
        }

        if (has_week) {
            str = str % " ";
            if (has_day) {
                str = str % QObject::tr("of week") % " " % QString::number(week_);
            } else {
                str = str % QObject::tr("in week") % " " % QString::number(week_);
            }
        }

        break;
    }
    case Weekly: {
        if (periodicity_ > 1) {
            str = str % QObject::tr("weeks");
        } else {
            str = str % QObject::tr("week");
        }

        if (has_day) {
            str = str % " " % QObject::tr("on day") % " " % QString::number(day_) % " ";
        }

        break;
    }
    default:
        str = QObject::tr("Unknown recurrence type");
    }

    return str;
}

//--------------------------------------------------------------------------------------------------
int32_t Recurrence::week() const {
    return week_;
}

} // budget namespace
} // ub namespace
