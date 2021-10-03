#ifndef LIBCOMPRESS_LIBRARY_H
#define LIBCOMPRESS_LIBRARY_H

#include <QString>

void removeAllCreatedBashScripts();
QString makeTheLegendBashFile(const QStringList& otherBashScripts);
QStringList getContentToWriteInIndividualBashScript(const QString& dirname, const QString& passwd);
void addItemToListForAllScripts(const QString& item);
void createListForAllScripts();
void clearListForAllScripts();
QString startProcess(const QString& dirname, const QString& passwdCategory);
QString createBashScript(const QString &dir, const QString& passwd);

#endif //LIBCOMPRESS_LIBRARY_H
