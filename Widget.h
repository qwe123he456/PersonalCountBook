/** @file 主窗口Widget类的定义
 */

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QTableWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QGroupBox>
#include <QMap>
#include <QTextBrowser>
#include <QPainter>
#include <QLabel>

#include "datamanager.h"

/** @brief 主窗口类
 *
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     */
    explicit Widget(QWidget *parent = nullptr);
    /**
     * @brief 析构函数
     */
    ~Widget() override;

private:
    DataManager *m_dataManager;

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QAction *saveAction;
    QAction *loadAction;
    QToolBar *toolBar;
    QAction *addAction;
    QAction *deleteAction;
    QAction *modifyAction;
    QAction *searchAction;
    QAction *sortAction;
    QAction *statisticsAction;

    QTableWidget *tableWidget;
    QDateEdit *dateEdit;
    QComboBox *categoryComboBox;
    QSpinBox *amountSpinBox;
    QLineEdit *descLineEdit;
    QPushButton *addButton;
    QGroupBox *inputGroup;
    QGroupBox *searchGroup;
    QComboBox *searchType;
    QLineEdit *keywordEdit;
    QPushButton *searchBtn;
    QPushButton *closeSearchBtn;
    QPushButton *closeAddBtn;
    bool isSearchMode;
    int currentSearchType;
    QString currentKeyword;
    QVector<Item> backupItems;

    /**
     * @brief 初始化所有UI组件
     *
     */
void setupUI();
    void loadDataToTable();
    void loadSearchResults();

private slots:
    /**
     * @brief 新增记录
     *
     */
    void onAddClicked();
    /**
     * @brief 删除记录
     *
     */
    void onDeleteClicked();
    /**
     * @brief 修改记录
     *
     */
    void onModifyClicked();
    /**
     * @brief 查找记录
     *
     */
    void onSearchClicked();
    /**
     * @brief 排序记录
     *
     */
    void onSortClicked();
    /**
     * @brief 统计功能
     *
     */
    void onStatisticsClicked();
    /**
     * @brief 保存数据
     *
     */
    void onSaveClicked();
    /**
     * @brief 读取数据
     *
     */
    void onLoadClicked();
};

#endif // WIDGET_H