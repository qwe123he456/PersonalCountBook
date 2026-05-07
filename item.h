#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include "date.h"

enum Category
{
    study,
    life,
    job
};

class Item
{
public:
    Date date;
    Category category;
    QString desc;
    int amount;
    int index;

    Item(Date date = Date(), Category category = study, QString desc = "", int amount = 0);
};

#endif // ITEM_H