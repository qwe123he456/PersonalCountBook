#include "Widget.h"

#include <QApplication>

/** @file 应用程序入口文件
 */

/** @brief 主函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数列表
 * @return 应用程序退出状态
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return QCoreApplication::exec();
}
