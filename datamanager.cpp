/** @file 数据管理器DataManager类的实现
 */

#include "datamanager.h"
#include <QFile>
#include <QDataStream>
#include <QDate>
#include <algorithm>
#include <QDebug>

/** @brief 构造函数
 * @return
 */
DataManager::DataManager()
{
    // 初始化测试数据
    m_items = {
        Item(Date(2024, 3, 21), study, "学习", 20),
        Item(Date(2022, 8, 12), life, "生活", -30),
    };
}

/** @brief 获取所有记录
 * @return 记录列表的引用
 */
const QVector<Item> &DataManager::getItems() const
{
    return m_items;
}

/** @brief 添加记录
 * @param item 要添加的记录
 * @return
 */
void DataManager::addItem(const Item &item)
{
    m_items.append(item);
}

/** @brief 删除记录
 * @param index 要删除的记录索引
 * @return
 */
void DataManager::deleteItem(int index)
{
    if (index >= 0 && index < m_items.size())
    {
        m_items.removeAt(index);
    }
}

/** @brief 修改记录
 * @param index 要修改的记录索引
 * @param item 修改后的记录内容
 * @return
 */
void DataManager::modifyItem(int index, const Item &item)
{
    if (index >= 0 && index < m_items.size())
    {
        m_items[index] = item;
    }
}

/** @brief 按日期查找记录
 * @param keyword 日期关键字
 * @return 匹配的记录索引列表
 */
QVector<int> DataManager::searchByDate(const QString &keyword) const
{
    QVector<int> results;
    for (int i = 0; i < m_items.size(); ++i)
    {
        const Item &item = m_items[i];
        QString dateStr = QString("%1-%2-%3")
                              .arg(item.date.year)
                              .arg(item.date.month, 2, 10, QChar('0'))
                              .arg(item.date.day, 2, 10, QChar('0'));
        if (dateStr.contains(keyword))
        {
            results.append(i);
        }
    }
    return results;
}

/** @brief 按明细查找记录
 * @param keyword 明细关键字
 * @return 匹配的记录索引列表
 */
QVector<int> DataManager::searchByDesc(const QString &keyword) const
{
    QVector<int> results;
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i].desc.contains(keyword))
        {
            results.append(i);
        }
    }
    return results;
}

/** @brief 按类型查找记录
 * @param keyword 类型关键字
 * @return 匹配的记录索引列表
 */
QVector<int> DataManager::searchByCategory(const QString &keyword) const
{
    QVector<int> results;
    for (int i = 0; i < m_items.size(); ++i)
    {
        QString catStr = categoryToDisplayString(m_items[i].category);
        QString catStrEn = categoryToString(m_items[i].category);
        if (catStr.contains(keyword) || catStrEn.contains(keyword))
        {
            results.append(i);
        }
    }
    return results;
}

/** @brief 按金额查找记录
 * @param keyword 金额关键字
 * @return 匹配的记录索引列表
 */
QVector<int> DataManager::searchByAmount(const QString &keyword) const
{
    QVector<int> results;
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (QString::number(m_items[i].amount).contains(keyword))
        {
            results.append(i);
        }
    }
    return results;
}

/** @brief 按日期升序排序
 * @return
 */
void DataManager::sortByDateAsc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b)
              {
        if (a.date.year != b.date.year)
            return a.date.year < b.date.year;
        if (a.date.month != b.date.month)
            return a.date.month < b.date.month;
        return a.date.day < b.date.day; });
}

/** @brief 按日期降序排序
 * @return
 */
void DataManager::sortByDateDesc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b)
              {
        if (a.date.year != b.date.year)
            return a.date.year > b.date.year;
        if (a.date.month != b.date.month)
            return a.date.month > b.date.month;
        return a.date.day > b.date.day; });
}

/** @brief 按金额升序排序
 * @return
 */
void DataManager::sortByAmountAsc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b)
              { return a.amount < b.amount; });
}

/** @brief 按金额降序排序
 * @return
 */
void DataManager::sortByAmountDesc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b)
              { return a.amount > b.amount; });
}

/** @brief 按明细升序排序
 * @return
 */
void DataManager::sortByDescAsc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b)
              { return a.desc < b.desc; });
}

/** @brief 按明细降序排序
 * @return
 */
void DataManager::sortByDescDesc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b)
              { return a.desc > b.desc; });
}

/** @brief 按年份统计
 * @param startYear 起始年份
 * @param endYear 结束年份
 * @return 统计结果字符串
 */
QString DataManager::statisticsByYear(int startYear, int endYear) const
{
    QMap<int, QPair<int, int>> yearStats;
    for (const Item &item : m_items)
    {
        if (item.date.year >= startYear && item.date.year <= endYear)
        {
            int y = item.date.year;
            if (item.amount >= 0)
                yearStats[y].first += item.amount;
            else
                yearStats[y].second += item.amount;
        }
    }

    QString result = "按年统计:\n\n";
    int totalIncome = 0, totalExpense = 0;
    for (auto it = yearStats.begin(); it != yearStats.end(); ++it)
    {
        result += QString("年份: %1\n  收入: %2\n  支出: %3\n  结余: %4\n\n")
                      .arg(it.key())
                      .arg(formatAmount(it.value().first))
                      .arg(formatAmount(it.value().second))
                      .arg(formatAmount(it.value().first + it.value().second));
        totalIncome += it.value().first;
        totalExpense += it.value().second;
    }
    result += QString("总计:\n  收入: %1\n  支出: %2\n  结余: %3")
                  .arg(formatAmount(totalIncome))
                  .arg(formatAmount(totalExpense))
                  .arg(formatAmount(totalIncome + totalExpense));
    return result;
}

