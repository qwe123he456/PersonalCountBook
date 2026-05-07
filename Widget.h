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
#include <QLabel>
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

enum Category
{
    study,
    life,
    job
};

class Date
{
public:
    int year;
    int month;
    int day;
    Date(int year = 1, int month = 1, int day = 1);
    bool operator<(const Date &other) const;
    bool operator==(const Date &other) const;
};

class Item
{
public:
    Date date;
    Category category;
    QString desc;
    int amount;
    int index;
    Item(Date date = Date(), Category category = study, QString desc = "", int amount = 0);
};

extern QVector<Item> itemList;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private:
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

    void setupUI();
    void loadDataToTable();
    void updateTableIndex();

    QString categoryToString(Category cat);
    Category stringToCategory(const QString &str);
    QString categoryToDisplayString(Category cat);
    QString formatAmount(int amount);

private slots:
    void onAddClicked();
    void onDeleteClicked();
    void onModifyClicked();
    void onSearchClicked();
    void onSortClicked();
    void onStatisticsClicked();
    void onSaveClicked();
    void onLoadClicked();
};

#endif // WIDGET_H