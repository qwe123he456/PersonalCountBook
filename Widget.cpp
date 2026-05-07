#include "Widget.h"
#include <QApplication>
#include <QHeaderView>
#include <algorithm>

/* 全局变量：存储所有帐务记录的列表 */
/* 全局变量：存储所有帐务记录的列表 */
QVector<Item> itemList = {
    Item(Date(2020, 3, 21), study, "", 20),
    Item(Date(2022, 8, 12), life, "", -30),
};

/* ============================================================
 * Date类的成员函数实现
 * ============================================================ */

// 构造函数：初始化日期，默认为1年1月1日
Date::Date(int year, int month, int day)
    : year(year), month(month), day(day) {}

// 重载小于运算符，用于日期比较（用于排序）
bool Date::operator<(const Date &other) const
{
    if (year != other.year)
        return year < other.year;
    if (month != other.month)
        return month < other.month;
    return day < other.day;
}

// 重载等于运算符，用于日期比较
bool Date::operator==(const Date &other) const
{
    return year == other.year && month == other.month && day == other.day;
}

/* ============================================================
 * Item类的成员函数实现
 * ============================================================ */

// 构造函数：初始化帐务记录
// 参数：date-日期，category-类型(study/life/job)，desc-明细，amount-金额
Item::Item(Date date, Category category, QString desc, int amount)
    : date(date), category(category), desc(desc), amount(amount), index(0) {}

/* ============================================================
 * Widget类的实现
 * ============================================================ */

// 构造函数：初始化UI并加载数据
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();         // 初始化界面组件
    loadDataToTable(); // 将itemList中的数据加载到表格显示
}

Widget::~Widget() = default;

/* -----------------------------------------------------------
 * 函数: setupUI
 * 作用: 初始化所有UI组件（菜单栏、工具栏、表格、输入面板）
 * ----------------------------------------------------------- */
void Widget::setupUI()
{
    // 设置窗口大小和标题
    resize(900, 650);
    setWindowTitle("个人账本管理器");

    // 创建主布局（垂直布局）
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 移除边距

    /* ---- 菜单栏 ---- */
    menuBar = new QMenuBar(this);
    fileMenu = new QMenu("文件(&F)", this);

    // 创建"保存"和"读取"菜单项
    saveAction = new QAction("保存(&S)", this);
    loadAction = new QAction("读取(&O)", this);

    fileMenu->addAction(saveAction); // 添加保存菜单项
    fileMenu->addAction(loadAction); // 添加读取菜单项
    menuBar->addMenu(fileMenu);      // 将文件菜单添加到菜单栏
    mainLayout->addWidget(menuBar);  // 将菜单栏添加到主布局

    /* ---- 工具栏 ---- */
    toolBar = new QToolBar(this);
    toolBar->setMovable(false); // 禁止工具栏移动

    // 创建各个功能按钮
    addAction = new QAction("新增", this);
    deleteAction = new QAction("删除", this);
    modifyAction = new QAction("修改", this);
    searchAction = new QAction("查找", this);
    sortAction = new QAction("排序", this);
    statisticsAction = new QAction("统计", this);

    // 将动作依次添加工具栏
    toolBar->addAction(addAction);
    toolBar->addAction(deleteAction);
    toolBar->addAction(modifyAction);
    toolBar->addAction(searchAction);
    toolBar->addAction(sortAction);
    toolBar->addAction(statisticsAction);
    mainLayout->addWidget(toolBar);

    /* ---- 表格控件 ---- */
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(5); // 设置5列
    // 设置表头标签：序号、日期、类型、金额、明细
    tableWidget->setHorizontalHeaderLabels(QStringList() << "序号"
                                                         << "日期"
                                                         << "类型"
                                                         << "金额"
                                                         << "明细");
    tableWidget->horizontalHeader()->setStretchLastSection(true);     // 最后一列自动拉伸
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选中
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁止编辑
    mainLayout->addWidget(tableWidget, 1);                            // 1表示拉伸权重，让表格占据更多空间

    /* ---- 输入面板 ---- */
    QGroupBox *inputGroup = new QGroupBox("添加新记录", this);
    QFormLayout *inputLayout = new QFormLayout();

    // 日期选择器（带日历弹窗）
    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);
    inputLayout->addRow("日期:", dateEdit);

    // 类型下拉框（学习、生活、工作）
    categoryComboBox = new QComboBox(this);
    categoryComboBox->addItem("学习", study);
    categoryComboBox->addItem("生活", life);
    categoryComboBox->addItem("工作", job);
    inputLayout->addRow("类型:", categoryComboBox);

    // 金额输入框（支持负数）
    amountSpinBox = new QSpinBox(this);
    amountSpinBox->setRange(-999999, 999999); // 金额范围
    amountSpinBox->setValue(0);
    inputLayout->addRow("金额:", amountSpinBox);

    // 明细输入框
    descLineEdit = new QLineEdit(this);
    inputLayout->addRow("明细:", descLineEdit);

    // 添加记录按钮
    addButton = new QPushButton("添加记录", this);
    inputLayout->addRow("", addButton);

    inputGroup->setLayout(inputLayout);
    mainLayout->addWidget(inputGroup);

    /* ---- 信号槽连接 ---- */
    // 将按钮/动作的点击信号连接到对应的处理函数
    connect(addButton, &QPushButton::clicked, this, &Widget::onAddClicked);
    connect(deleteAction, &QAction::triggered, this, &Widget::onDeleteClicked);
    connect(modifyAction, &QAction::triggered, this, &Widget::onModifyClicked);
    connect(searchAction, &QAction::triggered, this, &Widget::onSearchClicked);
    connect(sortAction, &QAction::triggered, this, &Widget::onSortClicked);
    connect(statisticsAction, &QAction::triggered, this, &Widget::onStatisticsClicked);
    connect(saveAction, &QAction::triggered, this, &Widget::onSaveClicked);
    connect(loadAction, &QAction::triggered, this, &Widget::onLoadClicked);
}

