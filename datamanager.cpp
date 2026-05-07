#include "datamanager.h"
#include <QFile>
#include <QDataStream>
#include <QDate>
#include <algorithm>
#include <QDebug>

DataManager::DataManager()
{
    // 初始化测试数据
    m_items = {
        Item(Date(2024, 3, 21), study, "学习", 20),
        Item(Date(2022, 8, 12), life, "生活", -30),
    };
}

const QVector<Item> &DataManager::getItems() const
{
    return m_items;
}

void DataManager::addItem(const Item &item)
{
    m_items.append(item);
}

void DataManager::deleteItem(int index)
{
    if (index >= 0 && index < m_items.size())
    {
        m_items.removeAt(index);
    }
}

void DataManager::modifyItem(int index, const Item &item)
{
    if (index >= 0 && index < m_items.size())
    {
        m_items[index] = item;
    }
}

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

void DataManager::sortByDateAsc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b) {
        if (a.date.year != b.date.year)
            return a.date.year < b.date.year;
        if (a.date.month != b.date.month)
            return a.date.month < b.date.month;
        return a.date.day < b.date.day;
    });
}

void DataManager::sortByDateDesc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b) {
        if (a.date.year != b.date.year)
            return a.date.year > b.date.year;
        if (a.date.month != b.date.month)
            return a.date.month > b.date.month;
        return a.date.day > b.date.day;
    });
}

void DataManager::sortByAmountAsc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b) {
        return a.amount < b.amount;
    });
}

void DataManager::sortByAmountDesc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b) {
        return a.amount > b.amount;
    });
}

void DataManager::sortByDescAsc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b) {
        return a.desc < b.desc;
    });
}

void DataManager::sortByDescDesc()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b) {
        return a.desc > b.desc;
    });
}

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

QString DataManager::formatAmount(int amount)
{
    if (amount > 0)
        return QString("+%1").arg(amount);
    return QString::number(amount);
}