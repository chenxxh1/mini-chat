#include "server.h"
#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QCommandLineParser>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 设置应用程序信息
    a.setApplicationName("ChatServer");
    a.setApplicationVersion("1.0.0");

    // 命令行解析器
    QCommandLineParser parser;
    parser.setApplicationDescription("Chat Server Console Application");
    parser.addHelpOption();
    parser.addVersionOption();

    // 添加打印间隔选项
    QCommandLineOption intervalOption(
        QStringList() << "i" << "interval",
        "Set online accounts printing interval in seconds (0 to disable)",
        "seconds",
        "30" // 默认值
        );
    parser.addOption(intervalOption);

    // 处理命令行参数
    parser.process(a);

    // 安装翻译器
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Server_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // 创建服务器实例
    Server server;

    // 设置打印间隔
    int interval = parser.value(intervalOption).toInt();
    server.setPrintInterval(interval);

    qDebug() << "Server is running. Press Ctrl+C to exit.";
    return a.exec();
}
