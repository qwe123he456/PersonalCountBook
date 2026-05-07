/** @file 主窗口Widget类的实现
 */

#include "Widget.h"
#include <QHeaderView>

/** @brief 主窗口类
 *
 */
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_dataManager = new DataManager();
    setupUI();
    loadDataToTable();
}

Widget::~Widget() = default;

/** @brief 初始化所有UI组件
 *
 */
void Widget::setupUI()
{
    resize(900, 650);
    setWindowTitle("个人账本管理器");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 菜单栏：文件(F)按钮
    menuBar = new QMenuBar(this);
    fileMenu = new QMenu("文件(&F)", this);
    saveAction = new QAction("保存(&S)", this);
    loadAction = new QAction("读取(&O)", this);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    menuBar->addMenu(fileMenu);
    mainLayout->addWidget(menuBar);

    // 工具栏
    toolBar = new QToolBar(this);
    toolBar->setMovable(false);
    deleteAction = new QAction("删除", this);
    modifyAction = new QAction("修改", this);
    searchAction = new QAction("查找", this);
    sortAction = new QAction("排序", this);
    statisticsAction = new QAction("统计", this);
    toolBar->addAction(deleteAction);
    toolBar->addAction(modifyAction);
    toolBar->addAction(searchAction);
    toolBar->addAction(sortAction);
    toolBar->addAction(statisticsAction);
    mainLayout->addWidget(toolBar);

    // 数据显示区表格
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(5);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "序号"
                                                         << "日期"
                                                         << "类型"
                                                         << "金额"
                                                         << "明细");
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(tableWidget, 1);

    // 数据添加区
    QGroupBox *inputGroup = new QGroupBox("添加新记录", this);
    QFormLayout *inputLayout = new QFormLayout();

    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);
    inputLayout->addRow("日期:", dateEdit);

    categoryComboBox = new QComboBox(this);
    categoryComboBox->addItem("学习", study);
    categoryComboBox->addItem("生活", life);
    categoryComboBox->addItem("工作", job);
    inputLayout->addRow("类型:", categoryComboBox);

    amountSpinBox = new QSpinBox(this);
    amountSpinBox->setRange(-999999, 999999);
    amountSpinBox->setValue(0);
    inputLayout->addRow("金额:", amountSpinBox);

    descLineEdit = new QLineEdit(this);
    inputLayout->addRow("明细:", descLineEdit);

    addButton = new QPushButton("添加记录", this);
    inputLayout->addRow("", addButton);

    inputGroup->setLayout(inputLayout);
    mainLayout->addWidget(inputGroup);

    // 绑定按钮与点击事件
    connect(addButton, &QPushButton::clicked, this, &Widget::onAddClicked);
    connect(deleteAction, &QAction::triggered, this, &Widget::onDeleteClicked);
    connect(modifyAction, &QAction::triggered, this, &Widget::onModifyClicked);
    connect(searchAction, &QAction::triggered, this, &Widget::onSearchClicked);
    connect(sortAction, &QAction::triggered, this, &Widget::onSortClicked);
    connect(statisticsAction, &QAction::triggered, this, &Widget::onStatisticsClicked);
    connect(saveAction, &QAction::triggered, this, &Widget::onSaveClicked);
    connect(loadAction, &QAction::triggered, this, &Widget::onLoadClicked);
}

/** @brief 刷新显示表格数据
 *
 */
void Widget::loadDataToTable()
{
    const QVector<Item> &items = m_dataManager->getItems();
    tableWidget->setRowCount(items.size());

    for (int i = 0; i < items.size(); ++i)
    {
        const Item &item = items[i];

        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(i + 1));
        item0->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(
            QString("%1-%2-%3").arg(item.date.year).arg(item.date.month, 2, 10, QChar('0')).arg(item.date.day, 2, 10, QChar('0')));
        item1->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 1, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(DataManager::categoryToDisplayString(item.category));
        item2->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 2, item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(DataManager::formatAmount(item.amount));
        item3->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tableWidget->setItem(i, 3, item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(item.desc);
        item4->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i, 4, item4);
    }
}

/* ======槽函数实现====== */

/** @brief 添加记录
 *
 */