/* -----------------------------------------------------------
 * 辅助函数：类型转换
 * ----------------------------------------------------------- */

// 将Category枚举转换为字符串（用于文件保存）
QString Widget::categoryToString(Category cat)
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

// 将字符串转换为Category枚举（用于文件读取）
Category Widget::stringToCategory(const QString &str)
{
    if (str == "life")
        return life;
    if (str == "job")
        return job;
    return study;
}

// 将Category枚举转换为显示用中文
QString Widget::categoryToDisplayString(Category cat)
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

/* -----------------------------------------------------------
 * 函数: formatAmount
 * 作用: 格式化金额显示，正数加+号，负数不变
 * ----------------------------------------------------------- */
QString Widget::formatAmount(int amount)
{
    if (amount > 0)
        return QString("+%1").arg(amount);
    return QString::number(amount);
}

/* -----------------------------------------------------------
 * 函数: loadDataToTable
 * 作用: 将itemList中的所有记录加载到表格控件中显示
 *       每次数据发生改变（增/删/改/读）后都需要调用此函数
 * ----------------------------------------------------------- */
void Widget::loadDataToTable()
{
    // 设置表格行数（与itemList大小一致）
    tableWidget->setRowCount(itemList.size());

    // 遍历itemList，将每条记录填入表格对应行
    for (int i = 0; i < itemList.size(); ++i)
    {
        const Item &item = itemList[i];

        // 第0列：序号（从1开始，居中）
        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(i + 1));
        item0->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 0, item0);

        // 第1列：日期（格式：YYYY-MM-DD，居中）
        QTableWidgetItem *item1 = new QTableWidgetItem(QString("%1-%2-%3").arg(item.date.year).arg(item.date.month, 2, 10, QChar('0')).arg(item.date.day, 2, 10, QChar('0')));
        item1->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 1, item1);

        // 第2列：类型（中文显示，居中）
        QTableWidgetItem *item2 = new QTableWidgetItem(categoryToDisplayString(item.category));
        item2->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 2, item2);

        // 第3列：金额（靠右对齐）
        QTableWidgetItem *item3 = new QTableWidgetItem(formatAmount(item.amount));
        item3->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tableWidget->setItem(i, 3, item3);

        // 第4列：明细（居中）
        QTableWidgetItem *item4 = new QTableWidgetItem(item.desc);
        item4->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 4, item4);
        tableWidget->setItem(i, 4, new QTableWidgetItem(item.desc));
    }
}

/* -----------------------------------------------------------
 * 函数: updateTableIndex
 * 作用: 排序后重新编排所有记录的序号（从1开始编号）
 * ----------------------------------------------------------- */
void Widget::updateTableIndex()
{
    for (int i = 0; i < itemList.size(); ++i)
    {
        itemList[i].index = i + 1; // 更新内存中的序号
        if (tableWidget->item(i, 0))
            tableWidget->item(i, 0)->setText(QString::number(i + 1)); // 更新表格显示
    }
}

