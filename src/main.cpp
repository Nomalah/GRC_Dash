#include <QApplication>
#include <QQmlApplicationEngine>
#include "RuntimeQml.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    RuntimeQml *rt = new RuntimeQml(&engine);
    rt->load(QStringLiteral("qrc:/main.qml"));
    rt->parseQrc("qml.qrc");
    rt->setAutoReload(true);
    return app.exec();
}