/** @brief 按月份统计
 * @param startYear 起始年份
 * @param startMonth 起始月份
 * @param endYear 结束年份
 * @param endMonth 结束月份
 * @return 统计结果字符串
 */
QString DataManager::statisticsByMonth(int startYear, int startMonth, int endYear, int endMonth) const
{
    QMap<QString, QPair<int, int>> monthStats;
    for (const Item &item : m_items)
    {
        int year = item.date.year;
        int month = item.date.month;
        if ((year > startYear || (year == startYear && month >= startMonth)) &&
            (year < endYear || (year == endYear && month <= endMonth)))
        {
            QString key = QString("%1-%2").arg(year).arg(month, 2, 10, QChar('0'));
            if (item.amount >= 0)
                monthStats[key].first += item.amount;
            else
                monthStats[key].second += item.amount;
        }
    }

    QString result = "按月统计:\n\n";
    int totalIncome = 0, totalExpense = 0;
    for (auto it = monthStats.begin(); it != monthStats.end(); ++it)
    {
        result += QString("月份: %1\n  收入: %2\n  支出: %3\n  结余: %4\n\n")
                      .arg(it.key())
                      .arg(formatAmount(it.value().first))
                      .arg(formatAmount(it.value().second))
                      .arg(formatAmount(it.value().first + it.value().second));
        totalIncome += it.value().first;
        totalExpense += it.value().second;
    }
    result += QString("总计:\n  收入: %1\n  支出: %2\n  结余: %3")
                  .arg(formatAmount(totalIncome))
                  .arg(formatAmount(totalExpense))
                  .arg(formatAmount(totalIncome + totalExpense));
    return result;
}

/** @brief 按类型统计
 * @return 统计结果字符串
 */
QString DataManager::statisticsByCategory() const
{
    QMap<Category, QPair<int, int>> catStats;
    for (const Item &item : m_items)
    {
        if (item.amount >= 0)
            catStats[item.category].first += item.amount;
        else
            catStats[item.category].second += item.amount;
    }

    QString result = "按类型统计:\n\n";
    int totalIncome = 0, totalExpense = 0;
    for (int c = study; c <= job; c = (Category)(c + 1))
    {
        QString catName = categoryToDisplayString((Category)c);
        result += QString("类型: %1\n  收入: %2\n  支出: %3\n  结余: %4\n\n")
                      .arg(catName)
                      .arg(formatAmount(catStats[(Category)c].first))
                      .arg(formatAmount(catStats[(Category)c].second))
                      .arg(formatAmount(catStats[(Category)c].first + catStats[(Category)c].second));
        totalIncome += catStats[(Category)c].first;
        totalExpense += catStats[(Category)c].second;
    }
    result += QString("总计:\n  收入: %1\n  支出: %2\n  结余: %3")
                  .arg(formatAmount(totalIncome))
                  .arg(formatAmount(totalExpense))
                  .arg(formatAmount(totalIncome + totalExpense));
    return result;
}

/** @brief 保存数据到文件
 * @param fileName 文件路径
 * @return 是否保存成功
 */
bool DataManager::saveToFile(const QString &fileName) const
{
    qDebug() << "保存记录数量:" << m_items.size();
    qDebug() << "保存文件路径:" << fileName;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "文件打开失败!";
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);

    out << m_items.size();
    for (const Item &item : m_items)
    {
        out << item.date.year;
        out << item.date.month;
        out << item.date.day;
        out << (int)item.category;
        out << item.desc;
        out << item.amount;
    }

    file.close();
    return true;
}

/** @brief 从文件读取数据
 * @param fileName 文件路径
 * @return 是否读取成功
 */
bool DataManager::loadFromFile(const QString &fileName)
{
    qDebug() << "读取文件路径:" << fileName;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件打开失败!";
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);

    int size;
    in >> size;
    qDebug() << "文件中的记录数量:" << size;

    m_items.clear();
    for (int i = 0; i < size; ++i)
    {
        int year, month, day, category, amount;
        QString desc;

        in >> year >> month >> day >> category >> desc >> amount;

        qDebug() << "读取记录" << (i + 1)
                 << "日期:" << year << "-" << month << "-" << day
                 << "类型:" << category << "金额:" << amount << "明细:" << desc;

        Date date(year, month, day);
        Item item(date, (Category)category, desc, amount);
        item.index = i + 1;
        m_items.append(item);
    }

    file.close();
    return true;
}

/** @brief 类型转换为英文字符串
 * @param cat 类型
 * @return 英文字符串
 */
QString DataManager::categoryToString(Category cat)
{
    switch (cat)
    {
    case study:
        return "study";
    case life:
        return "life";
    case job:
        return "job";
    }
    return "study";
}

/** @brief 类型转换为显示字符串
 * @param cat 类型
 * @return 中文显示字符串
 */
QString DataManager::categoryToDisplayString(Category cat)
{
    switch (cat)
    {
    case study:
        return "学习";
    case life:
        return "生活";
    case job:
        return "工作";
    }
    return "学习";
}

/** @brief 格式化金额显示
 * @param amount 金额
 * @return 格式化后的字符串
 */
QString DataManager::formatAmount(int amount)
{
    if (amount > 0)
        return QString("+%1").arg(amount);
    return QString::number(amount);
}