/* ============================================================
 * 槽函数实现（对应各个功能按钮）
 * ============================================================ */

/* ---- 新增记录 ---- */
void Widget::onAddClicked()
{
    // 从输入控件获取数据
    QDate qdate = dateEdit->date();                      // 获取选择的日期
    Date date(qdate.year(), qdate.month(), qdate.day()); // 转换为Date类型

    // 获取类型（从QVariant中提取）
    Category category = categoryComboBox->currentData().value<Category>();

    // 获取金额和明细
    int amount = amountSpinBox->value();
    QString desc = descLineEdit->text();

    // 创建新记录并添加到列表（明细允许为空，默认空串）
    Item newItem(date, category, desc, amount);
    itemList.append(newItem);

    // 刷新表格显示
    loadDataToTable();

    // 清空输入框，准备下一次输入
    descLineEdit->clear();
    amountSpinBox->setValue(0);
}

/* ---- 删除记录 ---- */
void Widget::onDeleteClicked()
{
    // 获取当前选中的行
    int currentRow = tableWidget->currentRow();

    if (currentRow < 0)
    {
        QMessageBox::warning(this, "警告", "请先选择要删除的记录!");
        return;
    }

    // 弹出确认对话框
    // QMessageBox::question的第二个参数是父窗口，第三个是标题，第四个是提示文本
    // QMessageBox::Yes | QMessageBox::No 是位运算，同时显示两个按钮
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认", "确定要删除选中的记录吗?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // 从列表中移除指定位置的记录
        itemList.removeAt(currentRow);
    }
    // 刷新表格显示
    loadDataToTable();

    // 更新序号（因为删除后索引变化了）
    updateTableIndex();
}

