#include "include/libcompress.h"

#include <iostream>
#include <QString>
#include <QFileInfoList>
#include <QDebug>
#include <unistd.h>
#include <QDir>
#include <QDirIterator>

 Compress::Compress(QWidget *parent) {
     allScripts.clear();
     u = new Utility(false);
     parentWidget = parent;

     aboutLib = "Using compiled version: Version 1.0.1 (Revision 1)"
                "\n"
                "libcompress is a static library with access to a ton of functionality that may be required by many\n"
                "different projects that implement zipping files and folders, but yet are exactly or almost the same.\n"
                "This library provides easy quick-access to such functions.\n\n"
                "This program is free software; you can redistribute it and/or modify it under the \n"
                "terms of the GNU General Public License as published by the Free Software Foundation; \neither version 2 of the "
                "License, or (at your option) any later version.\n\n"
                "New Features: \n"
                "1. Made the functions so that they the next one right after one function is done.\n"
                "2. Added functionality to use the library as a class. \n\n"
                "3. Improved the moveFiles() function to fix use cases for when it failed to move some\n"
                " files into their respective folders by implementing the lower level 'mv' command.\n\n"
                "Author @taps (Manic Software)\n\n"
                "Contact: 0785313273 or 0719069218";
}


void Compress::clearListForAllScripts() {
    allScripts.clear();
    qInfo() << "libcompress: Cleared list for all scripts";
}

void Compress::removeAllCreatedBashScripts() {
            foreach(const QString &script, allScripts) {
            bool result = Utility::removeFile(script);

            if (!result) {
                qWarning() << "libcompress: Could not remove file: " << script;
            }
        }

}

 void Compress::checkIfFilesWereDeleted() {
            foreach(const QString &_script, allScripts) {
            QFile _file(_script);

            if (_file.exists()) {
                qInfo() << "File: " << _file.fileName() << " still exists";

                // remove it
                bool res = _file.remove();

                if (!res) qWarning() << "Could not remove file: " << _file.fileName();
                else
                    qInfo() << "libcompress: " << "File " << _file.fileName() << " removed";
            } else {
                // file was successfully deleted the first time
            }
        }

    clearListForAllScripts();
}

QString Compress::makeTheLegendBashFile(const QStringList &otherBashScripts) {
    QStringList commands = {};

    const QString &shebang = "#!/bin/bash";
    commands.append(shebang);

    commands.append("\n");

    QString for_item_string = "for script in ";

            foreach(QString script, otherBashScripts) {
            QString msg = R"("%1")";
            const QString &scriptBasename = Utility::getBasename(script);
            msg = msg.arg(scriptBasename);
            for_item_string += msg;
            for_item_string += " ";
        }
    for_item_string = for_item_string.trimmed();
    for_item_string = for_item_string += ";";
    qDebug() << "For item in string: " << for_item_string;

    commands.append(for_item_string);

    QString do_string = "\tdo";
    commands.append(do_string);

    const QString &action_string = R"(        "/bin/bash" "$script")";
    commands.append(action_string);

    const QString done_string = "\tdone";
    commands.append(done_string);

    qInfo() << "libcompress: " << "Got commands as: " << commands;

    const QString &legendBashFile = "legend.sh";
    u->makeFile(legendBashFile);
    Utility::makeFileExecutable(legendBashFile);

    u->writeListToFile(commands, legendBashFile);

    return legendBashFile;

}

__attribute__((unused)) void Compress::createListForAllScripts() {
    allScripts.clear();
}

__attribute__((unused)) void Compress::addItemToListForAllScripts(const QString &item) {
    allScripts.append(item);
}

