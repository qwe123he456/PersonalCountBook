#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QVector>
#include <QString>
#include <QMap>
#include <QPair>
#include "item.h"

class DataManager
{
public:
    DataManager();

    // 获取数据
    const QVector<Item> &getItems() const;

    // 添加记录
    void addItem(const Item &item);

    // 删除记录（根据索引）
    void deleteItem(int index);

    // 修改记录（根据索引）
    void modifyItem(int index, const Item &item);

    // 查找记录（返回匹配的索引列表）
    QVector<int> searchByDate(const QString &keyword) const;
    QVector<int> searchByDesc(const QString &keyword) const;
    QVector<int> searchByCategory(const QString &keyword) const;
    QVector<int> searchByAmount(const QString &keyword) const;

    // 排序
    void sortByDateAsc();
    void sortByDateDesc();
    void sortByAmountAsc();
    void sortByAmountDesc();
    void sortByDescAsc();
    void sortByDescDesc();

    // 统计
    QString statisticsByYear(int startYear, int endYear) const;
    QString statisticsByMonth(int startYear, int startMonth, int endYear, int endMonth) const;
    QString statisticsByCategory() const;

    // 文件操作
    bool saveToFile(const QString &fileName) const;
    bool loadFromFile(const QString &fileName);

    // 辅助函数
    static QString categoryToString(Category cat);
    static QString categoryToDisplayString(Category cat);
    static QString formatAmount(int amount);

private:
    QVector<Item> m_items;
};

#endif // DATAMANAGER_H