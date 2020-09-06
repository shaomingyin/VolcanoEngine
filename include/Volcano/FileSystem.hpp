//
//
#ifndef VOLCANO_FILESYSTEM_HPP
#define VOLCANO_FILESYSTEM_HPP

#include <QStringList>
#include <QSharedPointer>
#include <QObject>
#include <QIODevice>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

typedef QSharedPointer<QIODevice> FilePtr;

class FileSystem: public QObject
{
    Q_OBJECT

public:
    enum Type
    {
        TypeUnknown = 0,
        TypeDir,
        TypeFile
    };

public:
    FileSystem(QObject *parent = nullptr);
    ~FileSystem(void) override;

public:
    virtual bool init(const QString &nativePath);
    const QString &nativePath(void) const;
    virtual Type type(const QString &path);
    bool exists(const QString &path);
    virtual bool enumDir(const QString &dirname, QStringList &entryList);
    virtual FilePtr openFile(const QString &filename, QIODevice::OpenMode mode = QIODevice::ReadOnly);
    static bool checkNameChar(QChar c);
    static bool checkName(const QString &name);
    static bool checkPath(const QString &path);

protected:
    QString m_nativePath;
};

VOLCANO_INLINE const QString &FileSystem::nativePath(void) const
{
    return m_nativePath;
}

VOLCANO_INLINE bool FileSystem::exists(const QString &path)
{
    return (type(path) != TypeUnknown);
}

VOLCANO_END

#endif // VOLCANO_FILESYSTEM_HPP
