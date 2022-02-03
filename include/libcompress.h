#ifndef LIBCOMPRESS_LIBCOMPRESS_H
#define LIBCOMPRESS_LIBCOMPRESS_H

#include "libcompress_global.h"
#include <QPushButton>
#include <QString>
#include <QMessageBox>

#include <libutility.h>

class COMPRESS_LIBRARY_EXPORT Compress {

public:
    explicit Compress(QWidget *parent);

     void checkIfFilesWereDeleted();

    void moveFiles(const QList<QStringList> &list);

    void moveAllFilesIntoDirs(const QString &folder);

   void removeAllCreatedBashScripts();

    QString makeTheLegendBashFile(const QStringList &otherBashScripts);

    static QStringList getContentToWriteInIndividualBashScript(const QString &dirname, const QString &passwd);

    __attribute__((unused)) void addItemToListForAllScripts(const QString &item);

    __attribute__((unused)) void createListForAllScripts();

    void clearListForAllScripts();

    __attribute__((unused)) QString startProcess(const QString &dirname, const QString &passwdCategory);

    QString createBashScript(const QString &dir, const QString &passwd);

     void showAboutLib_v2(const QString &imagePath, QWidget *parent);

private:
    Utility *u;
    QStringList allScripts = {};
    QString aboutLib;
    QWidget *parentWidget;
};

// about lib dialog

class COMPRESS_LIBRARY_EXPORT AboutLibraryCompress : public QDialog {

public:
    AboutLibraryCompress(const QString &_image_path, const QString &_msg, const QString &_window_title,
                         QWidget *parent);

    void showDialog();

private:

    QString imagePath;
    QString msg;

    QPushButton *okButton;
    QVBoxLayout *mainVbox;
    QHBoxLayout *hbox;
    QHBoxLayout *bottomLayout;
    QSpacerItem *sp1;
    QSpacerItem *sp2;
    QShortcut *enter;
    QPixmap pixmap;

    QLabel *textLabel;
    QLabel *pictureLabel;
};

#endif // LIBCOMPRESS_LIBCOMPRESS_H
