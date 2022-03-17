#ifndef LIBCOMPRESS_LIBCOMPRESS_H
#define LIBCOMPRESS_LIBCOMPRESS_H

#include "libcompress_global.h"
#include <QMessageBox>
#include <QPushButton>
#include <QString>

#include <libaboutlibrary.h>
#include <libutility.h>

class COMPRESS_LIBRARY_EXPORT Compress {

public:
    __attribute__((unused)) explicit Compress(QWidget *parent);

    __attribute__((unused)) void checkIfFilesWereDeleted();

    void moveFiles(const QList<QStringList> &list);

    void moveAllFilesIntoDirs(const QString &folder);

    __attribute__((unused)) void removeAllCreatedBashScripts();

    QString makeTheLegendBashFile(const QStringList &otherBashScripts);

    QStringList
    getContentToWriteInIndividualBashScript(const QString &dirname,
                                            const QString &passwd);

    __attribute__((unused)) void addItemToListForAllScripts(const QString &item);

    __attribute__((unused)) void createListForAllScripts();

    void clearListForAllScripts();

    __attribute__((unused)) QString startProcess(const QString &dirname,
                                                 const QString &passwdCategory);

    QString createBashScript(const QString &dir, const QString &passwd);

    __attribute__((unused)) void showAbout(QWidget *parent);

private:
    Utility *u;
    QStringList allScripts = {};
    QString aboutLib;
    QWidget *parentWidget;
};

#endif // LIBCOMPRESS_LIBCOMPRESS_H
