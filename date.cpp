#include "date.h"

Date::Date(int year, int month, int day)
    : year(year), month(month), day(day) {}

bool Date::operator<(const Date &other) const
{
    if (year != other.year)
        return year < other.year;
    if (month != other.month)
        return month < other.month;
    return day < other.day;
}

bool Date::operator==(const Date &other) const
{
    return year == other.year && month == other.month && day == other.day;
}