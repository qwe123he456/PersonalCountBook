/** @file 数据管理器DataManager类的定义
 */

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QVector>
#include <QString>
#include <QMap>
#include <QPair>
#include "item.h"

/** @brief 数据管理器类
 *
 */
class DataManager
{
public:
    /** @brief 构造函数
     *
     */
    DataManager();

    /** @brief 获取所有记录
     * @return 记录列表的引用
     */
    const QVector<Item> &getItems() const;

    /** @brief 添加记录
     * @param item 要添加的记录
     *
     */
    void addItem(const Item &item);

    /** @brief 删除记录
     * @param index 要删除的记录索引
     *
     */
    void deleteItem(int index);

    /** @brief 修改记录
     * @param index 要修改的记录索引
     * @param item 修改后的记录内容
     *
     */
    void modifyItem(int index, const Item &item);

    /** @brief 按日期查找记录
     * @param keyword 日期关键字
     * @return 匹配的记录索引列表
     */
    QVector<int> searchByDate(const QString &keyword) const;
    /** @brief 按明细查找记录
     * @param keyword 明细关键字
     * @return 匹配的记录索引列表
     */
    QVector<int> searchByDesc(const QString &keyword) const;
    /** @brief 按类型查找记录
     * @param keyword 类型关键字
     * @return 匹配的记录索引列表
     */
    QVector<int> searchByCategory(const QString &keyword) const;
    /** @brief 按金额查找记录
     * @param keyword 金额关键字
     * @return 匹配的记录索引列表
     */
    QVector<int> searchByAmount(const QString &keyword) const;

    /** @brief 按日期升序排序
     *
     */
    void sortByDateAsc();
    /** @brief 按日期降序排序
     *
     */
    void sortByDateDesc();
    /** @brief 按金额升序排序
     *
     */
    void sortByAmountAsc();
    /** @brief 按金额降序排序
     *
     */
    void sortByAmountDesc();
    /** @brief 按明细升序排序
     *
     */
    void sortByDescAsc();
    /** @brief 按明细降序排序
     *
     */
    void sortByDescDesc();

    /** @brief 按年份统计
     * @param startYear 起始年份
     * @param endYear 结束年份
     * @return 统计结果字符串
     */
    QString statisticsByYear(int startYear, int endYear) const;
    /** @brief 按月份统计
     * @param startYear 起始年份
     * @param startMonth 起始月份
     * @param endYear 结束年份
     * @param endMonth 结束月份
     * @return 统计结果字符串
     */
    QString statisticsByMonth(int startYear, int startMonth, int endYear, int endMonth) const;
    /** @brief 按类型统计
     * @return 统计结果字符串
     */
    QString statisticsByCategory() const;

    /** @brief 保存数据到文件
     * @param fileName 文件路径
     * @return 是否保存成功
     */
    bool saveToFile(const QString &fileName) const;
    /** @brief 从文件读取数据
     * @param fileName 文件路径
     * @return 是否读取成功
     */
    bool loadFromFile(const QString &fileName);

    /** @brief 类型转换为英文字符串
     * @param cat 类型
     * @return 英文字符串
     */
    static QString categoryToString(Category cat);
    /** @brief 类型转换为显示字符串
     * @param cat 类型
     * @return 中文显示字符串
     */
    static QString categoryToDisplayString(Category cat);
    /** @brief 格式化金额显示
     * @param amount 金额
     * @return 格式化后的字符串
     */
    static QString formatAmount(int amount);

private:
    QVector<Item> m_items;
};

#endif // DATAMANAGER_H