void Widget::onAddClicked()
{
    // 获取用户输入
    QDate qdate = dateEdit->date(); // 获取日期
    Date date(qdate.year(), qdate.month(), qdate.day());
    Category category = categoryComboBox->currentData().value<Category>(); // 获取类别
    int amount = amountSpinBox->value();                                   // 获取金额
    QString desc = descLineEdit->text();                                   // 获取明细

    Item newItem(date, category, desc, amount);
    m_dataManager->addItem(newItem); // 数组操作：添加新账目到列表中
    loadDataToTable();
    // 重置输入框
    descLineEdit->clear();
    amountSpinBox->setValue(0);
}

/** @brief 删除记录
 *
 */
void Widget::onDeleteClicked()
{
    int currentRow = tableWidget->currentRow(); // 光标所在的账目
    if (currentRow < 0)
    {
        QMessageBox::warning(this, "警告", "请先选择要删除的记录!");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认", "确定要删除选中的记录吗?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        m_dataManager->deleteItem(currentRow);
        loadDataToTable();
    }
}

/** @brief 修改记录
 *
 */
void Widget::onModifyClicked()
{
    int currentRow = tableWidget->currentRow(); // 光标所在的账目
    if (currentRow < 0)
    {
        QMessageBox::warning(this, "警告", "请先选择要修改的记录!");
        return;
    }

    const Item &item = m_dataManager->getItems()[currentRow]; // 通过引用对账目产生影响

    QDialog dialog(this);
    dialog.setWindowTitle("修改记录");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QFormLayout *form = new QFormLayout();

    QDateEdit *editDate = new QDateEdit(QDate(item.date.year, item.date.month, item.date.day), &dialog);
    editDate->setCalendarPopup(true);
    form->addRow("日期:", editDate);

    QComboBox *editCategory = new QComboBox(&dialog);
    editCategory->addItem("学习(study)", study);
    editCategory->addItem("生活(life)", life);
    editCategory->addItem("工作(job)", job);
    editCategory->setCurrentIndex((int)item.category);
    form->addRow("类型:", editCategory);

    QSpinBox *editAmount = new QSpinBox(&dialog);
    editAmount->setRange(-999999, 999999);
    editAmount->setValue(item.amount);
    form->addRow("金额:", editAmount);

    QLineEdit *editDesc = new QLineEdit(item.desc, &dialog);
    form->addRow("明细:", editDesc);

    layout->addLayout(form);

    QPushButton *okBtn = new QPushButton("确定", &dialog);
    QPushButton *cancelBtn = new QPushButton("取消", &dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        QDate qdate = editDate->date();
        Date newDate(qdate.year(), qdate.month(), qdate.day());
        Category newCategory = editCategory->currentData().value<Category>();
        int newAmount = editAmount->value();
        QString newDesc = editDesc->text();

        Item modifiedItem(newDate, newCategory, newDesc, newAmount);
        m_dataManager->modifyItem(currentRow, modifiedItem);
        loadDataToTable();
    }
}

/** @brief 查找记录
 *
 */
void Widget::onSearchClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("查找账务数据");
    dialog.resize(600, 450);
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QFormLayout *form = new QFormLayout();

    QComboBox *searchType = new QComboBox(&dialog);
    searchType->addItem("按日期");
    searchType->addItem("按明细");
    searchType->addItem("按类型");
    searchType->addItem("按金额");
    form->addRow("查找方式:", searchType);

    QLineEdit *keywordEdit = new QLineEdit(&dialog);
    form->addRow("关键字:", keywordEdit);

    layout->addLayout(form);

    QPushButton *searchBtn = new QPushButton("查找", &dialog);
    QPushButton *closeBtn = new QPushButton("关闭", &dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(searchBtn);
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);

    QTextBrowser *resultBrowser = new QTextBrowser(&dialog);
    layout->addWidget(resultBrowser);

    connect(searchBtn, &QPushButton::clicked, [&]()
            {
        resultBrowser->clear();
        int type = searchType->currentIndex();
        QString keyword = keywordEdit->text();

        QVector<int> results;
        if (type == 0)
            results = m_dataManager->searchByDate(keyword);
        else if (type == 1)
            results = m_dataManager->searchByDesc(keyword);
        else if (type == 2)
            results = m_dataManager->searchByCategory(keyword);
        else
            results = m_dataManager->searchByAmount(keyword);

        if (results.isEmpty())
        {
            resultBrowser->setText("未找到匹配的记录!");
        }
        else
        {
            const QVector<Item> &items = m_dataManager->getItems();
            QString result = QString("找到 %1 条记录:\n\n").arg(results.size());
            for (int idx : results)
            {
                const Item &item = items[idx];
                result += QString("序号:%1 日期:%2-%3-%4 类型:%5 金额:%6 明细:%7\n")
                              .arg(idx + 1)
                              .arg(item.date.year)
                              .arg(item.date.month, 2, 10, QChar('0'))
                              .arg(item.date.day, 2, 10, QChar('0'))
                              .arg(DataManager::categoryToDisplayString(item.category))
                              .arg(DataManager::formatAmount(item.amount))
                              .arg(item.desc);
            }
            resultBrowser->setText(result);
        } });

    connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::close);
    dialog.exec();
}

