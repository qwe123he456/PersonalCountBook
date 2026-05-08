#include "WelcomeWidget.h"
#include "Widget.h"

WelcomeWidget::WelcomeWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("个人账本管理系统");
    resize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(50);

    QPalette bgPalette;
    bgPalette.setColor(QPalette::Window, QColor(0, 85, 180));
    setPalette(bgPalette);
    setAutoFillBackground(true);

    QLabel *titleLabel = new QLabel("个人账本管理系统", this);
    QFont titleFont;
    titleFont.setPixelSize(48);
    titleFont.setBold(true);
    titleFont.setFamily("Microsoft YaHei");
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #ffffff;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(30);

    QPushButton *enterBtn = new QPushButton("进入", this);
    enterBtn->setMinimumSize(120, 50);
    enterBtn->setCursor(Qt::PointingHandCursor);
    enterBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #ffffff;"
        "   color: #0057B8;"
        "   border: 2px solid #ffffff;"
        "   border-radius: 8px;"
        "   font-size: 24px;"
        "   font-family: Microsoft YaHei;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #FFEC8B;"
        "   border: 2px solid #FFEC8B;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #E6C200;"
        "}");

    QPushButton *quitBtn = new QPushButton("退出", this);
    quitBtn->setMinimumSize(120, 50);
    quitBtn->setCursor(Qt::PointingHandCursor);
    quitBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #ffffff;"
        "   border: 2px solid #ffffff;"
        "   border-radius: 8px;"
        "   font-size: 24px;"
        "   font-family: Microsoft YaHei;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1E4C9A;"
        "   border: 2px solid #ffffff;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #003366;"
        "}");

    btnLayout->addStretch();
    btnLayout->addWidget(enterBtn);
    btnLayout->addWidget(quitBtn);
    btnLayout->addStretch();

    mainLayout->addLayout(btnLayout);
    mainLayout->addStretch();

    connect(enterBtn, &QPushButton::clicked, this, &WelcomeWidget::onEnterClicked);
    connect(quitBtn, &QPushButton::clicked, this, &WelcomeWidget::onQuitClicked);
}

void WelcomeWidget::onEnterClicked()
{
    Widget *w = new Widget();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
    close();
}

void WelcomeWidget::onQuitClicked()
{
    close();
}