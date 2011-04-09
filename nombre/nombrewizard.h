#pragma once

#include <QWizard>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>
#include <QRegExp>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QProcess>

namespace Ui {
    class NombreWizard;
}

class NombreWizard : public QWizard
{
    Q_OBJECT

public:
    explicit NombreWizard(QWidget *parent = 0);
    ~NombreWizard();

protected:
    //virutal int nextId() const;
    virtual bool validateCurrentPage();

private slots:
    void changeTargetFile();
    void changeRootDirectory();
    void changeNewFileName();

private:
    Ui::NombreWizard *ui;

    QFileInfo targetFile;
    QDir rootDir;
    QFileInfo newFile;
    QFile placeHolder;

    void updateTargetFileName();
    void updateRootDirectoryName();
    void updateNewFileName();

    bool copyFile( const QString& srcPath, const QString& dstPath );
    bool fileContains( const QRegExp& expr, const QString& srcPath );
    bool replaceInstances( const QRegExp& expr, const QString& rplc, const QString& srcPath, const QString& dstPath );
    void replaceInChildren( QDir& parent );
    void executeRename();
    void log( const QString& text );
};