/* ---- 修改记录 ---- */
void Widget::onModifyClicked()
{
    // 第一步：查找要修改的记录
    bool ok;
    QString keyword = QInputDialog::getText(this, "查找", "请输入要修改的记录关键字(明细/金额):",
                                            QLineEdit::Normal, "", &ok);
    if (!ok || keyword.isEmpty())
        return; // 用户取消或输入为空

    // 在itemList中查找匹配关键字的记录
    QVector<int> matchedIndices; // 存储匹配记录的索引
    for (int i = 0; i < itemList.size(); ++i)
    {
        const Item &item = itemList[i];
        // 匹配条件：明细包含关键字 或 金额包含关键字
        if (item.desc.contains(keyword) || QString::number(item.amount).contains(keyword))
        {
            matchedIndices.append(i);
        }
    }

    // 处理查找结果
    if (matchedIndices.isEmpty())
    {
        QMessageBox::information(this, "查找结果", "未找到匹配的记录!");
        return;
    }

    // 决定最终要修改的记录索引
    int targetIndex;
    if (matchedIndices.size() == 1)
    {
        // 只找到一条，直接使用
        targetIndex = matchedIndices[0];
    }
    else
    {
        // 找到多条，让用户选择具体哪一条
        QStringList items;
        for (int idx : matchedIndices)
        {
            const Item &item = itemList[idx];
            // 构建显示字符串，便于用户识别
            items.append(QString("%1: %2-%3-%4 %5 %6 %7")
                             .arg(idx + 1)
                             .arg(item.date.year)
                             .arg(item.date.month)
                             .arg(item.date.day)
                             .arg(categoryToDisplayString(item.category))
                             .arg(formatAmount(item.amount))
                             .arg(item.desc));
        }

        // 弹出下拉框让用户选择
        bool okSelect;
        QString selected = QInputDialog::getItem(this, "选择", "找到多条记录，请选择要修改的记录序号:",
                                                 items, 0, false, &okSelect);
        if (!okSelect)
            return; // 用户取消

        // 获取用户选择的索引
        targetIndex = matchedIndices[items.indexOf(selected)];
    }

    // 第二步：弹出修改对话框
    Item &item = itemList[targetIndex]; // 获取要修改的记录的引用

    // 创建对话框
    QDialog dialog(this);
    dialog.setWindowTitle("修改记录");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // 创建表单布局
    QFormLayout *form = new QFormLayout();

    // 日期编辑框（带日历）
    QDateEdit *editDate = new QDateEdit(QDate(item.date.year, item.date.month, item.date.day), &dialog);
    editDate->setCalendarPopup(true);
    form->addRow("日期:", editDate);

    // 类型下拉框
    QComboBox *editCategory = new QComboBox(&dialog);
    editCategory->addItem("学习(study)", study);
    editCategory->addItem("生活(life)", life);
    editCategory->addItem("工作(job)", job);
    editCategory->setCurrentIndex((int)item.category); // 设置当前值
    form->addRow("类型:", editCategory);

    // 金额输入框
    QSpinBox *editAmount = new QSpinBox(&dialog);
    editAmount->setRange(-999999, 999999);
    editAmount->setValue(item.amount);
    form->addRow("金额:", editAmount);

    // 明细输入框
    QLineEdit *editDesc = new QLineEdit(item.desc, &dialog);
    form->addRow("明细:", editDesc);

    layout->addLayout(form);

    // 确定和取消按钮
    QPushButton *okBtn = new QPushButton("确定", &dialog);
    QPushButton *cancelBtn = new QPushButton("取消", &dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    // 按钮信号连接
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    // 执行对话框，根据用户选择决定是否保存修改
    if (dialog.exec() == QDialog::Accepted)
    {
        // 将对话框中的值更新到itemList
        QDate qdate = editDate->date();
        item.date.year = qdate.year();
        item.date.month = qdate.month();
        item.date.day = qdate.day();
        item.category = editCategory->currentData().value<Category>();
        item.amount = editAmount->value();
        item.desc = editDesc->text();

        // 刷新表格显示
        loadDataToTable();
    }
}

/* ---- 查找记录 ---- */
void Widget::onSearchClicked()
{
    // 创建查找对话框
    QDialog dialog(this);
    dialog.setWindowTitle("查找帐务数据");
    dialog.resize(600, 450); // 设置默认宽度
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // 表单布局
    QFormLayout *form = new QFormLayout();

    // 查找方式下拉框
    QComboBox *searchType = new QComboBox(&dialog);
    searchType->addItem("按日期");
    searchType->addItem("按明细");
    searchType->addItem("按类型");
    searchType->addItem("按金额");
    form->addRow("查找方式:", searchType);

    // 关键字输入框
    QLineEdit *keywordEdit = new QLineEdit(&dialog);
    form->addRow("关键字:", keywordEdit);

    layout->addLayout(form);

    // 查找和关闭按钮
    QPushButton *searchBtn = new QPushButton("查找", &dialog);
    QPushButton *closeBtn = new QPushButton("关闭", &dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(searchBtn);
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);

    // 结果显示区（只读文本浏览器）
    QTextBrowser *resultBrowser = new QTextBrowser(&dialog);
    layout->addWidget(resultBrowser);

    // 查找按钮的点击处理（使用lambda表达式）
    connect(searchBtn, &QPushButton::clicked, [&]()
            {
        resultBrowser->clear();  // 清空上一次的结果

        // 获取用户选择的查找方式和输入的关键字
        int type = searchType->currentIndex();
        QString keyword = keywordEdit->text();

        // 遍历itemList查找匹配记录
        QVector<int> matchedIndices;
        for (int i = 0; i < itemList.size(); ++i)
        {
            const Item &item = itemList[i];
            bool matched = false;

            // 根据查找方式进行匹配
            if (type == 0)  // 按日期查找（模糊匹配）
            {
                // 将日期转换为字符串后进行模糊匹配
                QString dateStr = QString("%1-%2-%3")
                                      .arg(item.date.year)
                                      .arg(item.date.month, 2, 10, QChar('0'))
                                      .arg(item.date.day, 2, 10, QChar('0'));
                matched = dateStr.contains(keyword);
            }
            else if (type == 1)  // 按明细查找（模糊匹配）
            {
                matched = item.desc.contains(keyword);
            }
            else if (type == 2)  // 按类型查找（模糊匹配，支持中文或英文）
            {
                QString catStr = categoryToDisplayString(item.category);  // 中文：学习、生活、工作
                QString catStrEn = categoryToString(item.category);       // 英文：study、life、job
                matched = catStr.contains(keyword) || catStrEn.contains(keyword);
            }
            else if (type == 3)  // 按金额查找（模糊匹配）
            {
                matched = QString::number(item.amount).contains(keyword);
            }

            if (matched)
                matchedIndices.append(i);
        }

        // 显示查找结果
        if (matchedIndices.isEmpty())
        {
            resultBrowser->setText("未找到匹配的记录!");
        }
        else
        {
            QString result = QString("找到 %1 条记录:\n\n").arg(matchedIndices.size());
            for (int idx : matchedIndices)
            {
                const Item &item = itemList[idx];
                result += QString("序号:%1    日期:%2-%3-%4    类型:%5    金额:%6    明细:%7\n")
                              .arg(idx + 1)
                              .arg(item.date.year)
                              .arg(item.date.month, 2, 10, QChar('0'))
                              .arg(item.date.day, 2, 10, QChar('0'))
                              .arg(categoryToDisplayString(item.category))
                              .arg(formatAmount(item.amount))
                              .arg(item.desc);
            }
            resultBrowser->setText(result);
        } });

    // 关闭按钮
    connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::close);

    // 显示对话框（模态）
    dialog.exec();
}

