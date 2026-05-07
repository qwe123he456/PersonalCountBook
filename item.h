#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include "date.h"

/** @file 记录Item类的定义
 */

/** @brief 记录类型枚举
 *
 */
enum Category
{
    study,
    life,
    job
};

/** @brief 记录类
 *
 */
class Item
{
public:
    Date date;
    Category category;
    QString desc;
    int amount;
    int index;

    /**
     * @brief 构造函数
     * @param date 日期
     * @param category 类型
     * @param desc 明细
     * @param amount 金额
     *
     */
    Item(Date date = Date(), Category category = study, QString desc = "", int amount = 0);
};

#endif // ITEM_H