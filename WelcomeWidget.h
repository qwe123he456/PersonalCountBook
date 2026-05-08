/** @file 欢迎界面头文件
 */

#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPalette>

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWidget(QWidget *parent = nullptr);

signals:
    void enterClicked();

private slots:
    void onEnterClicked();
    void onQuitClicked();
};

#endif // WELCOMEWIDGET_H