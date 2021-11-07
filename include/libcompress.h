#ifndef LIBCOMPRESS_LIBCOMPRESS_H
#define LIBCOMPRESS_LIBCOMPRESS_H

#include <QString>

#include "libcompress_global.h"

void moveFiles(const QList<QStringList> &list);

void moveAllFilesIntoDirs(const QString &folder);

void removeAllCreatedBashScripts();

QString makeTheLegendBashFile(const QStringList &otherBashScripts);

QStringList getContentToWriteInIndividualBashScript(const QString &dirname, const QString &passwd);

void addItemToListForAllScripts(const QString &item);

void createListForAllScripts();

void clearListForAllScripts();

QString startProcess(const QString &dirname, const QString &passwdCategory);

QString createBashScript(const QString &dir, const QString &passwd);

#endif //LIBCOMPRESS_LIBCOMPRESS_H