QStringList Compress::getContentToWriteInIndividualBashScript(const QString &dirname, const QString &passwd) {
    QStringList list = {};

    // append first item
    const QString &shebang = "#!/bin/bash";
    list.append(shebang);
    list.append("\n"); // newline

    const QString &zip = "/usr/bin/zip";
    const QString namedWithExt = Utility::getNameWithExtensionAddedToEnd(dirname, "zip");

    QString command;

    if (passwd.isEmpty()) {
        // don't use any password.
        // just zip the files

        command = R"("%1" -m -v -y -r "%2" "%3")";
        command = command.arg(zip, Utility::getBasename(namedWithExt), Utility::getBasename(dirname));

    } else {
        command = R"("%1" -m -v -y -r -e -P "%2" "%3" "%4")";
        command = command.arg(zip, passwd, Utility::getBasename(namedWithExt), Utility::getBasename(dirname));

    }

    qInfo() << "Using command: " << command;

    list.append(command);

    return list;
}


QString Compress::createBashScript(const QString &dir, const QString &passwd) {
    QString name = "%1.sh";
    name = name.arg(dir);

    u->makeFile(name, "", true);
    Utility::makeFileExecutable(name);

    QStringList commands = getContentToWriteInIndividualBashScript(dir, passwd);
    u->writeListToFile(commands, name); // Write commands to bash script

    return name;
}

void Compress::moveFiles(const QList<QStringList> &list) {
            foreach(const QStringList &lst, list) {
        const QString &old_name = lst.first();
        const QString &new_name = lst.at(1);

        QFileInfo fileInfo(new_name);
        QString dir = fileInfo.dir().absolutePath();

        qInfo() << "Dir for new name: " << dir;

        QDir _dir(dir);

        bool res = _dir.mkpath(dir);

        if (!res) {
            Utility::showMessageBoxCritical(parentWidget, "Error", "Failed to "
                                                                   "create the folder: " + dir);
        }

        u->renameFile(old_name, new_name);
    }

}

void Compress::moveAllFilesIntoDirs(const QString &folder) {
    /*
     * Move single files (that are not in any dir) to a folder so that
     * everything can be compressed
     *
     * Create a list of lists like [[oldName, newName]]
     * Then loop through this list and rename old file name to new file name
     * Then after renaming files, everything is now in folders
     * Start compressingc
     */

    QDir dir(folder);
    QList<QStringList> mainList;

    QDirIterator iter(dir);

    while (iter.hasNext()) {
        const QString &d = iter.next();
        if (!d.endsWith('.')) {
            QFileInfo fileInfo(d);
            if (fileInfo.isDir()) {
                qInfo() << "libcompress: Ignoring \"" << d << "\" coz it's already a folder";

            } else if (fileInfo.isFile()) {
                QStringList list = {};
                qInfo() << "libcompress: Should move \"" << d << "\" because it is a file";
                QString absolute_name = fileInfo.absoluteFilePath();
                qInfo() << "libcompress: Absolute file path for: " << fileInfo << " is: " << absolute_name;
                const QString &base = fileInfo.baseName();
                const QString &new_dir_name = fileInfo.path() + "/" + base;

                const QString &suffix = fileInfo.suffix();

                if (!suffix.isEmpty()) {
                    // file has a file extension

                    // check if file has a .sh extension

                    if (Utility::stringsEqual(suffix, "sh")) {
                        // file is a shell script
                        // prompt user if they want to move it to a folder

                        const QString &_text = "File: " + fileInfo.fileName() + " is a shell script."
                                                                                "Are you sure you want to"
                                                                                "move it into a folder and c"
                                                                                "ompress the folder?";
                        QMessageBox::StandardButton ans = Utility::showQuestionYesOrNo(parentWidget,
                                                                                       "Confirmation",
                                                                                       _text);

                        if (ans == QMessageBox::Yes) {
                            u->createDir(new_dir_name);

                        } else {
                            qInfo() << "Ignoring shell script: " + fileInfo.fileName() + " for compression";
                        }
                    }


                } else {
                    // file has no file extension

                    qInfo() << "libcompress: File has no suffix. Now using a different approach to make the folder.";
                    const QString &appData = u->getAppDataLocation();
                    u->createDir(appData);

                    const QString &temp_name = appData + "/" + fileInfo.fileName();
                    QFile::rename(absolute_name, temp_name); // move file to temp location
                    absolute_name = temp_name;

                    u->createDir(new_dir_name);
                }

                qInfo() << "libcompress: Got new dir name as: " << new_dir_name;
                const QString &newAbsoluteName = new_dir_name + "/" + fileInfo.fileName();
                qInfo() << "libcompress: Got new file name as: " << newAbsoluteName;

                list.append(absolute_name);
                list.append(newAbsoluteName);

                mainList.append(list);
            }
        }
    }
    qInfo() << "libcompress: Got QList as: " << mainList;
    moveFiles(mainList);
}

