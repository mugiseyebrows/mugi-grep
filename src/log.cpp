#include "log.h"

#include <QString>
#include <QFile>
#include <QDebug>
#include <QDateTime>

void log(const QString& text) {
    QString path = "log.txt";
    QFile file(path);
    if (!file.open(QIODevice::Append)) {
        qDebug() << "cannot open";
    }
    QString msg = QDateTime::currentDateTime().toString("hh:mm:ss.zzz ") + text + "\n";
    file.write(msg.toUtf8());
}
