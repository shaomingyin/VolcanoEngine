//
//
#ifndef VOLCANO_VM_HPP
#define VOLCANO_VM_HPP

#include <QList>
#include <QPair>
#include <QString>
#include <QSharedPointer>
#include <QThread>
#include <QObject>
#include <QTimerEvent>
#include <QElapsedTimer>
#include <QPluginLoader>

#include <Volcano/Common.hpp>
#include <Volcano/FileSystem.hpp>
#include <Volcano/Drivers.hpp>
#include <Volcano/Gui.hpp>
#include <Volcano/Scene.hpp>
#include <Volcano/Module.hpp>

VOLCANO_BEGIN

typedef QSharedPointer<Module> ModulePtr;
typedef QList<ModulePtr> ModuleList;

typedef QSharedPointer<FileSystem> FileSystemPtr;
typedef QPair<QString, FileSystemPtr> FileSystemMap;
typedef QList<FileSystemMap> FileSystemList;

class VOLCANO_API VM: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int fpsMax READ fpsMax WRITE setFpsMax NOTIFY fpsMaxChanged)

public:
    VM(QObject *parent = nullptr);
    ~VM(void) override;

public:
    bool start(const QString &nativeRootPath, const QString &initrc, bool threaded = true);
    const QString &nativeRootPath(void) const;
    const QString &initrc(void) const;
    bool require(const QString &filename);
    const ModuleList &moduleList(void) const;
    bool mount(const QString &nativePath, const QString &path);
    bool unmount(const QString &nativePath, const QString &path);
    FileSystem::Type type(const QString &path);
    bool exists(const QString &path);
    bool enumDir(const QString &dirname, QStringList entryList);
    FilePtr openFile(const QString &filename, QIODevice::OpenMode mode = QIODevice::ReadOnly);
    int fps(void) const;
    int fpsMax(void) const;
    void setFpsMax(int v);

signals:
    void started(void);
    void finished(void);
    void error(const QString &msg);
    void fpsMaxChanged(int v);

protected:
    void timerEvent(QTimerEvent *event) override;
    void frame(void);
    void updateEvents(float elapsed);
    void updateModules(float elapsed);
    void updateScene(float elapsed);
    void updateGui(float elapsed);

protected slots:
    void threadStarted(void);
    void threadFinished(void);

private:

private:
    QThread *m_thread;
    QElapsedTimer m_elapsedTimer;
    int m_frameTimer;
    int m_fps;
    int m_fpsMax;
    qint64 m_minElapsed;

    QString m_nativeRootPath;
    QString m_initrc;

    FileSystemList m_fileSystemList;

    ModuleList m_moduleList;

    Drivers::Data m_driversData[2];
    int m_driverSlot;

    // Resouce Manager;
    // Gui Context;
    // Scene Context;
    // Module Manager;
};

VOLCANO_INLINE const QString &VM::nativeRootPath() const
{
    return m_nativeRootPath;
}

VOLCANO_INLINE const QString &VM::initrc(void) const
{
    return m_initrc;
}

VOLCANO_INLINE const ModuleList &VM::moduleList(void) const
{
    return m_moduleList;
}

VOLCANO_INLINE int VM::fps(void) const
{
    return m_fps;
}

VOLCANO_INLINE int VM::fpsMax(void) const
{
    return m_fpsMax;
}

VOLCANO_END

#endif // VOLCANO_VM_HPP
