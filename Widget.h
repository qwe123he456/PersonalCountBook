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

#include "datamanager.h"

/**
 * @brief 主窗口类
 *
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     * @return
     */
    explicit Widget(QWidget *parent = nullptr);
    /**
     * @brief 析构函数
     * @return
     */
    ~Widget() override;

private:
    DataManager *m_dataManager;

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QAction *saveAction;
    QAction *loadAction;
    QToolBar *toolBar;
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

    /**
     * @brief 初始化所有UI组件
     * @return
     */
    void setupUI();
    /**
     * @brief 刷新显示表格数据
     * @return
     */
    void loadDataToTable();

private slots:
    /**
     * @brief 新增记录
     * @return
     */
    void onAddClicked();
    /**
     * @brief 删除记录
     * @return
     */
    void onDeleteClicked();
    /**
     * @brief 修改记录
     * @return
     */
    void onModifyClicked();
    /**
     * @brief 查找记录
     * @return
     */
    void onSearchClicked();
    /**
     * @brief 排序记录
     * @return
     */
    void onSortClicked();
    /**
     * @brief 统计功能
     * @return
     */
    void onStatisticsClicked();
    /**
     * @brief 保存数据
     * @return
     */
    void onSaveClicked();
    /**
     * @brief 读取数据
     * @return
     */
    void onLoadClicked();
};

#endif // WIDGET_H