__attribute__((unused)) QString Compress::startProcess(const QString &dirname, const QString &passwdCategory) {
    std::string pwd_std = dirname.toStdString();
    const char *pwd_const_char = pwd_std.c_str();
    chdir(pwd_const_char);
    qInfo() << "libcompress: Changed current working directory to: " << dirname;

    moveAllFilesIntoDirs(dirname);

    QStringList bashScripts = {};
    QStringList dirs = u->getAllDirsInDir_StringList_v2(dirname);

    qInfo() << "libcompress: Got dirs in dir: " << dirs;

            foreach(QString file, dirs) {
            QFileInfo fileInfo(file);
            const QString &bashScript = createBashScript(fileInfo.fileName(), passwdCategory);
            bashScripts.append(bashScript);

            QFileInfo fileInfo1(bashScript);
            allScripts.append(fileInfo1.absoluteFilePath()); // use absolute path

            qInfo() << "Got bash script: " << bashScript;
        }


    const QString &legendBashFile = makeTheLegendBashFile(bashScripts);
    allScripts.append(legendBashFile);
//    const QString& pwd=dirname;

    return legendBashFile;
}

[[maybe_unused]] void Compress::showAboutLib_v2(const QString &imagePath, QWidget *parent) {
    AboutLibraryCompress about(imagePath, aboutLib, "About libcompress", parent);
    about.show();
}

// about library now

AboutLibraryCompress::AboutLibraryCompress(const QString &_image_path, const QString &_msg,
                                           const QString &_window_title,
                                           QWidget *parent)
        : QDialog(parent) {
    imagePath = _image_path;
    msg = _msg;

    setWindowModality(Qt::NonModal);
    qInfo() << "libcompress: " << imagePath << " :Image exists: " << QFile::exists(imagePath);

    textLabel = new QLabel;
    pictureLabel = new QLabel;
    okButton = new QPushButton("Ok");
    mainVbox = new QVBoxLayout;
    hbox = new QHBoxLayout;
    bottomLayout = new QHBoxLayout;
    enter = new QShortcut(QKeySequence("Enter"), this);
    sp1 = new QSpacerItem(32, 100, QSizePolicy::Expanding, QSizePolicy::Expanding);
    sp2 = new QSpacerItem(32, 100, QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(okButton, &QPushButton::clicked, this, &AboutLibraryCompress::close);
    connect(enter, &QShortcut::activated, this, &AboutLibraryCompress::close);
    pictureLabel->setFixedSize(300, 300);

    if (pixmap.load(imagePath)) {
        pixmap = pixmap.scaled(pictureLabel->size(), Qt::KeepAspectRatio);
        pictureLabel->setPixmap(pixmap);

        textLabel->setText(msg);

        hbox->addWidget(pictureLabel);
        hbox->addWidget(textLabel);

        bottomLayout->addSpacerItem(sp1);
        bottomLayout->addWidget(okButton);
        bottomLayout->addSpacerItem(sp2);

        mainVbox->addLayout(hbox);
        mainVbox->addLayout(bottomLayout);

        setLayout(mainVbox);
        setWindowTitle(_window_title);

        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        //        const QSize& currentSize = size();
        //        qInfo()<<"Current size: "<<currentSize;
        //        setFixedSize(currentSize);

        showDialog();

    } else {
        QMessageBox::warning(parent, "Error", "Could not load the image: '" + imagePath + "'");
        this->close();
        this->deleteLater();
    }
}

void AboutLibraryCompress::showDialog() {
    exec();
}
