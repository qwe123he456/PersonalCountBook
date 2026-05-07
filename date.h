#ifndef DATE_H
#define DATE_H

/** @file 日期Date类的定义
 */

/**
 * @brief 日期类
 *
 */
class Date
{
public:
    int year;
    int month;
    int day;

    /**
     * @brief 构造函数
     * @param year 年
     * @param month 月
     * @param day 日
     * @return
     */
    Date(int year = 1, int month = 1, int day = 1);

    /**
     * @brief 小于运算符重载
     * @param other 比较的日期对象
     * @return 是否小于
     */
    bool operator<(const Date &other) const;

    /**
     * @brief 等于运算符重载
     * @param other 比较的日期对象
     * @return 是否等于
     */
    bool operator==(const Date &other) const;
};

#endif // DATE_H