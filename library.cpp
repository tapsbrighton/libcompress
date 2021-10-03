#include "library.h"

#include <iostream>
#include <QString>
#include <QFileInfoList>
#include <QDebug>
#include <unistd.h>

// forward function declarations
QStringList getAllDirsInDir_StringList_v2(const QString& dirName);
void makeFileExecutable(const QString& filename);
void makeFile(const QString& fileName, const QString& errorMessage="", bool deleteIfExists= false);
bool writeListToFile(const QStringList& list, const QString& filename);
QString getNameWithExtensionAddedToEnd(const QString& dirname, const QString& ext="zip");
QString getBasename(const QString& name);
bool removeFile(const QString& filename);



QStringList allScripts={};


void removeAllCreatedBashScripts()
{
    foreach(const QString& script, allScripts){
        bool result= removeFile(script);

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
            QString msg=R"("%1")";
            const QString& scriptBasename=getBasename(script);
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
    const QString& action_string=R"(        "/bin/bash" "$script")";
    commands.append(action_string);

    const QString done_string="\tdone";
    commands.append(done_string);

    qDebug()<<"libmyutility: "<<"Got commands as: "<<commands;

    const QString& legendBashFile="legend.sh";
    makeFile(legendBashFile);
    makeFileExecutable(legendBashFile);

    writeListToFile(commands, legendBashFile);

//    QFile::copy(legendBashFile, "/home/taps/Music/"+legendBashFile);
//    qDebug()<<"Copied legend bash file for debug purposes.";

    return legendBashFile;

}

void clearListForAllScripts()
{
    allScripts.clear();
    qDebug()<<"libcompress: Cleared list for all scripts";
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

    const QString& zip="/usr/bin/zip";
    const QString namedWithExt= getNameWithExtensionAddedToEnd(dirname, "zip");
    QString command=R"("%1" -m -v -y -r -e -P "%2" "%3" "%4")";

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

    command=command.arg(zip, ps, getBasename(namedWithExt), getBasename(dirname));
    qDebug()<<"Using command: "<<command;

    list.append(command);

    return list;
}


QString createBashScript(const QString &dir, const QString& passwd) {
    QString name="%1.sh";
    name=name.arg(dir);

    makeFile(name, "", true);
    makeFileExecutable(name);

    QStringList commands= getContentToWriteInIndividualBashScript(dir, passwd);
    writeListToFile(commands, name); // Write commands to bash script

    return name;
}


QString startProcess(const QString& dirname, const QString& passwdCategory)
{
    std::string pwd_std=dirname.toStdString();
    const char* pwd_const_char=pwd_std.c_str();
    chdir(pwd_const_char);
    qDebug()<<"libcompress: Changed current working directory to: "<<dirname;

    QStringList bashScripts={};
    QStringList dirs= getAllDirsInDir_StringList_v2(dirname);
    qDebug()<<"libcompress: Got dirs in dir: "<<dirs;

    foreach(QString file, dirs){
        QFileInfo fileInfo(file);
        const QString& bashScript= createBashScript(fileInfo.fileName(), passwdCategory);
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
