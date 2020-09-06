//
//
#include <functional>

#include <QObject>
#include <QFileInfo>

#include <Volcano/ZipFileSystem.hpp>
#include <Volcano/VM.hpp>

VOLCANO_BEGIN

VM::VM(QObject *parent):
    QObject(parent),
    m_thread(nullptr),
    m_frameTimer(0),
    m_fps(0)
{
    setFpsMax(60);
}

VM::~VM(void)
{
}

bool VM::start(const QString &nativeRootPath, const QString &initrc, bool threaded)
{
    QFileInfo fi(nativeRootPath);
    if (!fi.isDir())
        return false;

    if (threaded)
    {
        m_thread = new QThread(this);
        if (m_thread == nullptr)
            return false;
        moveToThread(m_thread);
        connect(m_thread, &QThread::started, this, &VM::threadStarted, Qt::DirectConnection);
        connect(m_thread, &QThread::finished, this, &VM::threadFinished, Qt::DirectConnection);
    }
    else
        m_thread = QThread::currentThread();

    m_nativeRootPath = nativeRootPath;
    m_initrc = initrc;

    if (m_thread != nullptr)
        m_thread->start();

    return true;
}

bool VM::require(const QString &filename)
{
    if (!FileSystem::checkPath(filename))
        return false;

    return false;
}

bool VM::mount(const QString &nativePath, const QString &path)
{
    Q_ASSERT(QThread::currentThread() == m_thread);

    if (!FileSystem::checkPath(path))
        return false;

    QString np(path);
    if (np.size() > 1 && np.endsWith('/'))
        np.truncate(path.size() - 1);

    for (auto it(m_fileSystemList.begin()); it != m_fileSystemList.end(); ++it)
    {
        if (it->first == np && it->second->nativePath() == nativePath)
            return false;
    }

    FileSystemPtr fs;

    QFileInfo fi(nativePath);
    if (fi.isDir())
        fs.reset(new FileSystem());
    else if (fi.isFile())
        fs.reset(new ZipFileSystem());
    else
        return false;

    if (!fs || !fs->init(nativePath))
        return false;

    m_fileSystemList.prepend(qMakePair(np, fs));

    return true;
}

bool VM::unmount(const QString &nativePath, const QString &path)
{
    Q_ASSERT(QThread::currentThread() == m_thread);

    if (!FileSystem::checkPath(path))
        return false;

    QString np(path);
    if (np.size() > 1 && np.endsWith('/'))
        np.truncate(path.size() - 1);

    for (auto it(m_fileSystemList.begin()); it != m_fileSystemList.end(); ++it)
    {
        if (it->first == np && it->second->nativePath() == nativePath)
        {
            m_fileSystemList.erase(it);
            return true;
        }
    }

    return false;
}

FileSystem::Type VM::type(const QString &path)
{
    Q_ASSERT(QThread::currentThread() == m_thread);

    for (auto it(m_fileSystemList.begin()); it != m_fileSystemList.end(); ++it)
    {
        if (!path.startsWith(it->first))
            continue;
        QString np(path);
        np.remove(0, it->first.size());
        FileSystem::Type t = it->second->type(np);
        if (t != FileSystem::TypeUnknown)
            return t;
    }

    return FileSystem::TypeUnknown;
}

bool VM::exists(const QString &path)
{
    Q_ASSERT(QThread::currentThread() == m_thread);

    return (type(path) != FileSystem::TypeUnknown);
}

bool VM::enumDir(const QString &dirname, QStringList entryList)
{
    Q_ASSERT(QThread::currentThread() == m_thread);

    entryList.clear();

    for (auto it(m_fileSystemList.begin()); it != m_fileSystemList.end(); ++it)
    {
        if (!dirname.startsWith(it->first))
            continue;
        QString np(dirname);
        np.remove(0, it->first.size());
        it->second->enumDir(np, entryList);
    }

    return true;
}

FilePtr VM::openFile(const QString &filename, QIODevice::OpenMode mode)
{
    Q_ASSERT(QThread::currentThread() == m_thread);

    for (auto it(m_fileSystemList.begin()); it != m_fileSystemList.end(); ++it)
    {
        if (!filename.startsWith(it->first))
            continue;
        QString np(filename);
        np.remove(0, it->first.size());
        FilePtr file = it->second->openFile(np, mode);
        if (file)
            return file;
    }

    return FilePtr();
}

void VM::setFpsMax(int v)
{
    Q_ASSERT(QThread::currentThread() == m_thread);

    int tmp = (v < 1 ? 1 : v);
    if (m_fpsMax == tmp)
        return;

    m_minElapsed = 1000 / m_fpsMax;

    m_fpsMax = tmp;
    fpsMaxChanged(tmp);
}

void VM::timerEvent(QTimerEvent *event)
{
    Q_ASSERT(QThread::currentThread() == m_thread);

    if (event->timerId() == m_frameTimer)
        frame();
}

void VM::frame(void)
{
    Q_ASSERT(QThread::currentThread() == m_thread);

    qint64 pass = m_elapsedTimer.restart();
    float elapsed = float(pass) / 1000.0f;

    updateEvents(elapsed);

    updateModules(elapsed);

    updateScene(elapsed);

    updateGui(elapsed);

    // TODO build output for drivers.
}

void VM::updateEvents(float elapsed)
{

}

void VM::updateModules(float elapsed)
{

}

void VM::updateScene(float elapsed)
{

}

void VM::updateGui(float elapsed)
{

}

void VM::threadStarted(void)
{
    Q_ASSERT(QThread::currentThread() == m_thread);

    mount(m_nativeRootPath, "/");

    // load modules.

    m_frameTimer = startTimer(m_minElapsed, Qt::PreciseTimer);

    m_elapsedTimer.start();
}

void VM::threadFinished(void)
{
    Q_ASSERT(QThread::currentThread() == m_thread);

    m_nativeRootPath.clear();
    m_initrc.clear();

    killTimer(m_frameTimer);
    m_frameTimer = 0;

    m_moduleList.clear();

    finished();
}

VOLCANO_END
