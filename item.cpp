#include "item.h"

Item::Item(Date date, Category category, QString desc, int amount)
    : date(date), category(category), desc(desc), amount(amount), index(0) {}