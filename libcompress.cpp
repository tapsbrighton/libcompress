#include "include/libcompress.h"

#include <iostream>
#include <QString>
#include <QFileInfoList>
#include <QDebug>
#include <unistd.h>
#include <QDir>
#include <QDirIterator>
#include <utility.h>

#include "include/libcompress.h"

Compress::Compress() {
    allScripts.clear();
    u = new Utility(false);
}


void Compress::clearListForAllScripts() {
    allScripts.clear();
    qInfo() << "libcompress: Cleared list for all scripts";
}

void Compress::removeAllCreatedBashScripts() {
            foreach(const QString &script, allScripts) {
            bool result = u->removeFile(script);

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

                if (!res) qWarning() << "Could not remove file: " << _file;
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
            const QString &scriptBasename = u->getBasename(script);
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
    u->makeFileExecutable(legendBashFile);

    u->writeListToFile(commands, legendBashFile);

    return legendBashFile;

}

void Compress::createListForAllScripts() {
    allScripts.clear();
}

void Compress::addItemToListForAllScripts(const QString &item) {
    allScripts.append(item);
}

QStringList Compress::getContentToWriteInIndividualBashScript(const QString &dirname, const QString &passwd) {
    QStringList list = {};

    // append first item
    const QString &shebang = "#!/bin/bash";
    list.append(shebang);
    list.append("\n"); // newline

    const QString &zip = "/usr/bin/zip";
    const QString namedWithExt = u->getNameWithExtensionAddedToEnd(dirname, "zip");

    QString command;

    if (passwd.isEmpty()) {
        // don't use any password.
        // just zip the files

        command = R"("%1" -m -v -y -r "%2" "%3")";
        command = command.arg(zip, u->getBasename(namedWithExt), u->getBasename(dirname));

    } else {
        command = R"("%1" -m -v -y -r -e -P "%2" "%3" "%4")";
        command = command.arg(zip, passwd, u->getBasename(namedWithExt), u->getBasename(dirname));

    }

    qInfo() << "Using command: " << command;

    list.append(command);

    return list;
}


QString Compress::createBashScript(const QString &dir, const QString &passwd) {
    QString name = "%1.sh";
    name = name.arg(dir);

    u->makeFile(name, "", true);
    u->makeFileExecutable(name);

    QStringList commands = getContentToWriteInIndividualBashScript(dir, passwd);
    u->writeListToFile(commands, name); // Write commands to bash script

    return name;
}

void Compress::moveFiles(const QList<QStringList> &list) {
            foreach(const QStringList &lst, list) {
            const QString &old_name = lst.first();
            const QString &new_name = lst.at(1);

            try {
                QFile::rename(old_name, new_name);
                qInfo() << "libcompress: Successfully moved file: " << old_name;

            } catch (int num) {
                qInfo() << "libcompress: Failed to move file: " << old_name;
            }
        }

}

void Compress::moveAllFilesIntoDirs(const QString &folder) {
    /*
     * Move single files (that are not in any dir) to a folder so that
     * everything can be compressed
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
                    u->createDir(new_dir_name);

                } else {
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

QString Compress::startProcess(const QString &dirname, const QString &passwdCategory) {
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