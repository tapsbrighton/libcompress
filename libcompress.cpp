#include "libcompress.h"

#include <iostream>
#include <QString>
#include <QFileInfoList>
#include <QDebug>
#include <unistd.h>
#include <QDir>
#include <QDirIterator>
#include <utility.h>

#include "include/libcompress.h"

QStringList allScripts = {};
Utility *u = new Utility(false);

void clearListForAllScripts() {
    allScripts.clear();
    qDebug() << "libcompress: Cleared list for all scripts";
}

void removeAllCreatedBashScripts()
{
    foreach(const QString& script, allScripts){
            bool result = u->removeFile(script);

            if (!result){
                qDebug()<<"libcompress: Could not remove file: "<<script;
            }
    }

    clearListForAllScripts();
}

QString makeTheLegendBashFile(const QStringList& otherBashScripts)
{
    QStringList commands={};

    const QString& shebang="#!/bin/bash";
    commands.append(shebang);

    commands.append("\n");

    QString for_item_string="for script in ";

            foreach(QString script, otherBashScripts){
            QString msg = R"("%1")";
            const QString &scriptBasename = u->getBasename(script);
            msg=msg.arg(scriptBasename);
            for_item_string+=msg;
            for_item_string+=" ";
        }
    for_item_string=for_item_string.trimmed();
    for_item_string=for_item_string+=";";
    qDebug()<<"For item in string: "<<for_item_string;

    commands.append(for_item_string);

    QString do_string="\tdo";
    commands.append(do_string);

//    const QString& action_string=R"\t\t"/bin/bash" "$script")";
//    const QString& action_string="\t\t\"/bin/bash\" \"$script\"";
    const QString &action_string = R"(        "/bin/bash" "$script")";
    commands.append(action_string);

    const QString done_string = "\tdone";
    commands.append(done_string);

    qDebug() << "libmyutility: " << "Got commands as: " << commands;

    const QString &legendBashFile = "legend.sh";
    u->makeFile(legendBashFile);
    u->makeFileExecutable(legendBashFile);

    u->writeListToFile(commands, legendBashFile);

//    QFile::copy(legendBashFile, "/home/taps/Music/"+legendBashFile);
//    qDebug()<<"Copied legend bash file for debug purposes.";

    return legendBashFile;

}

void createListForAllScripts()
{
    allScripts.clear();
}

void addItemToListForAllScripts(const QString& item)
{
    allScripts.append(item);
}

QStringList getContentToWriteInIndividualBashScript(const QString& dirname, const QString& passwd)
{
    QStringList list={};

    // append first item
    const QString& shebang="#!/bin/bash";
    list.append(shebang);
    list.append("\n"); // newline

    const QString &zip = "/usr/bin/zip";
    const QString namedWithExt = u->getNameWithExtensionAddedToEnd(dirname, "zip");
    QString command = R"("%1" -m -v -y -r -e -P "%2" "%3" "%4")";

    QString ps;

    if (QString::compare(passwd, "savage", Qt::CaseInsensitive)==0){
        ps="savage";

    }else if (QString::compare(passwd, "Arch-Manic-Reborn", Qt::CaseInsensitive)==0){
        ps="Arch-Manic-Reborn";

    }else if (QString::compare(passwd, "unleash-carnage", Qt::CaseInsensitive)==0){
        ps="unleash-carnage";

    }else if (QString::compare(passwd, "", Qt::CaseInsensitive)==0){
        qDebug()<<"Should compress without any password";
        ps="";

    }else{
        qDebug()<<"Got password: "<<passwd<<" which was not matched by any of my if-else statements";
        qDebug()<<"Because of that I will not use any password";
        ps="";
    }

    command = command.arg(zip, ps, u->getBasename(namedWithExt), u->getBasename(dirname));
    qDebug()<<"Using command: "<<command;

    list.append(command);

    return list;
}


QString createBashScript(const QString &dir, const QString& passwd) {
    QString name = "%1.sh";
    name = name.arg(dir);

    u->makeFile(name, "", true);
    u->makeFileExecutable(name);

    QStringList commands = getContentToWriteInIndividualBashScript(dir, passwd);
    u->writeListToFile(commands, name); // Write commands to bash script

    return name;
}

void moveFiles(const QList<QStringList> &list) {
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

void moveAllFilesIntoDirs(const QString &folder) {
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

QString startProcess(const QString &dirname, const QString &passwdCategory) {
    std::string pwd_std = dirname.toStdString();
    const char *pwd_const_char = pwd_std.c_str();
    chdir(pwd_const_char);
    qDebug() << "libcompress: Changed current working directory to: " << dirname;

    moveAllFilesIntoDirs(dirname);

    QStringList bashScripts = {};
    QStringList dirs = u->getAllDirsInDir_StringList_v2(dirname);
    qDebug() << "libcompress: Got dirs in dir: " << dirs;

            foreach(QString file, dirs) {
            QFileInfo fileInfo(file);
            const QString &bashScript = createBashScript(fileInfo.fileName(), passwdCategory);
            bashScripts.append(bashScript);
            allScripts.append(bashScript);
        qDebug()<<"Got bash script: "<<bashScript;
    }


//    const QString pwd= getParentDir(bash);
    const QString& legendBashFile= makeTheLegendBashFile(bashScripts);
    allScripts.append(legendBashFile);
//    const QString& pwd=dirname;

    return legendBashFile;
}