/* ---- 排序记录 ---- */
void Widget::onSortClicked()
{
    // 弹出选择对话框，让用户选择排序方式
    QStringList items;
    items << "按日期(升序)" << "按日期(降序)" << "按金额(升序)" << "按金额(降序)"
          << "按明细(升序)" << "按明细(降序)";

    bool ok;
    QString selected = QInputDialog::getItem(this, "排序", "选择排序方式:", items, 0, false, &ok);
    if (!ok)
        return; // 用户取消

    // 根据用户选择执行不同的排序
    if (selected == "按日期(升序)")
    {
        // 使用std::sort进行排序，lambda表达式定义比较规则
        std::sort(itemList.begin(), itemList.end(), [](const Item &a, const Item &b)
                  {
            if (a.date.year != b.date.year)
                return a.date.year < b.date.year;
            if (a.date.month != b.date.month)
                return a.date.month < b.date.month;
            return a.date.day < b.date.day; });
    }
    else if (selected == "按日期(降序)")
    {
        std::sort(itemList.begin(), itemList.end(), [](const Item &a, const Item &b)
                  {
            if (a.date.year != b.date.year)
                return a.date.year > b.date.year;
            if (a.date.month != b.date.month)
                return a.date.month > b.date.month;
            return a.date.day > b.date.day; });
    }
    else if (selected == "按金额(升序)")
    {
        std::sort(itemList.begin(), itemList.end(), [](const Item &a, const Item &b)
                  { return a.amount < b.amount; });
    }
    else if (selected == "按金额(降序)")
    {
        std::sort(itemList.begin(), itemList.end(), [](const Item &a, const Item &b)
                  { return a.amount > b.amount; });
    }
    else if (selected == "按明细(升序)")
    {
        std::sort(itemList.begin(), itemList.end(), [](const Item &a, const Item &b)
                  { return a.desc < b.desc; });
    }
    else if (selected == "按明细(降序)")
    {
        std::sort(itemList.begin(), itemList.end(), [](const Item &a, const Item &b)
                  { return a.desc > b.desc; });
    }

    // 排序完成后刷新表格显示，并重新编排序号
    loadDataToTable();
    updateTableIndex();

    // 提示用户排序完成
    QMessageBox::information(this, "排序", "排序完成!");
}

