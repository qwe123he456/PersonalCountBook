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

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
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

    void setupUI();
    void loadDataToTable();

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