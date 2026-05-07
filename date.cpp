#include "date.h"

/** @file 日期Date类的实现
 */

/** @brief 构造函数
 * @param year 年
 * @param month 月
 * @param day 日
 * @return
 */
Date::Date(int year, int month, int day)
    : year(year), month(month), day(day) {}

/** @brief 小于运算符重载
 * @param other 比较的日期对象
 * @return 是否小于
 */
bool Date::operator<(const Date &other) const
{
    if (year != other.year)
        return year < other.year;
    if (month != other.month)
        return month < other.month;
    return day < other.day;
}

/** @brief 等于运算符重载
 * @param other 比较的日期对象
 * @return 是否等于
 */
bool Date::operator==(const Date &other) const
{
    return year == other.year && month == other.month && day == other.day;
}