/* ---- 统计功能 ---- */
void Widget::onStatisticsClicked()
{
    // 创建统计对话框
    QDialog dialog(this);
    dialog.setWindowTitle("帐务统计");
    dialog.resize(600, 450); // 设置默认宽度
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // 设置日期范围和统计方式
    QFormLayout *form = new QFormLayout();

    // 开始日期（默认为2020年1月1日）
    QDateEdit *startDate = new QDateEdit(QDate(2020, 1, 1), &dialog);
    startDate->setCalendarPopup(true);
    form->addRow("开始日期:", startDate);

    // 结束日期（默认为今天）
    QDateEdit *endDate = new QDateEdit(QDate::currentDate(), &dialog);
    endDate->setCalendarPopup(true);
    form->addRow("结束日期:", endDate);

    // 统计方式下拉框
    QComboBox *statType = new QComboBox(&dialog);
    statType->addItem("按年统计");
    statType->addItem("按月统计");
    statType->addItem("按类型统计");
    form->addRow("统计方式:", statType);

    layout->addLayout(form);

    // 统计按钮
    QPushButton *statBtn = new QPushButton("统计", &dialog);
    layout->addWidget(statBtn);

    // 结果显示区
    QTextBrowser *resultBrowser = new QTextBrowser(&dialog);
    layout->addWidget(resultBrowser);

    // 统计按钮点击处理
    connect(statBtn, &QPushButton::clicked, [&]()
            {
        // 获取用户选择的日期范围
        QDate start = startDate->date();
        QDate end = endDate->date();

        // 筛选出日期范围内的记录
        QVector<Item> filtered;
        for (const Item &item : itemList)
        {
            QDate itemDate(item.date.year, item.date.month, item.date.day);
            if (itemDate >= start && itemDate <= end)
                filtered.append(item);
        }

        QString result;
        int type = statType->currentIndex();

        /* ---- 按年统计 ---- */
        if (type == 0)
        {
            // 使用QMap存储统计数据（key:年份, value:QPair<收入,支出>）
            QMap<int, QPair<int, int>> yearStats;
            for (const Item &item : filtered)
            {
                int y = item.date.year;
                if (item.amount >= 0)
                    yearStats[y].first += item.amount;   // 收入
                else
                    yearStats[y].second += item.amount;  // 支出（负数）
            }

            result = "按年统计:\n\n";
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
        }
        /* ---- 按月统计 ---- */
        else if (type == 1)
        {
            QMap<QString, QPair<int, int>> monthStats;
            for (const Item &item : filtered)
            {
                // key格式：YYYY-MM
                QString key = QString("%1-%2").arg(item.date.year)
                                             .arg(item.date.month, 2, 10, QChar('0'));
                if (item.amount >= 0)
                    monthStats[key].first += item.amount;
                else
                    monthStats[key].second += item.amount;
            }

            result = "按月统计:\n\n";
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
        }
        /* ---- 按类型统计 ---- */
        else
        {
            QMap<Category, QPair<int, int>> catStats;
            for (const Item &item : filtered)
            {
                if (item.amount >= 0)
                    catStats[item.category].first += item.amount;
                else
                    catStats[item.category].second += item.amount;
            }

            result = "按类型统计:\n\n";
            int totalIncome = 0, totalExpense = 0;
            for (int c = study; c <= job; c = (Category)(c + 1))
            {
                QString catName = categoryToDisplayString((Category)c);
                result += QString("类型: %1\n  收入: %2\n  支出: %3\n  结余: %4\n\n")
                              .arg(catName)
                              .arg(catStats[(Category)c].first)
                              .arg(catStats[(Category)c].second)
                              .arg(catStats[(Category)c].first + catStats[(Category)c].second);
                totalIncome += catStats[(Category)c].first;
                totalExpense += catStats[(Category)c].second;
            }
            result += QString("总计:\n  收入: %1\n  支出: %2\n  结余: %3")
                          .arg(totalIncome)
                          .arg(totalExpense)
                          .arg(totalIncome + totalExpense);
        }

        resultBrowser->setText(result); });

    dialog.exec();
}

/* ---- 保存数据到文件（二进制格式）---- */
void Widget::onSaveClicked()
{
    // 弹出文件保存对话框
    // 参数：父窗口、标题、默认目录、文件过滤器
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", "", "Data Files (*.dat)");
    if (fileName.isEmpty())
        return; // 用户取消

    // 打开文件（只写模式）
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        // 打开失败，弹出错误对话框
        QMessageBox::critical(this, "错误", "无法打开文件进行写入!");
        return;
    }

    // 创建数据流对象，用于二进制写入
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15); // 设置版本，确保兼容性

    // 首先写入记录数量
    out << itemList.size();

    // 依次写入每条记录的数据
    for (const Item &item : itemList)
    {
        out << item.date.year;
        out << item.date.month;
        out << item.date.day;
        out << (int)item.category; // 枚举转int
        out << item.desc;          // QString会自动处理编码
        out << item.amount;
    }

    // 关闭文件
    file.close();

    // 提示保存成功
    QMessageBox::information(this, "保存", "数据保存成功!");
}

/* ---- 从文件读取数据（二进制格式）---- */
void Widget::onLoadClicked()
{
    // 弹出文件打开对话框
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "Data Files (*.dat)");
    if (fileName.isEmpty())
        return; // 用户取消

    // 打开文件（只读模式）
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "错误", "无法打开文件进行读取!");
        return;
    }

    // 创建数据流对象，用于二进制读取
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);

    // 首先读取记录数量
    int size;
    in >> size;

    // 清空现有数据
    itemList.clear();

    // 依次读取每条记录
    for (int i = 0; i < size; ++i)
    {
        int year, month, day, category, amount;
        QString desc;

        in >> year >> month >> day >> category >> desc >> amount;

        Date date(year, month, day);
        Item item(date, (Category)category, desc, amount);
        item.index = i + 1;
        itemList.append(item);
    }

    // 关闭文件
    file.close();

    // 刷新表格显示
    loadDataToTable();

    // 提示读取成功
    // QMessageBox::information(this, "读取", "数据读取成功!");
}