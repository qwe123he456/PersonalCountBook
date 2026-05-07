#ifndef DATE_H
#define DATE_H

class Date
{
public:
    int year;
    int month;
    int day;

    Date(int year = 1, int month = 1, int day = 1);

    bool operator<(const Date &other) const;
    bool operator==(const Date &other) const;
};

#endif // DATE_H