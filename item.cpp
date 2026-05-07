#include "item.h"

/** @file 记录Item类的实现
 */

/** @brief 构造函数
 * @param date 日期
 * @param category 类型
 * @param desc 明细
 * @param amount 金额
 *
 */
Item::Item(Date date, Category category, QString desc, int amount)
    : date(date), category(category), desc(desc), amount(amount), index(0) {}