#include "widget.h"

#include <QApplication>
#include <QFileInfo>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>

#include "DataProvider/XMLParameters/XMLContainer.h"
#include "DataProvider/li/include.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString s=QFileInfo(argv[0]).absolutePath();
    appFolder(s.toStdString());
    backupFolder(QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0].toStdString());
    XML::XMLContainer::instance();

    Widget w;
    w.show();
    return a.exec();
}
