// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Recurrence.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
Recurrence::Recurrence() : day_(0), month_(0), periodicity_(0), scope_(Yearly), week_(0) {}

//--------------------------------------------------------------------------------------------------
int Recurrence::day() const {
    return day_;
}

//--------------------------------------------------------------------------------------------------
int Recurrence::month() const {
    return month_;
}

//--------------------------------------------------------------------------------------------------
int Recurrence::periodicity() const {
    return periodicity_;
}

//--------------------------------------------------------------------------------------------------
void Recurrence::setDay(int day) {
    day_ = day;
}

//--------------------------------------------------------------------------------------------------
void Recurrence::setMonth(int month) {
    month_ = month;
}

//--------------------------------------------------------------------------------------------------
void Recurrence::setPeriodicity(int periodicity) {
    periodicity_ = periodicity;
}

//--------------------------------------------------------------------------------------------------
void Recurrence::setScope(ScopeType scope) {
    scope_ = scope;
}

//--------------------------------------------------------------------------------------------------
void Recurrence::setWeek(int week) {
    week_ = week;
}

//--------------------------------------------------------------------------------------------------
Recurrence::ScopeType Recurrence::scope() const {
    return scope_;
}

//--------------------------------------------------------------------------------------------------
QString Recurrence::toString() const {
    return "";
}

//--------------------------------------------------------------------------------------------------
int Recurrence::week() const {
    return week_;
}

} // budget namespace
} // ub namespace