/** @brief 排序记录
 *
 */
void Widget::onSortClicked()
{
    QStringList items;
    items << "按日期(升序)" << "按日期(降序)" << "按金额(升序)" << "按金额(降序)"
          << "按明细(升序)" << "按明细(降序)";

    bool ok;
    QString selected = QInputDialog::getItem(this, "排序", "选择排序方式:", items, 0, false, &ok);
    if (!ok)
        return;

    if (selected == "按日期(升序)")
        m_dataManager->sortByDateAsc();
    else if (selected == "按日期(降序)")
        m_dataManager->sortByDateDesc();
    else if (selected == "按金额(升序)")
        m_dataManager->sortByAmountAsc();
    else if (selected == "按金额(降序)")
        m_dataManager->sortByAmountDesc();
    else if (selected == "按明细(升序)")
        m_dataManager->sortByDescAsc();
    else if (selected == "按明细(降序)")
        m_dataManager->sortByDescDesc();

    loadDataToTable();
    QMessageBox::information(this, "排序", "排序完成!");
}

/** @brief 统计功能
 *
 */
void Widget::onStatisticsClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("账务统计");
    dialog.resize(600, 450);
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QFormLayout *form = new QFormLayout();

    QDateEdit *startDate = new QDateEdit(QDate(2019, 1, 1), &dialog);
    startDate->setCalendarPopup(true);
    form->addRow("开始日期:", startDate);

    QDateEdit *endDate = new QDateEdit(QDate::currentDate(), &dialog);
    endDate->setCalendarPopup(true);
    form->addRow("结束日期:", endDate);

    QComboBox *statType = new QComboBox(&dialog);
    statType->addItem("按年统计");
    statType->addItem("按月统计");
    statType->addItem("按类型统计");
    form->addRow("统计方式:", statType);

    layout->addLayout(form);

    QPushButton *statBtn = new QPushButton("统计", &dialog);
    layout->addWidget(statBtn);

    QTextBrowser *resultBrowser = new QTextBrowser(&dialog);
    layout->addWidget(resultBrowser);

    connect(statBtn, &QPushButton::clicked, [&]()
            {
        QDate start = startDate->date();
        QDate end = endDate->date();
        QString result;

        int type = statType->currentIndex();
        if (type == 0)
            result = m_dataManager->statisticsByYear(start.year(), end.year());
        else if (type == 1)
            result = m_dataManager->statisticsByMonth(start.year(), start.month(), end.year(), end.month());
        else
            result = m_dataManager->statisticsByCategory();

        resultBrowser->setText(result); });

    dialog.exec();
}

/** @brief 保存数据
 *
 */
void Widget::onSaveClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", "", "Data Files (*.dat)");
    if (fileName.isEmpty())
        return;

    if (m_dataManager->saveToFile(fileName))
        QMessageBox::information(this, "保存", "数据保存成功!");
    else
        QMessageBox::critical(this, "错误", "无法打开文件进行写入!");
}

/** @brief 读取数据
 *
 */
void Widget::onLoadClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "Data Files (*.dat)");
    if (fileName.isEmpty())
        return;

    if (m_dataManager->loadFromFile(fileName))
    {
        loadDataToTable();
        QMessageBox::information(this, "读取", "数据读取成功!");
    }
    else
    {
        QMessageBox::critical(this, "错误", "无法打开文件进行读取!");
    }
}